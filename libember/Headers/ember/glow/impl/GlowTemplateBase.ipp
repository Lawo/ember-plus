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

#ifndef __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP
#define __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowTemplateBase::GlowTemplateBase(GlowType const& type, ber::Tag const& tag)
        : GlowElement(type, tag)
    {

    }

    LIBEMBER_INLINE
    void GlowTemplateBase::setDescription(std::string const& description)
    {
        ber::Tag tag = descriptionTag();

        insert(end(), new dom::VariantLeaf(tag, description));
    }

    LIBEMBER_INLINE
    void GlowTemplateBase::setElement(GlowElement* element)
    {
        if (element == 0 || element->applicationTag() != elementTag())
        {
            return;
        }

        insert(end(), element);
    }

    LIBEMBER_INLINE
    std::string GlowTemplateBase::description() const
    {
        ber::Tag const tag = descriptionTag();
        const_iterator first = begin();
        const_iterator last = end();
        const_iterator result = util::find_tag(first, last, tag);

        if (result != last)
        {
            std::string const value = util::ValueConverter::valueOf<std::string>(&*result, std::string());
            return value;
        }
        else
        {
            return std::string();
        }
    }

    LIBEMBER_INLINE
    GlowElement const* GlowTemplateBase::element() const
    {
        ber::Tag const tag = elementTag();
        const_iterator first = begin();
        const_iterator last = end();
        const_iterator result = util::find_tag(first, last, tag);

        if (result != last)
        {
            return dynamic_cast<GlowElement const*>(&*result);
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP
