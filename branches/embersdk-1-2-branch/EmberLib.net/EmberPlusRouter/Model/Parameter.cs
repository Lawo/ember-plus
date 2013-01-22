using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   abstract class Parameter<T> : ParameterBase
   {
      public Parameter(int number, Element parent, string identifier, Dispatcher dispatcher, bool isWriteable)
      : base(number, parent, identifier, dispatcher, isWriteable)
      {
      }

      public T Value
      {
         get { return _value; }
         set
         {
            lock(SyncRoot)
            {
               _value = value;

               Dispatcher.NotifyParameterValueChanged(this);
            }
         }
      }

      #region Implementation
      T _value;
      #endregion
   }
}
