using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   class OneToNMatrix : Matrix
   {
      public OneToNMatrix(int number,
                          Element parent,
                          string identifier,
                          Dispatcher dispatcher,
                          IEnumerable<Signal> targets,
                          IEnumerable<Signal> sources,
                          Node labelsNode,
                          int? targetCount = null,
                          int? sourceCount = null)
      : base(number, parent, identifier, dispatcher, targets, sources, labelsNode, targetCount, sourceCount)
      {
      }

      protected override bool ConnectOverride(Signal target, IEnumerable<Signal> sources, ConnectOperation operation)
      {
         target.Connect(sources.Take(1), isAbsolute: true);

         return true;
      }

      public override TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }
   }
}
