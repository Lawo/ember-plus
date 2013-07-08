using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   class Signal
   {
      public Signal(int number, StringParameter labelParameter)
      {
         Number = number;
         LabelParameter = labelParameter;
      }

      public int Number { get; private set; }
      public StringParameter LabelParameter { get; private set; }

      public IEnumerable<Signal> ConnectedSources
      {
         get { return _connectedSources; }
      }

      public int ConnectedSourcesCount
      {
         get { return _connectedSources.Count; }
      }

      public void Connect(IEnumerable<Signal> sources, bool isAbsolute)
      {
         if(isAbsolute)
         {
            _connectedSources.Clear();
            _connectedSources.AddRange(sources);
         }
         else
         {
            foreach(var source in sources)
            {
               if(_connectedSources.Contains(source) == false)
                  _connectedSources.Add(source);
            }
         }
      }

      public void Disconnect(IEnumerable<Signal> sources)
      {
         foreach(var signal in sources)
            _connectedSources.Remove(signal);
      }

      #region Implementation
      List<Signal> _connectedSources = new List<Signal>();
      #endregion
   }
}
