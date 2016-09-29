/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_IPP
#define __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../util/Validation.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowInvocationResult::GlowInvocationResult(ber::Tag const& tag)
        : GlowContainer(GlowType::InvocationResult, tag)
    {
    }

    LIBEMBER_INLINE
    void GlowInvocationResult::setInvocationId(int id)
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::InvocationResult::InvocationId());

        if (result == last)
        {
            insert(last, new dom::VariantLeaf(GlowTags::InvocationResult::InvocationId(), id));
        }
        else
        {
            dom::VariantLeaf* node = dynamic_cast<dom::VariantLeaf*>(&*result);

            if (node != 0)
                node->setValue(id);
        }
    }

    LIBEMBER_INLINE
    void GlowInvocationResult::setSuccess(bool value)
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::InvocationResult::Success());

        if (result == last)
        {
            insert(last, new dom::VariantLeaf(GlowTags::InvocationResult::Success(), value));
        }
        else
        {
            dom::VariantLeaf* node = dynamic_cast<dom::VariantLeaf*>(&*result);

            if (node != 0)
                node->setValue(value);
        }
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowInvocationResult::result()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::InvocationResult::Result());

        dom::Sequence* container = 0;

        if (result != last)
        {
            container = dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            container = new dom::Sequence(GlowTags::InvocationResult::Result());
            insert(last, container);
        }

        return container;
    }

    LIBEMBER_INLINE
    int GlowInvocationResult::invocationId() const
    {
        ber::Tag const tag = GlowTags::InvocationResult::InvocationId();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, -1);
            return value;
        }
        else
        {
            return -1;
        }

    }

    LIBEMBER_INLINE
    bool GlowInvocationResult::success() const
    {
        ber::Tag const tag = GlowTags::InvocationResult::Success();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            bool const value = util::ValueConverter::valueOf(&*result, true);
            return value;
        }
        else
        {
            return true;
        }

    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowInvocationResult::result() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, GlowTags::InvocationResult::Result());

        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_IPP
