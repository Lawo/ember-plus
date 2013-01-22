#ifndef __TINYEMBERROUTER_MODEL_INTEGERPARAMETER_H
#define __TINYEMBERROUTER_MODEL_INTEGERPARAMETER_H

#include "Parameter.h"

namespace model
{
   /**
     * Simple integer parameter.
     */
   class IntegerParameter : public Parameter<int>
   {
   public:
      /**
        * Creates a new instance of IntegerParameter.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink Pointer to the object receiving value change
        *     notifications.
        * @param minimum The parameter's minimum value.
        * @param maximum The parameter's maximum value.
        */
      IntegerParameter(int number, Element* parent, std::string identifier, NotificationSink* notificationSink, int minimum, int maximum);

      /**
        * Returns the parameter's minimum value.
        * @Return The parameter's minimum value.
        */
      inline int minimum() const { return m_minimum; }

      /**
        * Returns the parameter's maximum value.
        * @Return The parameter's maximum value.
        */
      inline int maximum() const { return m_maximum; }

      /**
        * Overridden to call the appropriate visit() overload.
        */
      void accept(ElementVisitor* visitor);

   protected:
      /**
        * Overridden to call the appropriate notifyParameterValueChanged() overload
        * on the notificationSink.
        */
      virtual void onValueChanged();

   private:
      int m_minimum;
      int m_maximum;
   };
}

#endif//__TINYEMBERROUTER_MODEL_INTEGERPARAMETER_H
