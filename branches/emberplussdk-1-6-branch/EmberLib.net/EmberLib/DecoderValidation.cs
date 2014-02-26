using System;
using System.Collections.Generic;
using System.Text;

namespace EmberLib
{
   /// <summary>
   /// Singleton that provides a global event raised when a validation error occurs on decoding a node.
   /// </summary>
   public class DecoderValidation
   {
      #region Singleton
      DecoderValidation()
      {
      }

      public static readonly DecoderValidation Instance = new DecoderValidation();
      #endregion

      #region ValidationError Event
      public class ValidationErrorArgs : EventArgs
      {
         public EmberNode Node { get; private set; }
         public string Message { get; private set; }

         public ValidationErrorArgs(EmberNode node, string message)
         {
            Node = node;
            Message = message;
         }
      }

      /// <summary>
      /// Raised when a validation error occurs on decoding a node.
      /// </summary>
      public event EventHandler<ValidationErrorArgs> ValidationError;

      protected virtual void OnValidationError(ValidationErrorArgs e)
      {
         if(ValidationError != null)
            ValidationError(this, e);
      }
      #endregion

      internal bool HasSubscriptions
      {
         get { return ValidationError != null; }
      }

      internal void RaiseValidationError(EmberNode node, string message)
      {
         OnValidationError(new ValidationErrorArgs(node, message));
      }
   }
}
