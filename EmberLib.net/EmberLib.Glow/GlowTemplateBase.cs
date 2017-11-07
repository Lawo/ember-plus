using System;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// This class serves as base class for the <see cref="GlowTemplate"/> and <see cref="GlowQualifiedTemplate"/>
   /// classes.
   /// </summary>
   public abstract class GlowTemplateBase : GlowContainer
   {
      /// <summary>
      /// Initializes a new instance of the <see cref="GlowTemplateBase"/> class.
      /// </summary>
      /// <param name="tag">The application tag of this instance.</param>
      /// <param name="type">The application defined type of the template.</param>
      protected GlowTemplateBase(BerTag tag, uint type)
         : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the element that is described by the template. This can be any of
      /// <see cref="GlowNode"/>, <see cref="GlowParameter"/>, <see cref="GlowFunction"/>
      /// or <see cref="GlowMatrix"/>.
      /// To set a template element, its tag must be set to <see cref="GlowTags.Template.Element"/>. Otherwise,
      /// an <see cref="ArgumentException"/> will be thrown.
      /// </summary>
      public GlowElement Element
      {
         get
         {
            return this[GlowTags.Template.Element] as GlowElement;
         }
         set
         {
            var tag = GlowTags.Template.Element;

            if (tag != value.Tag)
               throw new ArgumentException("Tag mismatch");

            Remove(tag);
            Insert(value);
         }
      }

      /// <summary>
      /// Gets or sets an optional description string.
      /// </summary>
      public string Description
      {
         get
         {
            return GetChildValue<string>(GlowTags.Template.Description);
         }
         set
         {
            var tag = GlowTags.Template.Description;

            Remove(tag);
            Insert(new StringEmberLeaf(tag, value));
         }
      }
   }
}
