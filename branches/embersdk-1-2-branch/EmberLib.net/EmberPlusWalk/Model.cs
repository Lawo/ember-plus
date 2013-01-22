using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;
using System.Xml;

namespace EmberPlusWalk
{
   /// <summary>
   /// The only class used for the local object model.
   /// An Element instance can represent either a node, a parameter or
   /// the root of the local object tree.
   /// </summary>
   class Element : IGlowVisitor<object, bool>
   {
      /// <summary>
      /// Constructs a new instance of Element.
      /// </summary>
      /// <param name="parent">The parent element. Null if the new instance is the root element.</param>
      /// <param name="number">The node/parameter number of the new instance.</param>
      /// <param name="identifier">The node/parameter identifier of the new instance.</param>
      /// <param name="type">The type of the new instance.</param>
      /// <param name="xml">The XML representation of the GlowNode/GlowParameter used to create the new instance from.</param>
      public Element(Element parent, int number, string identifier, ElementType type, string xml)
      {
         Parent = parent;
         Number = number;
         Update(identifier, type, xml);
      }

      /// <summary>
      /// Gets the parent element. Null if this instance is the root element.
      /// </summary>
      public Element Parent { get; private set; }

      /// <summary>
      /// Gets the node/parameter number.
      /// </summary>
      public int Number { get; private set; }

      /// <summary>
      /// Gets the node/parameter identifier.
      /// </summary>
      public string Identifier { get; private set; }

      /// <summary>
      /// Gets the type of the element.
      /// </summary>
      public ElementType Type { get; private set; }

      /// <summary>
      /// Gets the XML representation of the node/parameter that has been
      /// used to create this element from.
      /// </summary>
      public string Xml { get; private set; }

      /// <summary>
      /// Gets the parameter type of the parameter this element represents.
      /// Only valid if Type is ElementType.Parameter.
      /// </summary>
      public int ParameterType { get; private set; }

      /// <summary>
      /// Gets the collection of child Element objects.
      /// </summary>
      public IEnumerable<Element> Children
      {
         get { return _children; }
      }

      /// <summary>
      /// Creates an ember/glow tree which can be used to issue
      /// the "dir" command to the remote host.
      /// </summary>
      /// <returns>The GlowContainer object that is the root of
      /// a glow tree that mirrors the element tree up this element,
      /// having the GlowCommand object as the single leaf.</returns>
      public GlowContainer GetDirectory()
      {
         var glow = new GlowCommand(GlowCommandType.GetDirectory);

         // this builds a QualifiedNode or QualifiedParameter,
         // containing the GetDirectory Command
         return BuildQualified(this, glow);

         // this builds a complete glow tree using the Node and
         // Parameter types (more verbose).
         //return BuildGlowTree(this, glow);
      }

      /// <summary>
      /// Creates an ember/glow tree which can be used to issue
      /// a parameter value change to the remote host.
      /// </summary>
      /// <returns>The GlowContainer object that is the root of
      /// a glow tree that mirrors the element tree up this element,
      /// having a GlowParameter object as the single leaf.</returns>
      public GlowContainer SetParameterValue(GlowValue value)
      {
         if(Type != ElementType.Parameter)
            throw new InvalidOperationException();

         var glow = new GlowParameter(Number)
         {
            Value = value,
         };

         // this builds a QualifiedNode or QualifiedParameter
         return BuildQualified(Parent, glow);

         // this builds a complete glow tree using the Node and
         // Parameter types (more verbose).
         //return BuildGlowTree(Parent, glow);
      }

      /// <summary>
      /// Creates a new instance of Element which is initialized as root.
      /// </summary>
      public static Element CreateRoot()
      {
         return new Element(null, 0, null, ElementType.Root, null);
      }

      /// <summary>
      /// Gets the descendant element with the specified path.
      /// This call only makes sense on the root element.
      /// </summary>
      /// <param name="path">Path to the element to get.</param>
      /// <param name="parent">Receives the parent element of the found element.</param>
      /// <returns>The found element or null if not found.</returns>
      public Element GetElementAt(int[] path, out Element parent)
      {
         var current = this;
         parent = null;

         for(var index = 0; index < path.Length; index++)
         {
            var child = (from elem in current.Children
                         where elem.Number == path[index]
                         select elem)
                         .FirstOrDefault();

            if(child == null)
            {
               parent = index == path.Length - 1
                        ? current
                        : null;

               return null;
            }
            else
            {
               parent = current;
               current = child;
            }
         }

         return current;
      }

      #region Implementation
      List<Element> _children = new List<Element>();

      void Update(string identifier, ElementType type, string xml)
      {
         if(identifier != null)
            Identifier = identifier;

         Type = type;

         if(xml != null)
            Xml = xml;
      }

      int[] BuildPath()
      {
         var path = new LinkedList<int>();
         var elem = this;

         while(elem != null && elem.Type != ElementType.Root)
         {
            path.AddFirst(elem.Number);
            elem = elem.Parent;
         }

         return path.ToArray();
      }

      static GlowRootElementCollection BuildQualified(Element local, GlowElement glow)
      {
         var qualified = null as GlowElement;

         if(local.Type != ElementType.Root)
         {
            var path = local.BuildPath();

            if(local.Type == ElementType.Parameter)
            {
               var qparam = new GlowQualifiedParameter(path);
               qparam.Children = new GlowElementCollection(GlowTags.QualifiedParameter.Children);
               qparam.Children.Insert(glow);
               qualified = qparam;
            }
            else if(local.Type == ElementType.Node)
            {
               var qnode = new GlowQualifiedNode(path);
               qnode.Children = new GlowElementCollection(GlowTags.QualifiedNode.Children);
               qnode.Children.Insert(glow);
               qualified = qnode;
            }
         }
         else
         {
            qualified = glow;
         }

         if(qualified != null)
         {
            var root = GlowRootElementCollection.CreateRoot();
            root.Insert(qualified);
            return root;
         }

         return null;
      }

      static GlowRootElementCollection BuildGlowTree(Element local, GlowElement glow)
      {
         while(local.Type != ElementType.Root)
         {
            GlowElement glowParent;

            if(local.Type == ElementType.Parameter)
            {
               var param = new GlowParameter(local.Number);
               param.Children = new GlowElementCollection(GlowTags.Parameter.Children);
               param.Children.Insert(glow);
               glowParent = param;
            }
            else
            {
               var node = new GlowNode(local.Number);
               node.Children = new GlowElementCollection(GlowTags.Node.Children);
               node.Children.Insert(glow);
               glowParent = node;
            }

            glow = glowParent;
            local = local.Parent;
         }

         var root = GlowRootElementCollection.CreateRoot();
         root.Insert(glow);
         return root;
      }

      static string BuildXml(EmberLib.EmberNode node)
      {
         var settings = new XmlWriterSettings
         {
            Indent = true,
            IndentChars = "  ",
            CloseOutput = false,
            OmitXmlDeclaration = true,
         };

         var buffer = new StringBuilder();
         using(var writer = XmlWriter.Create(buffer, settings))
            EmberLib.Xml.XmlExport.Export(node, writer);

         return buffer.ToString();
      }
      #endregion

      #region IGlowVisitor<object,bool> Members
      bool IGlowVisitor<object, bool>.Visit(GlowCommand glow,object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowElementCollection glow,object state)
      {
         var hasCompleteNodeOrParameter = false;

         foreach(var element in glow.Elements)
            hasCompleteNodeOrParameter |= element.Accept(this, state);

         return hasCompleteNodeOrParameter;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowRootElementCollection glow, object state)
      {
         var hasCompleteNodeOrParameter = false;

         foreach(var element in glow.Elements)
            hasCompleteNodeOrParameter |= element.Accept(this, state);

         return hasCompleteNodeOrParameter;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowNode glow, object state)
      {
         var local = (from elem in _children
                      where elem.Number == glow.Number
                      select elem)
                      .FirstOrDefault();

         var isComplete = glow.Identifier != null;
         var xml = isComplete
                   ? BuildXml(glow)
                   : null;

         if(local == null)
         {
            local = new Element(this, glow.Number, glow.Identifier, ElementType.Node, xml);

            _children.Add(local);
         }
         else
         {
            local.Update(glow.Identifier, ElementType.Node, xml);
         }

         var children = glow.Children;

         if(children != null)
            isComplete |= children.Accept(local, null);

         return isComplete;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedNode glow, object state)
      {
         Element parent;
         var local = GetElementAt(glow.Path, out parent);
         var isComplete = glow.Identifier != null;

         if(parent != null)
         {
            var xml = isComplete
                      ? BuildXml(glow)
                      : null;

            if(local == null)
            {
               local = new Element(parent, glow.Path.Last(), glow.Identifier, ElementType.Node, xml);

               parent._children.Add(local);
            }
            else
            {
               local.Update(glow.Identifier, ElementType.Node, xml);
            }

            var children = glow.Children;

            if(children != null)
               isComplete |= children.Accept(local, null);
         }

         return isComplete;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowParameter glow, object state)
      {
         var local = (from elem in _children
                      where elem.Number == glow.Number
                      select elem)
                      .FirstOrDefault();

         var xml = BuildXml(glow);

         if(local == null)
         {
            local = new Element(this, glow.Number, glow.Identifier, ElementType.Parameter, xml);

            _children.Add(local);
         }
         else
         {
            local.Update(glow.Identifier, ElementType.Parameter, xml);
         }

         var value = glow.Value;

         if(value != null)
            local.ParameterType = value.Type;

         return true;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedParameter glow, object state)
      {
         Element parent;
         var local = GetElementAt(glow.Path, out parent);
         var xml = BuildXml(glow);

         if(parent != null)
         {
            if(local == null)
            {
               local = new Element(parent, glow.Path.Last(), glow.Identifier, ElementType.Parameter, xml);

               parent._children.Add(local);
            }
            else
            {
               local.Update(glow.Identifier, ElementType.Parameter, xml);
            }

            var value = glow.Value;

            if(value != null)
               local.ParameterType = value.Type;
         }

         return true;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowStreamCollection glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowSubContainer glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowMatrix glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedMatrix glow, object state)
      {
         return false;
      }
      #endregion
   }

   /// <summary>
   /// Possible values for Element.Type.
   /// </summary>
   enum ElementType
   {
      Root,
      Node,
      Parameter,
   }
}
