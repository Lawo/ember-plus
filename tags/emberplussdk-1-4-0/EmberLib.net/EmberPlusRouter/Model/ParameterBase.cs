using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   abstract class ParameterBase : Element
   {
      public ParameterBase(int number, Element parent, string identifier, Dispatcher dispatcher, bool isWriteable)
      : base(number, parent, identifier)
      {
         Dispatcher = dispatcher;
         IsWriteable = isWriteable;
      }

      public Dispatcher Dispatcher { get; private set; }
      public bool IsWriteable { get; private set; }
   }
}
