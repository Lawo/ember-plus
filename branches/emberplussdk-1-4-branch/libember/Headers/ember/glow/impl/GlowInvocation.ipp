/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_GLOWINVOCATION_IPP
#define __LIBEMBER_GLOW_GLOWINVOCATION_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../util/Validation.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowInvocation::GlowInvocation(ber::Tag const& tag)
        : GlowContainer(GlowType::Invocation, tag)
    {
    }

    LIBEMBER_INLINE
    void GlowInvocation::setInvocationId(int id)
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::Invocation::InvocationId());

        if (result == last)
        {
            insert(last, new dom::VariantLeaf(GlowTags::Invocation::InvocationId(), id));
        }
        else
        {
            dom::VariantLeaf* node = dynamic_cast<dom::VariantLeaf*>(&*result);

            if (node != 0)
                node->setValue(id);
        }
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowInvocation::arguments()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::Invocation::Arguments());

        dom::Sequence* container = 0;

        if (result != last)
        {
            container = dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            container = new dom::Sequence(GlowTags::Invocation::Arguments());
            insert(last, container);
        }

        return container;
    }

    LIBEMBER_INLINE
    int GlowInvocation::invocationId() const
    {
        ber::Tag const tag = GlowTags::Invocation::InvocationId();
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
    dom::Sequence const* GlowInvocation::arguments() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, GlowTags::Invocation::Arguments());

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

#endif  // __LIBEMBER_GLOW_GLOWINVOCATION_IPP
