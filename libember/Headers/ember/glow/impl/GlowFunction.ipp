/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWFUNCTION_IPP
#define __LIBEMBER_GLOW_GLOWFUNCTION_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"
#include "../GlowNodeBase.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowFunction::GlowFunction(ber::Tag const& tag)
        : GlowFunctionBase(GlowType::Function, tag, GlowTags::Function::Contents(), GlowTags::Function::Children())
    {
    }

    LIBEMBER_INLINE
    GlowFunction::GlowFunction(int number)
        : GlowFunctionBase(GlowType::Function, GlowTags::ElementDefault(), GlowTags::Function::Contents(), GlowTags::Function::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Function::Number(), number));
    }

    LIBEMBER_INLINE
    GlowFunction::GlowFunction(GlowRootElementCollection* parent, int number)
        : GlowFunctionBase(GlowType::Function, GlowTags::ElementDefault(), GlowTags::Function::Contents(), GlowTags::Function::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Function::Number(), number));

        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowFunction::GlowFunction(GlowNodeBase* parent, int number)
        : GlowFunctionBase(GlowType::Function, GlowTags::ElementDefault(), GlowTags::Function::Contents(), GlowTags::Function::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Function::Number(), number));

        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowFunction::GlowFunction(int number, ber::Tag const& tag)
        : GlowFunctionBase(GlowType::Function, tag, GlowTags::Function::Contents(), GlowTags::Function::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Function::Number(), number));
    }

    LIBEMBER_INLINE
    int GlowFunction::number() const
    {
        ber::Tag const tag = GlowTags::Function::Number();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            return util::ValueConverter::valueOf(&*result, -1);
        }
        else
        {
            return -1;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWFUNCTION_IPP
