/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "Node.h"
#include "ElementVisitor.h"

namespace model
{
   Node::Node(int number, Element* parent, std::string const& identifier)
      : Element(number, parent, identifier)
   {}

   // override
   void Node::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }
}
