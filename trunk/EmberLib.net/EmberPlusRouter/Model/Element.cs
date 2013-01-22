using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter.Model
{
   abstract class Element
   {
      public Element(int number, Element parent, string identifier)
      {
         Parent = parent;
         Number = number;
         Identifier = identifier;

         if(parent != null)
            parent.AddChild(this);
      }

      public Element Parent { get; private set; }
      public int Number { get; private set; }
      public string Identifier { get; private set; }

      public virtual int ChildrenCount
      {
         get { return 0; }
      }

      public virtual IEnumerable<Element> Children
      {
         get { yield break; }
      }

      public string Description { get; set; }

      public bool IsRoot
      {
         get { return Parent == null; }
      }

      public string IdentifierPath
      {
         get
         {
            lock(_sync)
            {
               if(_identifierPath == null)
               {
                  var list = new LinkedList<string>();
                  list.AddFirst(Identifier);

                  var element = Parent;

                  while(element.IsRoot == false)
                  {
                     list.AddFirst(element.Identifier);
                     element = element.Parent;
                  }

                  _identifierPath = String.Join("/", list);
               }
            }

            return _identifierPath;
         }
      }

      public int[] Path
      {
         get
         {
            lock(_sync)
            {
               if(_path == null)
               {
                  var path = new LinkedList<int>();
                  var elem = this;

                  while(elem.IsRoot == false)
                  {
                     path.AddFirst(elem.Number);
                     elem = elem.Parent;
                  }

                  _path = path.ToArray();
               }
            }

            return _path;
         }
      }

      protected object SyncRoot
      {
         get { return _sync; }
      }

      public virtual void AddChild(Element child)
      {
      }

      public Element ResolveChild(int[] path, out IDynamicPathHandler dynamicPathHandler)
      {
         var element = this;

         dynamicPathHandler = null;

         foreach(var number in path)
         {
            var child = (from elem in element.Children
                         where elem.Number == number
                         select elem)
                         .FirstOrDefault();

            if(child == null)
            {
               if(element is IDynamicPathHandler)
                  dynamicPathHandler = (IDynamicPathHandler)element;

               return null;
            }

            element = child;
         }

         return element;
      }

      public abstract TResult Accept<TState, TResult>(IElementVisitor<TState, TResult> visitor, TState state);

      #region Implementation
      string _identifierPath;
      int[] _path;
      object _sync = new object();
      #endregion
   }
}
