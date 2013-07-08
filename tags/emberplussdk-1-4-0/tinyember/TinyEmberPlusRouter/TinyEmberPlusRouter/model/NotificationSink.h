#ifndef __TINYEMBERROUTER_MODEL_NOTIFICATIONSINK_H
#define __TINYEMBERROUTER_MODEL_NOTIFICATIONSINK_H

#include "../util/Types.h"

namespace model
{
   namespace matrix
   {
      class Matrix;
      class Signal;
   }

   /**
     * Interface that can be implemented to receive notifications from the DOM.
     */
   class NotificationSink
   {
   public:
      /**
        * Implement this method to handle connection changes from matrices.
        * @param matrix Pointer to the matrix object the connection change was issued on.
        * @param target Pointer to the target object that changed.
        * @param target State passed through by the caller that initiated the change.
        */
      virtual void notifyMatrixConnection(matrix::Matrix* matrix, matrix::Signal* target, void* state) = 0;

      /**
        * Implement this method to handle integer parameter value changes.
        * @param parameterPath Path of the parameter that changed.
        * @param value The new parameter value.
        */
      virtual void notifyParameterValueChanged(util::Oid const& parameterPath, int value) = 0;

      /**
        * Implement this method to handle string parameter value changes.
        * @param parameterPath Path of the parameter that changed.
        * @param value The new parameter value.
        */
      virtual void notifyParameterValueChanged(util::Oid const& parameterPath, std::string const& value) = 0;
   };
}

#endif//__TINYEMBERROUTER_MODEL_NOTIFICATIONSINK_H
