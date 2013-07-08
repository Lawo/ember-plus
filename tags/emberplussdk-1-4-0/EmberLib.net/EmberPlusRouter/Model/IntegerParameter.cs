using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   class IntegerParameter : Parameter<long>
   {
      public IntegerParameter(int number, Element parent, string identifier, Dispatcher dispatcher, int min, int max, bool isWriteable)
      : base(number, parent, identifier, dispatcher, isWriteable)
      {
         Minimum = min;
         Maximum = max;
      }

      public long Minimum { get; private set; }
      public long Maximum { get; private set; }

      public override TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }
   }
}
