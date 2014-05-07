/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_IPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowQualifiedMatrix::GlowQualifiedMatrix(ber::ObjectIdentifier const& path)
        : GlowMatrixBase(
            GlowType::QualifiedMatrix,
            GlowTags::ElementDefault(),
            GlowTags::QualifiedMatrix::Contents(),
            GlowTags::QualifiedMatrix::Children(),
            GlowTags::QualifiedMatrix::Targets(),
            GlowTags::QualifiedMatrix::Sources(),
            GlowTags::QualifiedMatrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedMatrix::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedMatrix::GlowQualifiedMatrix(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path)
        : GlowMatrixBase(
            GlowType::QualifiedMatrix,
            GlowTags::ElementDefault(),
            GlowTags::QualifiedMatrix::Contents(),
            GlowTags::QualifiedMatrix::Children(),
            GlowTags::QualifiedMatrix::Targets(),
            GlowTags::QualifiedMatrix::Sources(),
            GlowTags::QualifiedMatrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedMatrix::Path(), path));
        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowQualifiedMatrix::GlowQualifiedMatrix(ber::ObjectIdentifier const& path, ber::Tag const& tag)
        : GlowMatrixBase(
            GlowType::QualifiedMatrix,
            tag,
            GlowTags::QualifiedMatrix::Contents(),
            GlowTags::QualifiedMatrix::Children(),
            GlowTags::QualifiedMatrix::Targets(),
            GlowTags::QualifiedMatrix::Sources(),
            GlowTags::QualifiedMatrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Node::Number(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedMatrix::GlowQualifiedMatrix(ber::Tag const& tag)
        : GlowMatrixBase(
            GlowType::QualifiedMatrix,
            tag,
            GlowTags::QualifiedMatrix::Contents(),
            GlowTags::QualifiedMatrix::Children(),
            GlowTags::QualifiedMatrix::Targets(),
            GlowTags::QualifiedMatrix::Sources(),
            GlowTags::QualifiedMatrix::Connections())
    {}

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowQualifiedMatrix::path() const
    {
        ber::Tag const tag = GlowTags::QualifiedMatrix::Path();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            return util::ValueConverter::valueOf(&*result, ber::ObjectIdentifier());
        }
        else
        {
            return ber::ObjectIdentifier();
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_IPP
