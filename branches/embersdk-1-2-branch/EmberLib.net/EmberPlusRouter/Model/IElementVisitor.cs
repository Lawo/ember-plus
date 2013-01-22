using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   interface IElementVisitor<TState, TResult>
   {
      TResult Visit(Node element, TState state);
      TResult Visit(IntegerParameter element, TState state);
      TResult Visit(StringParameter element, TState state);
      TResult Visit(OneToNMatrix element, TState state);
      TResult Visit(NToNMatrix element, TState state);
      TResult Visit(DynamicMatrix element, TState state);
      TResult Visit(OneToOneMatrix element, TState state);
   }

   class InlineElementVisitor : Model.IElementVisitor<object, object>
   {
      public InlineElementVisitor(Action<Model.Node> onNode = null,
                                  Action<Model.ParameterBase> onParameter = null,
                                  Action<Model.Matrix> onMatrix = null)
      {
         _onNode = onNode;
         _onParameter = onParameter;
         _onMatrix = onMatrix;
      }

      #region Implementation
      Action<Model.Node> _onNode;
      Action<Model.ParameterBase> _onParameter;
      Action<Model.Matrix> _onMatrix;
      #endregion

      #region IElementVisitor<object,object> Members
      object Model.IElementVisitor<object, object>.Visit(Model.Node element, object state)
      {
         if(_onNode != null)
            _onNode(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.IntegerParameter element, object state)
      {
         if(_onParameter != null)
            _onParameter(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.StringParameter element, object state)
      {
         if(_onParameter != null)
            _onParameter(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.OneToNMatrix element, object state)
      {
         if(_onMatrix != null)
            _onMatrix(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.NToNMatrix element, object state)
      {
         if(_onMatrix != null)
            _onMatrix(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.DynamicMatrix element, object state)
      {
         if(_onMatrix != null)
            _onMatrix(element);

         return null;
      }

      object Model.IElementVisitor<object, object>.Visit(Model.OneToOneMatrix element, object state)
      {
         if(_onMatrix != null)
            _onMatrix(element);

         return null;
      }
      #endregion
   }
}
