/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWCONNECTION_IPP
#define __LIBEMBER_GLOW_GLOWCONNECTION_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowConnection::GlowConnection(int target, ber::Tag const& tag)
        : GlowContainer(GlowType::Connection, tag)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Connection::Target(), target));
    }

    LIBEMBER_INLINE
    GlowConnection::GlowConnection(int target)
        : GlowContainer(GlowType::Connection)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Connection::Target(), target));
    }

    LIBEMBER_INLINE
    GlowConnection::GlowConnection(ber::Tag const& tag)
        : GlowContainer(GlowType::Connection, tag)
    {
    }

    LIBEMBER_INLINE
    void GlowConnection::setSources(ber::ObjectIdentifier const& sources)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Connection::Sources(), sources));
    }

    LIBEMBER_INLINE
    void GlowConnection::setOperation(ConnectionOperation const& operation)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Connection::Operation(), operation.value()));
    }

    LIBEMBER_INLINE
    void GlowConnection::setDisposition(ConnectionDisposition const& disposition)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Connection::Disposition(), disposition.value()));
    }

    LIBEMBER_INLINE
    int GlowConnection::target() const
    {
        ber::Tag const tag = GlowTags::Connection::Target();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, 0);
            return value;
        }
        else
        {
            return -1;
        }
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowConnection::sources() const
    {
        ber::Tag const tag = GlowTags::Connection::Sources();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            ber::ObjectIdentifier const value = util::ValueConverter::valueOf(&*result, ber::ObjectIdentifier());
            return value;
        }
        else
        {
            return ber::ObjectIdentifier();
        }
    }

    LIBEMBER_INLINE
    ConnectionOperation GlowConnection::operation() const
    {
        ber::Tag const tag = GlowTags::Connection::Operation();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, static_cast<int>(ConnectionOperation::Absolute));
            return static_cast<ConnectionOperation::_Domain>(value);
        }
        else
        {
            return ConnectionOperation::Absolute;
        }
    }

    LIBEMBER_INLINE
    ConnectionDisposition GlowConnection::disposition() const
    {
        ber::Tag const tag = GlowTags::Connection::Disposition();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, static_cast<int>(ConnectionDisposition::Tally));
            return static_cast<ConnectionDisposition::_Domain>(value);
        }
        else
        {
            return ConnectionDisposition::Tally;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCONNECTION_IPP
