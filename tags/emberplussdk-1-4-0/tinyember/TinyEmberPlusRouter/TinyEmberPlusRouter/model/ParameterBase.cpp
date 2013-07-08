#include "ParameterBase.h"

namespace model
{
   ParameterBase::ParameterBase(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : Element(number, parent, identifier)
      , m_notificationSink(notificationSink)
      , m_isReadOnly(false)
   {}
}