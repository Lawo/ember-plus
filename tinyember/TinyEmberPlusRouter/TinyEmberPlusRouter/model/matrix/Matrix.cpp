/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "Matrix.h"

namespace model { namespace matrix
{
   Matrix::Matrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : Element(number, parent, identifier)
      , m_notificationSink(notificationSink)
   {}

   Matrix::~Matrix()
   {
      for(auto signal : m_targets)
         delete signal;

      for(auto signal : m_sources)
         delete signal;
   }
}}
