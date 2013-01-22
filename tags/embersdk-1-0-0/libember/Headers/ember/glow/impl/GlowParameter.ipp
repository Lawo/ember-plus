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

#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowTags.hpp"
#include "../GlowNode.hpp"
#include "../GlowQualifiedNode.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number)
        : GlowParameterBase(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(GlowNode* parent, int number)
        : GlowParameterBase(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(GlowQualifiedNode* parent, int number)
        : GlowParameterBase(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }
    }
    
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number, ber::Tag const& tag)
        : GlowParameterBase(GlowType::Parameter, tag, GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(ber::Tag const& tag)
        : GlowParameterBase(GlowType::Parameter, tag, GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
    {}

    LIBEMBER_INLINE
    int GlowParameter::number() const
    {
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(begin(), end(), GlowTags::Parameter::Number());
        return util::ValueConverter::toValue(leaf, -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_IPP
