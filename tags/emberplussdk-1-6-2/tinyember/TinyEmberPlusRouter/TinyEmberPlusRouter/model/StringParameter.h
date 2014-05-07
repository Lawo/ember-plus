#ifndef __TINYEMBERROUTER_MODEL_STRINGPARAMETER_H
#define __TINYEMBERROUTER_MODEL_STRINGPARAMETER_H

#include "Parameter.h"

namespace model
{
   /**
     * Simple string parameter.
     */
   class StringParameter : public Parameter<std::string>
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
        */
      StringParameter(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink);

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
   };
}

#endif//__TINYEMBERROUTER_MODEL_INTEGERPARAMETER_H
