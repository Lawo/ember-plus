#ifndef __TINYEMBERROUTER_MODEL_MATRIX_NONLINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_NONLINEARMATRIX_H

#include "Matrix.h"

namespace model { namespace matrix
{
   /**
     * Abstract base class for matrices with non-linear addressing mode.
     */
   class NonlinearMatrix : public Matrix
   {
   public:
      /**
        * Creates a new instance of NonlinearMatrix.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink The object to notify of parameter value changes
        *     and matrix connections.
        * @param firstTarget Iterator pointing to the first target to contain
        *     in the matrix.
        * @param lastTarget Iterator pointing behind the last target to contain
        *     in the matrix.
        * @param firstSource Iterator pointing to the first source to contain
        *     in the matrix.
        * @param lastSource Iterator pointing behind the last source to contain
        *     in the matrix.
        */
      template<typename SignalIterator>
      NonlinearMatrix(int number,
                      Element* parent,
                      std::string const& identifier,
                      NotificationSink* notificationSink,
                      SignalIterator firstTarget, SignalIterator lastTarget,
                      SignalIterator firstSource, SignalIterator lastSource);

   // overrides
   public:
      /**
        * Overridden to return the number of targets contained in
        * the collection returned by targets().
        */
      virtual int targetCount() const;

      /**
        * Overridden to return the number of sources contained in
        * the collection returned by sources().
        */
      virtual int sourceCount() const;

      /**
        * Overridden to find the target with the specified number.
        */
      virtual Signal* getTarget(int number) const;

      /**
        * Overridden to find the source with the specified number.
        */
      virtual Signal* getSource(int number) const;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   template<typename SignalIterator>
   NonlinearMatrix::NonlinearMatrix(int number,
                                    Element* parent,
                                    std::string const& identifier,
                                    NotificationSink* notificationSink,
                                    SignalIterator firstTarget, SignalIterator lastTarget,
                                    SignalIterator firstSource, SignalIterator lastSource)
      : Matrix(number, parent, identifier, notificationSink)
   {
      auto& targets = this->targets();
      auto& sources = this->sources();

      targets.insert(targets.end(), firstTarget, lastTarget);
      sources.insert(sources.end(), firstSource, lastSource);
   }
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_NONLINEARMATRIX_H
