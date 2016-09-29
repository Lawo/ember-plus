/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_IPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowQualifiedFunction::GlowQualifiedFunction(ber::Tag const& tag)
        : GlowFunctionBase(GlowType::QualifiedFunction, tag, GlowTags::QualifiedFunction::Contents(), GlowTags::QualifiedFunction::Children())
    {
    }

    LIBEMBER_INLINE
    GlowQualifiedFunction::GlowQualifiedFunction(ber::ObjectIdentifier const& path)
        : GlowFunctionBase(GlowType::QualifiedFunction, GlowTags::ElementDefault(), GlowTags::QualifiedFunction::Contents(), GlowTags::QualifiedFunction::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedFunction::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedFunction::GlowQualifiedFunction(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path)
        : GlowFunctionBase(GlowType::QualifiedFunction, GlowTags::ElementDefault(), GlowTags::QualifiedFunction::Contents(), GlowTags::QualifiedFunction::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedFunction::Path(), path));

        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowQualifiedFunction::GlowQualifiedFunction(ber::ObjectIdentifier const& path, ber::Tag const& tag)
        : GlowFunctionBase(GlowType::QualifiedFunction, tag, GlowTags::QualifiedFunction::Contents(), GlowTags::QualifiedFunction::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedFunction::Path(), path));
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowQualifiedFunction::path() const
    {
        ber::Tag const tag = GlowTags::QualifiedFunction::Path();
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

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_IPP
