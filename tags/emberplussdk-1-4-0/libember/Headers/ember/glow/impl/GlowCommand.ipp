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

#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_IPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../CommandType.hpp"
#include "../GlowInvocation.hpp"
#include "../GlowTags.hpp"
#include "../GlowParameterBase.hpp"
#include "../GlowNodeBase.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (mask.value() != DirFieldMask::Default)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowRootElementCollection* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
            parent->insert(parent->end(), this);

        if (mask.value() != DirFieldMask::Default)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowNodeBase* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::Default)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowParameterBase* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::Default)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(ber::Tag const& tag)
        : GlowElement(GlowType::Command, tag)
    {}

    LIBEMBER_INLINE
    CommandType GlowCommand::number() const
    {
        ber::Tag const tag = GlowTags::Command::Number();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, static_cast<int>(CommandType::None));
            return static_cast<CommandType::_Domain>(value);
        }
        else
        {
            return CommandType::None;
        }
    }

    LIBEMBER_INLINE
    GlowInvocation const* GlowCommand::invocation() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, GlowTags::Command::Invocation());

        if (result != last)
        {
            return dynamic_cast<GlowInvocation const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    DirFieldMask GlowCommand::dirFieldMask() const
    {
        ber::Tag const tag = GlowTags::Command::DirFieldMask();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, 0);
            return DirFieldMask(value);
        }
        else
        {
            return DirFieldMask(0);
        }
    }

    LIBEMBER_INLINE
    void GlowCommand::setDirFieldMask(DirFieldMask const& value)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Command::DirFieldMask(), value.value()));
    }

    LIBEMBER_INLINE
    void GlowCommand::setInvocation(GlowInvocation* value)
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, GlowTags::Command::Invocation());

        if (result == last)
        {
            insert(last, value);
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_IPP
