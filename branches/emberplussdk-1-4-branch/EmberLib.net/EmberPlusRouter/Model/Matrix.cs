using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   abstract class Matrix : Element
   {
      public Matrix(int number,
                    Element parent,
                    string identifier,
                    Dispatcher dispatcher,
                    IEnumerable<Signal> targets,
                    IEnumerable<Signal> sources,
                    Node labelsNode,
                    int? targetCount,
                    int? sourceCount)
      : base(number, parent, identifier)
      {
         Dispatcher = dispatcher;
         LabelsNode = labelsNode;

         _targets = new List<Signal>(targets);
         _sources = new List<Signal>(sources);

         _targetCount = targetCount ?? _targets.Count;
         _sourceCount = sourceCount ?? _sources.Count;
      }

      public Dispatcher Dispatcher { get; private set; }
      public Node LabelsNode { get; private set; }

      public IEnumerable<Signal> Targets
      {
         get { return _targets; }
      }

      public IEnumerable<Signal> Sources
      {
         get { return _sources; }
      }

      public int TargetCount
      {
         get { return _targetCount; }
      }

      public int SourceCount
      {
         get { return _sourceCount; }
      }

      public Signal GetTarget(int number)
      {
         return (from signal in _targets
                 where signal.Number == number
                 select signal)
                 .FirstOrDefault();
      }

      public Signal GetSource(int number)
      {
         return (from signal in _sources
                 where signal.Number == number
                 select signal)
                 .FirstOrDefault();
      }

      public bool Connect(Signal target, IEnumerable<Signal> sources, object state, ConnectOperation operation = ConnectOperation.Absolute)
      {
         if(_targets.Contains(target) == false)
            throw new ArgumentException("target");

         var firstSource = sources.FirstOrDefault();

         if(firstSource != null)
         {
            if(_sources.Contains(firstSource) == false)
               throw new ArgumentException("sources");
         }

         var result = ConnectOverride(target, sources, operation);

         if(result)
            Dispatcher.NotifyMatrixConnection(this, target, state);

         return result;
      }

      protected abstract bool ConnectOverride(Signal target, IEnumerable<Signal> sources, ConnectOperation operation);

      #region Implementation
      List<Signal> _targets;
      List<Signal> _sources;
      int _targetCount;
      int _sourceCount;
      #endregion
   }

   enum ConnectOperation
   {
      Absolute,
      Connect,
      Disconnect,
   }
}
