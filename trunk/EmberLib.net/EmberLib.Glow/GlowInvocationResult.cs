using System;
using System.Collections.Generic;
using System.Text;
using BerLib;

namespace EmberLib.Glow
{
   public class GlowInvocationResult : GlowContainer
   {
      /// <summary>
      /// Creates a new instance of GlowInvocationResult.
      /// </summary>
      /// <remarks>
      /// Usually, objects of type GlowInvocationResult are created using
      /// the Method CreateRoot(), which assigns the root tag to the new
      /// object.
      /// </remarks>
      protected internal GlowInvocationResult(BerTag tag)
      : base(tag, GlowType.InvocationResult)
      {
      }

      /// <summary>
      /// Gets or sets the "invocationId" field.
      /// </summary>
      public int InvocationId
      {
         get { return GetChildValue<int>(GlowTags.InvocationResult.InvocationId); }
         set
         {
            var tag = GlowTags.InvocationResult.InvocationId;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "success" field.
      /// </summary>
      public bool? Success
      {
         get
         {
            var childNode = this[GlowTags.InvocationResult.Success] as BooleanEmberLeaf;

            if(childNode != null)
               return childNode.Value;

            return null;
         }
         set
         {
            var tag = GlowTags.InvocationResult.Success;

            Remove(tag);
            Insert(new BooleanEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "result" field.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Result
      {
         get { return this[GlowTags.InvocationResult.Result] as EmberSequence; }
         set
         {
            var tag = GlowTags.InvocationResult.Result;

            if(value.Tag != tag)
               throw new ArgumentException("Tag mismatch");

            Remove(tag);
            Insert(value);
         }
      }

      /// <summary>
      /// Gets or sets an typed enumeration over the tuple values contained in 
      /// in the "arguments" field.
      /// Getter returns null if field "arguments" not present.
      /// </summary>
      public IEnumerable<GlowValue> ResultValues
      {
         get
         {
            var result = Result;

            return result != null
                   ? InternalTools.EnumerateValues(result)
                   : null;
         }
         set
         {
            if(value == null)
               throw new ArgumentNullException();

            var sequence = new EmberSequence(GlowTags.InvocationResult.Result);

            foreach(var glowValue in value)
               sequence.Insert(InternalTools.ValueToLeaf(GlowTags.CollectionItem, glowValue));

            Result = sequence;
         }
      }

      /// <summary>
      /// Ensures that the "result" field is present, creating a
      /// new EmberSequence and assigning it to the "result" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "result" field, never null.</returns>
      public EmberSequence EnsureResult()
      {
         var result = Result;

         if(result == null)
         {
            result = new EmberSequence(GlowTags.InvocationResult.Result);
            Insert(result);
         }

         return result;
      }

      /// <summary>
      /// Creates a GlowInvocationResult instance with the outer tag identifying the object tree root.
      /// </summary>
      /// <param name="invocationId">The value of the "invocationId" field.</param>
      /// <returns>A new instance of GlowInvocationResult with the outer tag identifying the object tree root.</returns>
      public static GlowInvocationResult CreateRoot(int invocationId)
      {
         return new GlowInvocationResult(GlowTags.Root, invocationId);
      }

      /// <summary>
      /// Accepts a visitor of type IGlowVisitor
      /// </summary>
      /// <typeparam name="TState">Visitor state type</typeparam>
      /// <typeparam name="TResult">Visitor result type</typeparam>
      /// <param name="visitor">The visitor instance to accept</param>
      /// <param name="state">State object to pass to the visitor</param>
      /// <returns>The result of the visiting method</returns>
      public override TResult Accept<TState, TResult>(IGlowVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      #region Implementation
      GlowInvocationResult(BerTag tag, int invocationId)
      : this(tag)
      {
         InvocationId = invocationId;
      }
      #endregion
   }
}
