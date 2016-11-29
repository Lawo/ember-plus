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
      /// <param name="tag"></param>
      /// <param name="type"></param>
      protected GlowTemplateBase(BerTag tag, uint type)
         : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the element that is described by the template. This can be any of
      /// <see cref="GlowNode"/>, <see cref="GlowParameter"/>, <see cref="GlowFunction"/>
      /// or <see cref="GlowMatrix"/>.
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
