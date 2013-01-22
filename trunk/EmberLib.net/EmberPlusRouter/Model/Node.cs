using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   class Node : Element
   {
      public Node(int number, Element parent, string identifier)
      : base(number, parent, identifier)
      {
      }

      public override IEnumerable<Element> Children
      {
         get { return _children; }
      }

      public override int ChildrenCount
      {
         get { return _children.Count; }
      }

      public override void AddChild(Element child)
      {
         _children.Add(child);
      }

      public static Node CreateRoot()
      {
         return new Node(0, null, null);
      }

      public override TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      #region Implementation
      List<Element> _children = new List<Element>();
      #endregion
   }
}
