using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;
using EmberLib;

namespace EmberPlusRouter
{
   class Dispatcher : Model.IElementVisitor<EmberPlusRouter.Dispatcher.ElementToGlowOptions, GlowContainer>
   {
      public Model.Element Root { get; set; }

      #region GlowRootReady Event
      public class GlowRootReadyArgs : EventArgs
      {
         public GlowContainer Root { get; private set; }
         public Client SourceClient { get; private set; }
         public Model.Matrix Matrix { get; set; }

         public GlowRootReadyArgs(GlowContainer root, Client sourceClient)
         {
            Root = root;
            SourceClient = sourceClient;
         }
      }

      public event EventHandler<GlowRootReadyArgs> GlowRootReady;

      protected virtual void OnGlowRootReady(GlowRootReadyArgs oArgs)
      {
         if(GlowRootReady != null)
            GlowRootReady(this, oArgs);
      }
      #endregion

      public void DispatchGlow(GlowContainer glow, Client source)
      {
         var walker = new Walker(this, source);

         walker.Walk(glow);
      }

      public void NotifyParameterValueChanged(Model.ParameterBase parameter)
      {
         var options = new ElementToGlowOptions
         {
            DirFieldMask = GlowFieldFlags.Value
         };

         var glowParam = ElementToGlow(parameter, options);

         var glow = GlowRootElementCollection.CreateRoot();
         glow.Insert(glowParam);

         OnGlowRootReady(new GlowRootReadyArgs(glow, null));
      }

      public void NotifyParameterValueChanged(int[] parameterPath, GlowValue value)
      {
         var glowParam = new GlowQualifiedParameter(parameterPath)
         {
            Value = value,
         };

         var glow = GlowRootElementCollection.CreateRoot();
         glow.Insert(glowParam);

         OnGlowRootReady(new GlowRootReadyArgs(glow, null));
      }

      public void NotifyMatrixConnection(Model.Matrix matrix, Model.Signal target, object state)
      {
         var glow = GlowRootElementCollection.CreateRoot();
         var glowMatrix = new GlowQualifiedMatrix(matrix.Path);
         var glowConnection = new GlowConnection(target.Number)
         {
            Sources = target.ConnectedSources.Select(signal => signal.Number).ToArray(),
            Disposition = GlowConnectionDisposition.Modified,
         };

         glowMatrix.EnsureConnections().Insert(glowConnection);

         glow.Insert(glowMatrix);

         OnGlowRootReady(new GlowRootReadyArgs(glow, null) { Matrix = matrix });
      }

      #region Implementation
      GlowMatrixBase MatrixToGlow(Model.Matrix matrix, ElementToGlowOptions options)
      {
         var dirFieldMask = options.DirFieldMask;
         var glow = new GlowQualifiedMatrix(matrix.Path)
         {
            Identifier = matrix.Identifier,
            TargetCount = matrix.TargetCount,
            SourceCount = matrix.SourceCount,
         };

         if(dirFieldMask.HasBits(GlowFieldFlags.Description)
         && String.IsNullOrEmpty(matrix.Description) == false)
            glow.Description = matrix.Description;

         if(matrix.LabelsNode != null
         && dirFieldMask == GlowFieldFlags.All)
         {
            var labels = new EmberSequence(GlowTags.MatrixContents.Labels);
            labels.Insert(new GlowLabel { BasePath = matrix.LabelsNode.Path, Description = "Primary" });
            glow.Labels = labels;
         }

         if(dirFieldMask.HasBits(GlowFieldFlags.Connections)
         && options.IsCompleteMatrixEnquired)
         {
            var glowConnections = glow.EnsureConnections();

            foreach(var signal in matrix.Targets)
            {
               var glowConnection = new GlowConnection(signal.Number);

               if(signal.ConnectedSources.Any())
                  glowConnection.Sources = signal.ConnectedSources.Select(source => source.Number).ToArray();

               glowConnections.Insert(glowConnection);
            }
         }

         if((dirFieldMask == GlowFieldFlags.All)
         && String.IsNullOrEmpty(matrix.SchemaIdentifier) == false)
            glow.SchemaIdentifiers = matrix.SchemaIdentifier;

         return glow;
      }

      GlowContainer ElementToGlow(Model.Element element, ElementToGlowOptions options)
      {
         return element.Accept(this, options);
      }
      #endregion

      #region Class Walker
      class Walker : GlowWalker
      {
         public Walker(Dispatcher dispatcher, Client source)
         {
            _dispatcher = dispatcher;
            _source = source;
         }

         protected override void OnCommand(GlowCommand glow, int[] path)
         {
            Model.IDynamicPathHandler dynamicPathHandler;
            var parent = _dispatcher.Root.ResolveChild(path, out dynamicPathHandler);

            if(parent != null)
            {
               if(glow.Number == GlowCommandType.GetDirectory)
               {
                  var glowRoot = GlowRootElementCollection.CreateRoot();
                  var options = new ElementToGlowOptions { DirFieldMask = glow.DirFieldMask ?? GlowFieldFlags.All };

                  var visitor = new Model.InlineElementVisitor(
                     node =>
                     {
                        // "dir" in node
                        if(node.ChildrenCount == 0)
                        {
                           glowRoot.Insert(new GlowQualifiedNode(node.Path));
                        }
                        else
                        {
                           var glowChildren = from element in node.Children
                                              select _dispatcher.ElementToGlow(element, options);

                           foreach(var glowChild in glowChildren)
                              glowRoot.Insert(glowChild);
                        }
                     },
                     parameter =>
                     {
                        // "dir" in parameter
                        glowRoot.Insert(_dispatcher.ElementToGlow(parameter, options));
                     },
                     matrix =>
                     {
                        // "dir" in matrix
                        options.IsCompleteMatrixEnquired = true;
                        glowRoot.Insert(_dispatcher.ElementToGlow(matrix, options));
                        _source.SubscribeToMatrix(matrix, subscribe: true);
                     },
                     function =>
                     {
                        // "dir" in function
                        glowRoot.Insert(_dispatcher.ElementToGlow(function, options));
                     });

                  parent.Accept(visitor, null); // run inline visitor against parent

                  _source.Write(glowRoot);
               }
               else if(glow.Number == GlowCommandType.Unsubscribe)
               {
                  var visitor = new Model.InlineElementVisitor(
                     onMatrix: matrix => _source.SubscribeToMatrix(matrix, subscribe: false));

                  parent.Accept(visitor, null); // run inline visitor against parent
               }
               else if(glow.Number == GlowCommandType.Invoke)
               {
                  var visitor = new Model.InlineElementVisitor(
                     onFunction: function =>
                     {
                        var invocation = glow.Invocation;
                        var invocationResult = null as GlowInvocationResult;

                        try { invocationResult = function.Invoke(invocation); }
                        catch
                        {
                           if(invocation != null && invocation.InvocationId != null)
                           {
                              invocationResult = GlowInvocationResult.CreateRoot(invocation.InvocationId.Value);
                              invocationResult.Success = false;
                           }
                        }

                        if(invocationResult != null)
                           _source.Write(invocationResult);
                     });

                  parent.Accept(visitor, null); // run inline visitor against parent
               }
            }
            else
            {
               if(dynamicPathHandler != null)
                  dynamicPathHandler.HandleCommand(glow, path, _source);
            }
         }

         protected override void OnParameter(GlowParameterBase glow, int[] path)
         {
            Model.IDynamicPathHandler dynamicPathHandler;
            var parameter = _dispatcher.Root.ResolveChild(path, out dynamicPathHandler) as Model.ParameterBase;

            if(parameter != null)
            {
               var glowValue = glow.Value;

               if(glowValue != null)
               {
                  switch(glowValue.Type)
                  {
                     case GlowParameterType.Integer:
                     {
                        var integerParameter = parameter as Model.IntegerParameter;

                        if(integerParameter != null)
                           integerParameter.Value = glowValue.Integer;

                        break;
                     }

                     case GlowParameterType.String:
                     {
                        var stringParameter = parameter as Model.StringParameter;

                        if(stringParameter != null)
                           stringParameter.Value = glowValue.String;

                        break;
                     }
                  }
               }
            }
            else
            {
               if(dynamicPathHandler != null)
                  dynamicPathHandler.HandleParameter(glow, path, _source);
            }
         }

         protected override void OnMatrix(GlowMatrixBase glow, int[] path)
         {
            Model.IDynamicPathHandler dummy;
            var matrix = _dispatcher.Root.ResolveChild(path, out dummy) as Model.Matrix;

            if(matrix != null)
            {
               var connections = glow.Connections;

               if(connections != null)
               {
                  foreach(var connection in glow.TypedConnections)
                  {
                     var target = matrix.GetTarget(connection.Target);

                     if(target != null)
                     {
                        var glowSources = connection.Sources;
                        var sources = glowSources != null
                                      ? from sourceNumber in glowSources
                                        let source = matrix.GetSource(sourceNumber)
                                        where source != null
                                        select source
                                      : Enumerable.Empty<Model.Signal>();

                        var operation = connection.Operation != null
                                        ? (Model.ConnectOperation)connection.Operation.Value
                                        : Model.ConnectOperation.Absolute;

                        matrix.Connect(target, sources, _source, operation);
                     }
                  }
               }
            }
         }

         protected override void OnNode(GlowNodeBase glow, int[] path)
         {
         }

         protected override void OnStreamEntry(GlowStreamEntry glow)
         {
         }

         protected override void OnFunction(GlowFunctionBase glow, int[] path)
         {
         }

         protected override void OnInvocationResult(GlowInvocationResult glow)
         {
         }

         #region Implementation
         Dispatcher _dispatcher;
         Client _source;
         #endregion
      }
      #endregion

      #region IElementVisitor<ElementToGlowOptions,GlowContainer> Members
      class ElementToGlowOptions
      {
         public int DirFieldMask { get; set; }
         public bool IsCompleteMatrixEnquired { get; set; }
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.Node element, ElementToGlowOptions state)
      {
         var glow = new GlowQualifiedNode(element.Path);
         var dirFieldMask = state.DirFieldMask;

         if(dirFieldMask.HasBits(GlowFieldFlags.Identifier))
            glow.Identifier = element.Identifier;

         if((dirFieldMask.HasBits(GlowFieldFlags.Description))
         && String.IsNullOrEmpty(element.Description) == false)
            glow.Description = element.Description;

         if((dirFieldMask == GlowFieldFlags.All)
         && String.IsNullOrEmpty(element.SchemaIdentifier) == false)
            glow.SchemaIdentifiers = element.SchemaIdentifier;

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.IntegerParameter element, ElementToGlowOptions state)
      {
         var glow = new GlowQualifiedParameter(element.Path);
         var dirFieldMask = state.DirFieldMask;

         if(dirFieldMask.HasBits(GlowFieldFlags.Identifier))
            glow.Identifier = element.Identifier;

         if(dirFieldMask.HasBits(GlowFieldFlags.Description)
         && String.IsNullOrEmpty(element.Description) == false)
            glow.Description = element.Description;

         if(dirFieldMask.HasBits(GlowFieldFlags.Value))
            glow.Value = new GlowValue(element.Value);

         if(dirFieldMask == GlowFieldFlags.All)
         {
            glow.Minimum = new GlowMinMax(element.Minimum);
            glow.Maximum = new GlowMinMax(element.Maximum);

            if(element.IsWriteable)
               glow.Access = GlowAccess.ReadWrite;
         }

         if((dirFieldMask == GlowFieldFlags.All)
         && String.IsNullOrEmpty(element.SchemaIdentifier) == false)
            glow.SchemaIdentifiers = element.SchemaIdentifier;

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.StringParameter element, ElementToGlowOptions state)
      {
         var glow = new GlowQualifiedParameter(element.Path);
         var dirFieldMask = state.DirFieldMask;

         if(dirFieldMask.HasBits(GlowFieldFlags.Identifier))
            glow.Identifier = element.Identifier;

         if(dirFieldMask.HasBits(GlowFieldFlags.Description)
         && String.IsNullOrEmpty(element.Description) == false)
            glow.Description = element.Description;

         if(dirFieldMask.HasBits(GlowFieldFlags.Value))
            glow.Value = new GlowValue(element.Value);

         if(dirFieldMask == GlowFieldFlags.All)
         {
            if(element.IsWriteable)
               glow.Access = GlowAccess.ReadWrite;
         }

         if((dirFieldMask == GlowFieldFlags.All)
         && String.IsNullOrEmpty(element.SchemaIdentifier) == false)
            glow.SchemaIdentifiers = element.SchemaIdentifier;

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.OneToNMatrix element, ElementToGlowOptions state)
      {
         var glow = MatrixToGlow(element, state);

         if(state.DirFieldMask == GlowFieldFlags.All
         && state.IsCompleteMatrixEnquired)
         {
            if(element.Targets.Count() < element.TargetCount)
            {
               var glowTargets = glow.EnsureTargets();

               foreach(var signal in element.Targets)
                  glowTargets.Insert(new GlowTarget(signal.Number));
            }
            
            if(element.Sources.Count() < element.SourceCount)
            {
               var glowSources = glow.EnsureSources();

               foreach(var signal in element.Sources)
                  glowSources.Insert(new GlowSource(signal.Number));
            }
         }

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.NToNMatrix element, ElementToGlowOptions state)
      {
         var glow = MatrixToGlow(element, state);

         if(state.DirFieldMask == GlowFieldFlags.All)
         {
            glow.AddressingMode = GlowMatrixAddressingMode.NonLinear;
            glow.MatrixType = GlowMatrixType.NToN;

            if(element.ParametersNode != null)
               glow.ParametersLocation = new GlowParametersLocation(element.ParametersNode.Path);

            if(state.IsCompleteMatrixEnquired)
            {
               var glowTargets = glow.EnsureTargets();
               var glowSources = glow.EnsureSources();

               foreach(var signal in element.Targets)
                  glowTargets.Insert(new GlowTarget(signal.Number));

               foreach(var signal in element.Sources)
                  glowSources.Insert(new GlowSource(signal.Number));
            }
         }

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.DynamicMatrix element, ElementToGlowOptions state)
      {
         var glow = MatrixToGlow(element, state);

         if(state.DirFieldMask == GlowFieldFlags.All)
         {
            glow.MatrixType = GlowMatrixType.NToN;
            glow.ParametersLocation = new GlowParametersLocation(element.ParametersSubIdentifier);
            glow.GainParameterNumber = 1;
         }

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.OneToOneMatrix element, ElementToGlowOptions state)
      {
         var glow = MatrixToGlow(element, state);

         if(state.DirFieldMask == GlowFieldFlags.All)
         {
            glow.MatrixType = GlowMatrixType.OneToOne;
         }

         return glow;
      }

      GlowContainer Model.IElementVisitor<ElementToGlowOptions, GlowContainer>.Visit(Model.Function element, ElementToGlowOptions state)
      {
         var glow = new GlowQualifiedFunction(element.Path);
         var dirFieldMask = state.DirFieldMask;

         if(dirFieldMask.HasBits(GlowFieldFlags.Identifier))
            glow.Identifier = element.Identifier;

         if(dirFieldMask.HasBits(GlowFieldFlags.Description)
         && String.IsNullOrEmpty(element.Description) == false)
            glow.Description = element.Description;

         if(dirFieldMask == GlowFieldFlags.All)
         {
            if(element.Arguments != null)
            {
               var tupleItemDescs = from tuple in element.Arguments
                                    select new GlowTupleItemDescription(tuple.Item2) { Name = tuple.Item1 };
               var arguments = glow.EnsureArguments();

               foreach(var tupleItemDesc in tupleItemDescs)
                  arguments.Insert(tupleItemDesc);
            }

            if(element.Result != null)
            {
               var tupleItemDescs = from tuple in element.Result
                                    select new GlowTupleItemDescription(tuple.Item2) { Name = tuple.Item1 };
               var result = glow.EnsureResult();

               foreach(var tupleItemDesc in tupleItemDescs)
                  result.Insert(tupleItemDesc);
            }
         }

         return glow;
      }
      #endregion
   }
}
