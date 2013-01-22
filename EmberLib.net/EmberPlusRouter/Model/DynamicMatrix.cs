using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;

namespace EmberPlusRouter.Model
{
   class DynamicMatrix : Matrix, IDynamicPathHandler
   {
      public DynamicMatrix(int number,
                           Element parent,
                           string identifier,
                           Dispatcher dispatcher,
                           IEnumerable<Signal> targets,
                           IEnumerable<Signal> sources,
                           Node labelsNode)
      : base(number, parent, identifier, dispatcher, targets, sources, labelsNode, null, null)
      {
         _xpointParameters = new Dictionary<int,Dictionary<int,XpointParams>>();
         foreach(var target in targets)
         {
            var dict = new Dictionary<int, XpointParams>();

            foreach(var source in sources)
               dict.Add(source.Number, new XpointParams());

            _xpointParameters.Add(target.Number, dict);
         }
      }

      public int ParametersSubIdentifier
      {
         get { return s_parametersSubIdentifier; }
      }

      protected override bool ConnectOverride(Signal target, IEnumerable<Signal> sources, ConnectOperation operation)
      {
         if(operation == ConnectOperation.Disconnect)
            target.Disconnect(sources);
         else
            target.Connect(sources, operation == ConnectOperation.Absolute);

         return true;
      }

      public override TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      #region Implementation
      static readonly int s_parametersSubIdentifier = 0;
      Dictionary<int, Dictionary<int, XpointParams>> _xpointParameters;
      #endregion

      #region IDynamicPathHandler Members
      void IDynamicPathHandler.HandleParameter(GlowParameterBase parameter, int[] path, Client source)
      {
         var offset = Path.Length;

         if(path.Length == offset + 5
         && path[offset + 0] == ParametersSubIdentifier
         && path[offset + 1] == 3) // connections
         {
            Dictionary<int, XpointParams> dict;

            if(_xpointParameters.TryGetValue(path[offset + 2], out dict)) // target
            {
               XpointParams xpointParams;

               if(dict.TryGetValue(path[offset + 3], out xpointParams)) // source
               {
                  if(path[offset + 4] == 1) // gain
                  {
                     var value = parameter.Value;

                     if(value != null
                     && value.Type == GlowParameterType.Real)
                     {
                        xpointParams.Gain = value.Real;

                        Dispatcher.NotifyParameterValueChanged(path, new GlowValue(xpointParams.Gain));
                     }
                  }
               }
            }
         }
      }

      void IDynamicPathHandler.HandleCommand(GlowCommand command, int[] path, Client source)
      {
         if(command.Number == GlowCommandType.GetDirectory)
         {
            var offset = Path.Length;

            if(path.Length == offset + 4
            && path[offset + 0] == ParametersSubIdentifier
            && path[offset + 1] == 3) // connections
            {
               Dictionary<int, XpointParams> dict;

               if(_xpointParameters.TryGetValue(path[offset + 2], out dict)) // target
               {
                  XpointParams xpointParams;

                  if(dict.TryGetValue(path[offset + 3], out xpointParams)) // source
                  {
                     var gainPath = path.Concat(new[] { 1 }).ToArray();

                     var glow = new GlowQualifiedParameter(gainPath)
                     {
                        Identifier = "dynamicGain",
                        Value = new GlowValue(xpointParams.Gain),
                        Minimum = new GlowMinMax(XpointParams.MinimumGain),
                        Maximum = new GlowMinMax(XpointParams.MaximumGain),
                     };

                     var root = GlowRootElementCollection.CreateRoot();
                     root.Insert(glow);
                     source.Write(root);
                  }
               }
            }
         }
      }
      #endregion
   }

   class XpointParams
   {
      public const double MinimumGain = -128;
      public const double MaximumGain = 15;

      public XpointParams()
      {
         Gain = MinimumGain;
      }

      public double Gain { get; set; }
   }
}
