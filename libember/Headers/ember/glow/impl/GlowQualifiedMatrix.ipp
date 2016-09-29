/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
