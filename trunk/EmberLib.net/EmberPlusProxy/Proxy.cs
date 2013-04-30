using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;
using System.Threading.Tasks;
using System.Threading;

namespace EmberPlusProxy
{
   class Proxy : IDisposable
   {
      public Proxy(IEnumerable<GlowEndPointDesc> endPoints, int maxPackageLength)
      {
         _endPoints = new List<Consumer.GlowEndPoint>(
            from ep in endPoints
            select new Consumer.GlowEndPoint(ep.HostName, ep.Port, maxPackageLength, ep.LocalNumber, (byte)ep.SlotId, ep.Description));

         foreach(var ep in _endPoints)
         {
            ep.GlowRootReady += EndPoint_GlowRootReady;
            ep.Notification += EndPoint_Notification;
            ep.StateChanged += EndPoint_StateChanged;
         }
      }

      #region GlowFromProviderReady Event
      public class GlowFromProviderReadyArgs : EventArgs
      {
         public EmberLib.Glow.GlowContainer Root { get; private set; }

         public GlowFromProviderReadyArgs(GlowContainer root)
         {
            Root = root;
         }
      }

      /// <summary>
      /// Raised when a valid ember/glow package has been read.
      /// </summary>
      public event EventHandler<GlowFromProviderReadyArgs> GlowFromProviderReady;

      protected virtual void OnGlowFromProviderReady(GlowFromProviderReadyArgs e)
      {
         if(GlowFromProviderReady != null)
            GlowFromProviderReady(this, e);
      }
      #endregion

      public void Connect()
      {
         Parallel.ForEach(_endPoints, ep => ep.Connect());

         _timer = new Timer(TimerCallback, null, 10 * 1000, 10 * 1000);
      }

      public void DispatchGlow(GlowContainer glow, Provider.Client source)
      {
         var visitor = new ConsumerToProviderTranslator(_endPoints);
         var newGlow = glow.Accept(visitor, null).FirstOrDefault();

         if(newGlow != null)
         {
            if(visitor.EndPointNumber != null)
            {
               // message to provider
               var endPoint = (from ep in _endPoints
                               where ep.LocalNumber == visitor.EndPointNumber.Value
                               select ep)
                               .FirstOrDefault();

               if(endPoint.State == Consumer.GlowEndPointState.Connected
               || endPoint.State == Consumer.GlowEndPointState.ProtocolProbing)
               {
                  endPoint.Write(newGlow);
               }
               else
               {
                  // if endpoint is offline, and a dir command has been issued
                  // on the endpoint node (top-level node) -> send empty top-level node
                  if(newGlow is GlowRootElementCollection)
                  {
                     var command = newGlow.FirstOrDefault() as GlowCommand;

                     if(command != null
                     && command.Number == GlowCommandType.GetDirectory)
                     {
                        var root = GlowRootElementCollection.CreateRoot();
                        root.Insert(new GlowNode(endPoint.LocalNumber));
                        source.Write(root);
                     }
                  }
               }
            }
            else
            {
               // message to proxy - usually command at root level
               source.Write(newGlow);
            }
         }
      }

      #region Implementation
      List<Consumer.GlowEndPoint> _endPoints;
      Timer _timer;
      object _sync = new object();

      void TimerCallback(object state)
      {
         var endPoints = _endPoints.ToArray();

         Parallel.ForEach(endPoints, ep =>
            {
               if(ep.State == Consumer.GlowEndPointState.Closed
               || ep.State == Consumer.GlowEndPointState.Error)
                  ep.Connect();
            });
      }

      void EndPoint_GlowRootReady(object sender, Consumer.GlowEndPoint.GlowRootReadyArgs e)
      {
         var ep = (Consumer.GlowEndPoint)sender;
         var visitor = new ProviderToConsumerTranslator(ep.LocalNumber);
         var newGlow = e.Root.Accept(visitor, null);

         // raise event, passing newGlow to Provider.GlowHost
         OnGlowFromProviderReady(new GlowFromProviderReadyArgs(newGlow));
      }

      void EndPoint_Notification(object sender, Consumer.GlowEndPoint.NotificationArgs e)
      {
      }

      void EndPoint_StateChanged(object sender, EventArgs e)
      {
         var glow = GlowRootElementCollection.CreateRoot();
         var ep = (Consumer.GlowEndPoint)sender;
         var glowNode = new GlowNode(ep.LocalNumber)
         {
            Identifier = ep.HostName + ":" + ep.TcpPort,
            IsOnline = ep.State == Consumer.GlowEndPointState.Connected,
         };

         glow.Insert(glowNode);
         OnGlowFromProviderReady(new GlowFromProviderReadyArgs(glow));
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         foreach(var ep in _endPoints)
            ep.Close();

         var timer = _timer;

         if(timer != null)
            timer.Dispose();

         _timer = null;
      }
      #endregion

      #region Class ProviderToConsumerTranslator
      class ProviderToConsumerTranslator : IGlowVisitor<object, GlowContainer>
      {
         public ProviderToConsumerTranslator(int endPointNumber)
         {
            EndPointNumber = endPointNumber;
         }

         public int EndPointNumber { get; private set; }

         #region Implementation
         const int StreamIdentifierFactor = 1000000;

         GlowContainer WrapWithEndPointNode(GlowContainer glow)
         {
            var newNode = new GlowNode(EndPointNumber);
            var children = new GlowElementCollection(GlowTags.Node.Children);

            children.Insert(glow);

            newNode.Children = children;
            return newNode;
         }

         int[] PrependPathWithEndPointNumber(int[] path)
         {
            var newPath = new int[path.Length + 1];
            newPath[0] = EndPointNumber;
            Array.Copy(path, 0, newPath, 1, path.Length);
            return newPath;
         }

         void RemapParameterStreamIdentifier(GlowParameterBase glow)
         {
            var streamIdentifier = glow.StreamIdentifier;

            if(streamIdentifier != null)
               glow.StreamIdentifier = streamIdentifier.Value + StreamIdentifierFactor * EndPointNumber;
         }

         void RemapMatrixParameterPaths(GlowMatrixBase glow)
         {
            var parametersLocation = glow.ParametersLocation;

            if(parametersLocation != null
            && parametersLocation.Kind == GlowParametersLocationKind.BasePath)
            {
               var newPath = PrependPathWithEndPointNumber(parametersLocation.BasePath);

               glow.ParametersLocation = new GlowParametersLocation(newPath);
            }

            var labels = glow.Labels;

            if(labels != null)
            {
               foreach(var label in glow.TypedLabels)
                  label.BasePath = PrependPathWithEndPointNumber(label.BasePath);
            }
         }

         void Walk(GlowElementCollectionBase elements, Action<GlowParameterBase> onParameter, Action<GlowMatrixBase> onMatrix)
         {
            foreach(var element in elements)
            {
               if(element is GlowParameterBase)
               {
                  if(onParameter != null)
                     onParameter((GlowParameterBase)element);
               }
               else if(element is GlowMatrixBase)
               {
                  if(onMatrix != null)
                     onMatrix((GlowMatrixBase)element);
               }
               else if(element is GlowNodeBase)
               {
                  var children = ((GlowNodeBase)element).Children;

                  if(children != null)
                     Walk(children, onParameter, onMatrix);
               }
            }
         }
         #endregion

         #region IGlowVisitor<object, GlowContainer> Members
         public GlowContainer Visit(GlowCommand glow, object state)
         {
            return WrapWithEndPointNode(glow);
         }

         public GlowContainer Visit(GlowNode glow, object state)
         {
            return WrapWithEndPointNode(glow);
         }

         public GlowContainer Visit(GlowParameter glow, object state)
         {
            return WrapWithEndPointNode(glow);
         }

         public GlowContainer Visit(GlowQualifiedParameter glow, object state)
         {
            var newPath = PrependPathWithEndPointNumber(glow.Path);
            var newQualified = new GlowQualifiedParameter(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedParameter.Path)
                  newQualified.Insert(ember);
            }

            return newQualified;
         }

         public GlowContainer Visit(GlowQualifiedNode glow, object state)
         {
            var newPath = PrependPathWithEndPointNumber(glow.Path);
            var newQualified = new GlowQualifiedNode(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedNode.Path)
                  newQualified.Insert(ember);
            }

            return newQualified;
         }

         public GlowContainer Visit(GlowRootElementCollection glow, object state)
         {
            var root = GlowRootElementCollection.CreateRoot();

            Walk(glow, RemapParameterStreamIdentifier, RemapMatrixParameterPaths);

            foreach(var element in glow.Elements)
               root.Insert(element.Accept(this, null));

            return root;
         }

         public GlowContainer Visit(GlowStreamCollection glow, object state)
         {
            foreach(var entry in glow.StreamEntries)
               entry.Accept(this, state);

            return glow;
         }

         public GlowContainer Visit(GlowStreamEntry glow, object state)
         {
            var streamIdentifier = glow.StreamIdentifier;
            glow.StreamIdentifier = streamIdentifier + StreamIdentifierFactor * EndPointNumber;
            return glow;
         }

         public GlowContainer Visit(GlowElementCollection glow, object state)
         {
            return glow;
         }

         public GlowContainer Visit(GlowSubContainer glow, object state)
         {
            return glow;
         }

         public GlowContainer Visit(GlowMatrix glow, object state)
         {
            return WrapWithEndPointNode(glow);
         }

         public GlowContainer Visit(GlowQualifiedMatrix glow, object state)
         {
            var newPath = PrependPathWithEndPointNumber(glow.Path);
            var newQualified = new GlowQualifiedMatrix(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedMatrix.Path)
                  newQualified.Insert(ember);
            }

            return newQualified;
         }

         public GlowContainer Visit(GlowFunction glow, object state)
         {
            return WrapWithEndPointNode(glow);
         }

         public GlowContainer Visit(GlowQualifiedFunction glow, object state)
         {
            var newPath = PrependPathWithEndPointNumber(glow.Path);
            var newQualified = new GlowQualifiedFunction(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedMatrix.Path)
                  newQualified.Insert(ember);
            }

            return newQualified;
         }

         public GlowContainer Visit(GlowInvocationResult glow, object state)
         {
            return glow;
         }
         #endregion
      }
      #endregion

      #region Class ConsumerToProviderTranslator
      class ConsumerToProviderTranslator : IGlowVisitor<object, IEnumerable<GlowContainer>>
      {
         public ConsumerToProviderTranslator(IEnumerable<Consumer.GlowEndPoint> endPoints)
         {
            EndPoints = endPoints;
         }

         public IEnumerable<Consumer.GlowEndPoint> EndPoints { get; private set; }
         public int? EndPointNumber { get; private set; }

         #region IGlowVisitor<object,IEnumerable<GlowContainer>> Members
         // - handle Command at top level (dir on root: return all endpoint nodes)
         public IEnumerable<GlowContainer> Visit(GlowCommand glow, object state)
         {
            // return top-level nodes (describing endpoints)
            var nodes = from ep in EndPoints
                        select new GlowNode(ep.LocalNumber)
                        {
                           Identifier = ep.HostName + ":" + ep.TcpPort,
                           Description = ep.Description,
                           IsOnline = ep.State == Consumer.GlowEndPointState.Connected,
                        };

            foreach(var node in nodes)
               yield return node;
         }

         // - handle Node, stripping outer node (turn Node.Children into GlowRootElementCollection)
         public IEnumerable<GlowContainer> Visit(GlowNode glow, object state)
         {
            EndPointNumber = glow.Number;

            var children = glow.Children;

            if(children != null)
            {
               foreach(var child in children.Elements)
                  yield return child;
            }
         }

         // - handle Qualified..., stripping first number in path
         public IEnumerable<GlowContainer> Visit(GlowQualifiedParameter glow, object state)
         {
            EndPointNumber = glow.Path[0];

            var newPath = glow.Path.Skip(1).ToArray();
            var newQualified = new GlowQualifiedParameter(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedParameter.Path)
                  newQualified.Insert(ember);
            }

            yield return newQualified;
         }

         public IEnumerable<GlowContainer> Visit(GlowQualifiedNode glow, object state)
         {
            EndPointNumber = glow.Path[0];

            if(glow.Path.Length == 1) // translate to root level
            {
               var children = glow.Children;

               if(children != null)
               {
                  foreach(var child in children.Elements)
                     yield return child;
               }
            }
            else
            {
               var newPath = glow.Path.Skip(1).ToArray();
               var newQualified = new GlowQualifiedNode(newPath);

               foreach(var ember in glow)
               {
                  if(ember.Tag != GlowTags.QualifiedNode.Path)
                     newQualified.Insert(ember);
               }

               yield return newQualified;
            }
         }

         public IEnumerable<GlowContainer> Visit(GlowRootElementCollection glow, object state)
         {
            var root = GlowRootElementCollection.CreateRoot();

            foreach(var element in glow.Elements)
            {
               var newElements = element.Accept(this, null);

               foreach(var newElement in newElements)
                  root.Insert(newElement);
            }

            yield return root;
         }

         public IEnumerable<GlowContainer> Visit(GlowQualifiedMatrix glow, object state)
         {
            EndPointNumber = glow.Path[0];

            var newPath = glow.Path.Skip(1).ToArray();
            var newQualified = new GlowQualifiedMatrix(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedMatrix.Path)
                  newQualified.Insert(ember);
            }

            yield return newQualified;
         }

         public IEnumerable<GlowContainer> Visit(GlowQualifiedFunction glow, object state)
         {
            EndPointNumber = glow.Path[0];

            var newPath = glow.Path.Skip(1).ToArray();
            var newQualified = new GlowQualifiedFunction(newPath);

            foreach(var ember in glow)
            {
               if(ember.Tag != GlowTags.QualifiedFunction.Path)
                  newQualified.Insert(ember);
            }

            yield return newQualified;
         }

         public IEnumerable<GlowContainer> Visit(GlowStreamCollection glow, object state)
         {
            yield return glow;
         }

         public IEnumerable<GlowContainer> Visit(GlowElementCollection glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowParameter glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowStreamEntry glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowSubContainer glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowMatrix glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowFunction glow, object state)
         {
            throw new NotImplementedException();
         }

         public IEnumerable<GlowContainer> Visit(GlowInvocationResult glow, object state)
         {
            throw new NotImplementedException();
         }
         #endregion
      }
      #endregion
   }
}
