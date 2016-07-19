/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_IPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowQualifiedTemplate::GlowQualifiedTemplate(ber::Tag const& tag)
        : GlowTemplateBase(GlowType::QualifiedTemplate, tag)
    {

    }

    LIBEMBER_INLINE
    GlowQualifiedTemplate::GlowQualifiedTemplate(ber::ObjectIdentifier const& path)
        : GlowTemplateBase(GlowType::QualifiedTemplate, GlowTags::ElementDefault())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedTemplate::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedTemplate::GlowQualifiedTemplate(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path)
        : GlowTemplateBase(GlowType::QualifiedTemplate, GlowTags::ElementDefault())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedTemplate::Path(), path));

        if (parent)
            parent->insert(parent->end(), this);
    }

    LIBEMBER_INLINE
    GlowQualifiedTemplate::GlowQualifiedTemplate(ber::ObjectIdentifier const& path, ber::Tag const& tag)
        : GlowTemplateBase(GlowType::QualifiedTemplate, tag)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedTemplate::Path(), path));
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowQualifiedTemplate::path() const
    {
        ber::Tag const tag = GlowTags::QualifiedTemplate::Path();
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
    ber::Tag GlowQualifiedTemplate::descriptionTag() const
    {
        return GlowTags::QualifiedTemplate::Description();
    }

    LIBEMBER_INLINE
    ber::Tag GlowQualifiedTemplate::elementTag() const
    {
        return GlowTags::QualifiedTemplate::Element();
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_IPP
