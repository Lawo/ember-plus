using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;

namespace EmberPlusRouter.Model
{
   class Function : Element
   {
      public Function(int number, Element parent, string identifier, Tuple<string, int>[] arguments, Tuple<string, int>[] result, Func<GlowValue[], GlowValue[]> coreFunc)
      : base(number, parent, identifier)
      {
         Arguments = arguments;
         Result = result;
         _coreFunc = coreFunc;
      }

      public GlowInvocationResult Invoke(GlowInvocation invocation)
      {
         GlowValue[] arguments;
         int? invocationId;

         if(invocation == null)
         {
            if(Arguments != null)
               throw new ArgumentException("Function with parameters called without arguments!");

            arguments = null;
            invocationId = null;
         }
         else
         {
            var argumentValues = invocation.ArgumentValues;

            arguments = argumentValues != null
                        ? argumentValues.ToArray()
                        : null;
            invocationId = invocation.InvocationId;

            AssertValueTypes(arguments, Arguments);
         }

         if(invocationId == null && HasResult)
            throw new ArgumentException("Function with result called without invocation id!");

         var result = _coreFunc(arguments);

         AssertValueTypes(result, Result);

         if(invocationId != null)
         {
            var invocationResult = GlowInvocationResult.CreateRoot(invocationId.Value);

            if(result != null)
               invocationResult.ResultValues = result;

            return invocationResult;
         }

         return null;
      }

      public Tuple<string, int>[] Arguments { get; private set; }
      public Tuple<string, int>[] Result { get; private set; }

      public bool HasResult
      {
         get { return Result != null; }
      }

      public override TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      #region Implementation
      Func<GlowValue[], GlowValue[]> _coreFunc;

      void AssertValueTypes(GlowValue[] values, Tuple<string, int>[] expected)
      {
         if(expected == null)
         {
            if(values != null)
               throw new ArgumentException();
         }
         else
         {
            if(values.Length != expected.Length)
               throw new ArgumentException();

            for(int index = 0; index < values.Length; index++)
            {
               if(values[index].Type != expected[index].Item2)
                  throw new ArgumentException();
            }
         }
      }
      #endregion
   }
}
