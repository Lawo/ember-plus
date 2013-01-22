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

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDPARAMETER_IPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDPARAMETER_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowTags.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowQualifiedParameter::GlowQualifiedParameter(ber::ObjectIdentifier const& path)
        : GlowParameterBase(GlowType::QualifiedParameter, GlowTags::ElementDefault(), GlowTags::QualifiedParameter::Contents(), GlowTags::QualifiedParameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedParameter::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedParameter::GlowQualifiedParameter(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path)
        : GlowParameterBase(GlowType::QualifiedParameter, GlowTags::ElementDefault(), GlowTags::QualifiedParameter::Contents(), GlowTags::QualifiedParameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedParameter::Path(), path));
        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowQualifiedParameter::GlowQualifiedParameter(ber::ObjectIdentifier const& path, ber::Tag const& tag)
        : GlowParameterBase(GlowType::QualifiedParameter, tag, GlowTags::QualifiedParameter::Contents(), GlowTags::QualifiedParameter::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedParameter::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedParameter::GlowQualifiedParameter(ber::Tag const& tag)
        : GlowParameterBase(GlowType::QualifiedParameter, tag, GlowTags::QualifiedParameter::Contents(), GlowTags::QualifiedParameter::Children())
    {}

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowQualifiedParameter::path() const
    {
        ber::ObjectIdentifier default_;
        ber::Tag const tag = GlowTags::QualifiedParameter::Path();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            return util::ValueConverter::valueOf(&*result, default_);
        }
        else
        {
            return default_;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDPARAMETER_IPP
