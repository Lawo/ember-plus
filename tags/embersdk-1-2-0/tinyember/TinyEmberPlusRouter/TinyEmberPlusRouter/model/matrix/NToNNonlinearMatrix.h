#ifndef __TINYEMBERROUTER_MODEL_MATRIX_NTONNONLINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_NTONNONLINEARMATRIX_H

#include "NonlinearMatrix.h"

namespace model { namespace matrix
{
   /**
     * Matrix with N:N connection semantics, non-linear addressing mode
     * and static parameters.
     */
   class NToNNonlinearMatrix : public NonlinearMatrix
   {
   public:
      /**
        * Creates a new instance of NToNNonlinearMatrix.
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
      NToNNonlinearMatrix(int number,
                          Element* parent,
                          std::string const& identifier,
                          NotificationSink* notificationSink,
                          SignalIterator firstTarget, SignalIterator lastTarget,
                          SignalIterator firstSource, SignalIterator lastSource);

      /**
        * Returns the path to the static parameters defined for this matrix.
        * @return The path to the static parameters defined for this matrix.
        *     This path is empty if not set.
        */
      inline util::Oid parametersPath() const { return m_parametersPath; }

      /**
        * Sets the path to the static parameters defined for this matrix.
        * @param value The path to the static parameters defined for this matrix.
        */
      inline void setParametersPath(util::Oid const& value)
      {
         m_parametersPath = value;
      }

      /**
        * Returns the number of the "gain" parameter for each crosspoint.
        * @return The number of the "gain" parameter for each crosspoint.
        *     -1 if no gain parameter defined.
        */
      inline int gainParameterNumber() const { return m_gainParameterNumber; }

      /**
        * Sets the number of the "gain" parameter for each crosspoint.
        * @param value The number of the "gain" parameter for each crosspoint.
        */
      inline void setGainParameterNumber(int value)
      {
         m_gainParameterNumber = value;
      }

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);

   protected:
      /**
        * Overridden to execute connects according to N:N connection semantics.
        */
      virtual bool connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);

   private:
      util::Oid m_parametersPath;
      int m_gainParameterNumber;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   template<typename SignalIterator>
   NToNNonlinearMatrix::NToNNonlinearMatrix(int number,
                                            Element* parent,
                                            std::string const& identifier,
                                            NotificationSink* notificationSink,
                                            SignalIterator firstTarget, SignalIterator lastTarget,
                                            SignalIterator firstSource, SignalIterator lastSource)
      : NonlinearMatrix(number, parent, identifier, notificationSink, firstTarget, lastTarget, firstSource, lastSource)
      , m_gainParameterNumber(-1)
   {}
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_NTONNONLINEARMATRIX_H
