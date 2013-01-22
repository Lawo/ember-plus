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
#include "../GlowTags.hpp"
#include "../GlowParameter.hpp"
#include "../GlowQualifiedParameter.hpp"
#include "../GlowNode.hpp"
#include "../GlowQualifiedNode.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (mask.value() != DirFieldMask::None)
            setDirFieldMask(mask);
    }


    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowParameter* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::None)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowQualifiedParameter* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::None)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowNode* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::None)
            setDirFieldMask(mask);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(GlowQualifiedNode* parent, CommandType const& number, DirFieldMask const& mask)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Command::Number(), number.value()));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }

        if (mask.value() != DirFieldMask::None)
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
        return static_cast<CommandType::_Domain>(util::ValueConverter::toValue(find_node<dom::VariantLeaf>(begin(), end(), tag), -1));
    }

    LIBEMBER_INLINE
    DirFieldMask GlowCommand::dirFieldMask() const
    {
        ber::Tag const tag = GlowTags::Command::DirFieldMask();
        int const result = util::ValueConverter::toValue(find_node<dom::VariantLeaf>(begin(), end(), tag), static_cast<int>(DirFieldMask::All));
        return DirFieldMask(result);
    }

    LIBEMBER_INLINE
    void GlowCommand::setDirFieldMask(DirFieldMask const& value)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Command::DirFieldMask(), value.value()));
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_IPP
