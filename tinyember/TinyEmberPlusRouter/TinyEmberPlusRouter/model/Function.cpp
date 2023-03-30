/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "Function.h"
#include "ElementVisitor.h"

namespace model
{
    Function::Delegate::~Delegate()
    {}

    Function::~Function()
    {
        delete m_delegate;
    }

    // override
    void Function::accept(ElementVisitor* visitor)
    {
        visitor->visit(this);
    }
}
