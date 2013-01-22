#include "StringParameter.h"
#include "ElementVisitor.h"

namespace model
{
   StringParameter::StringParameter(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : Parameter(number, parent, identifier, notificationSink)
   {}

   void StringParameter::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }

   void StringParameter::onValueChanged()
   {
      auto notificationSink = this->notificationSink();

      if(notificationSink != nullptr)
         notificationSink->notifyParameterValueChanged(path(), value());
   }
}
