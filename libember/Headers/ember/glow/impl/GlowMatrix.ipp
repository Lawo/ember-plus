/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWMATRIX_IPP
#define __LIBEMBER_GLOW_GLOWMATRIX_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"
#include "../GlowNodeBase.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowMatrix::GlowMatrix(int number)
        : GlowMatrixBase(
            GlowType::Matrix,
            GlowTags::ElementDefault(),
            GlowTags::Matrix::Contents(),
            GlowTags::Matrix::Children(),
            GlowTags::Matrix::Targets(),
            GlowTags::Matrix::Sources(),
            GlowTags::Matrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Matrix::Number(), number));
    }

    LIBEMBER_INLINE
    GlowMatrix::GlowMatrix(GlowRootElementCollection* parent, int number)
        : GlowMatrixBase(
            GlowType::Matrix,
            GlowTags::ElementDefault(),
            GlowTags::Matrix::Contents(),
            GlowTags::Matrix::Children(),
            GlowTags::Matrix::Targets(),
            GlowTags::Matrix::Sources(),
            GlowTags::Matrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Matrix::Number(), number));
        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowMatrix::GlowMatrix(GlowNodeBase* parent, int number)
        : GlowMatrixBase(
            GlowType::Matrix,
            GlowTags::ElementDefault(),
            GlowTags::Matrix::Contents(),
            GlowTags::Matrix::Children(),
            GlowTags::Matrix::Targets(),
            GlowTags::Matrix::Sources(),
            GlowTags::Matrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Node::Number(), number));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowMatrix::GlowMatrix(int number, ber::Tag const& tag)
        : GlowMatrixBase(
            GlowType::Matrix,
            tag,
            GlowTags::Matrix::Contents(),
            GlowTags::Matrix::Children(),
            GlowTags::Matrix::Targets(),
            GlowTags::Matrix::Sources(),
            GlowTags::Matrix::Connections())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Node::Number(), number));
    }

    LIBEMBER_INLINE
    GlowMatrix::GlowMatrix(ber::Tag const& tag)
        : GlowMatrixBase(
            GlowType::Matrix,
            tag,
            GlowTags::Matrix::Contents(),
            GlowTags::Matrix::Children(),
            GlowTags::Matrix::Targets(),
            GlowTags::Matrix::Sources(),
            GlowTags::Matrix::Connections())
    {}

    LIBEMBER_INLINE
    int GlowMatrix::number() const
    {
        ber::Tag const tag = GlowTags::Node::Number();
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

#endif  // __LIBEMBER_GLOW_GLOWMATRIX_IPP
