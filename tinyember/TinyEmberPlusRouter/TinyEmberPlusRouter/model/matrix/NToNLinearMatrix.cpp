/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "NToNLinearMatrix.h"
#include "../ElementVisitor.h"
#include "detail/Connect.h"

namespace model { namespace matrix
{
   NToNLinearMatrix::NToNLinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount)
      : LinearMatrix(number, parent, identifier, notificationSink, targetCount, sourceCount)
   {}

   void NToNLinearMatrix::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }

   bool NToNLinearMatrix::connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      return detail::connectNToN(target, sources, state, operation);
   }
}}
