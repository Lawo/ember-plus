/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP
#define __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowTupleItemDescription::GlowTupleItemDescription(ber::Tag const& tag)
        : GlowContainer(GlowType::TupleItemDescription, tag)
    {}

    LIBEMBER_INLINE
    GlowTupleItemDescription::GlowTupleItemDescription(ParameterType const& type, std::string const& name)
        : GlowContainer(GlowType::TupleItemDescription)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::TupleItemDescription::Type(), type.value()));
        insert(end(), new dom::VariantLeaf(GlowTags::TupleItemDescription::Name(), name));
    }

    LIBEMBER_INLINE
    ParameterType GlowTupleItemDescription::type() const
    {
        ber::Tag const tag = GlowTags::TupleItemDescription::Type(); 
        const_iterator first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, int(0));
            return static_cast<ParameterType::_Domain>(value);
        }
        else
        {
            return ParameterType::None;
        }
    }

    LIBEMBER_INLINE
    std::string GlowTupleItemDescription::name() const
    {
        ber::Tag const tag = GlowTags::TupleItemDescription::Name(); 
        const_iterator first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            std::string const value = util::ValueConverter::valueOf(&*result, std::string());
            return value;
        }
        else
        {
            return std::string();
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP
