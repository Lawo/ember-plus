using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   class NToNMatrix : Matrix
   {
      public NToNMatrix(int number,
                        Element parent,
                        string identifier,
                        Dispatcher dispatcher,
                        IEnumerable<Signal> targets,
                        IEnumerable<Signal> sources,
                        Node labelsNode,
                        Node parametersNode)
      : base(number, parent, identifier, dispatcher, targets, sources, labelsNode, null, null)
      {
         ParametersNode = parametersNode;
      }

      public Node ParametersNode { get; private set; }

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
   }
}
