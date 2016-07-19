/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTEMPLATE_IPP
#define __LIBEMBER_GLOW_GLOWTEMPLATE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowTemplate::GlowTemplate(ber::Tag const& tag)
        : GlowTemplateBase(GlowType::Template, tag)
    {

    }

    LIBEMBER_INLINE
    GlowTemplate::GlowTemplate(int number)
        : GlowTemplateBase(GlowType::Template, GlowTags::ElementDefault())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Template::Number(), number));
    }

    LIBEMBER_INLINE
    GlowTemplate::GlowTemplate(GlowRootElementCollection* parent, int number)
        : GlowTemplateBase(GlowType::Template, GlowTags::ElementDefault())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Template::Number(), number));

        if (parent)
            parent->insert(parent->end(), this);
    }

    LIBEMBER_INLINE
    GlowTemplate::GlowTemplate(int number, ber::Tag const& tag)
        : GlowTemplateBase(GlowType::Template, tag)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Template::Number(), number));
    }

    LIBEMBER_INLINE
    int GlowTemplate::number() const
    {
        ber::Tag const tag = GlowTags::Template::Number();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);

        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, static_cast<int>(0));
            return value;
        }
        else
        {
            return -1;
        }
    }

    LIBEMBER_INLINE
    ber::Tag GlowTemplate::descriptionTag() const
    {
        return GlowTags::Template::Description();
    }

    LIBEMBER_INLINE
    ber::Tag GlowTemplate::elementTag() const
    {
        return GlowTags::Template::Element();
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTEMPLATE_IPP
