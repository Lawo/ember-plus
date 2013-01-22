#ifndef __TINYEMBERROUTER_MODEL_MATRIX_ONETONLINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_ONETONLINEARMATRIX_H

#include "LinearMatrix.h"

namespace model { namespace matrix
{
   /**
     * Matrix with 1:N connection semantics and linear addressing mode.
     */
   class OneToNLinearMatrix : public LinearMatrix
   {
   public:
      /**
        * Creates a new instance of OneToNLinearMatrix.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink The object to notify of parameter value changes
        *     and matrix connections.
        * @param targetCount The number of targets to create.
        * @param sourceCount The number of sources to create.
        */
      OneToNLinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount);

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);

   protected:
      /**
        * Overridden to execute connects according to 1:N connection semantics.
        */
      virtual bool connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);
   };
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_ONETONLINEARMATRIX_H
