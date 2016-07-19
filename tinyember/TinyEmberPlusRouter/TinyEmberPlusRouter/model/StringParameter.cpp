/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

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
