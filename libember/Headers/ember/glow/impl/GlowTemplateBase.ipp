/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP
#define __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowFunction.hpp"
#include "../GlowNode.hpp"
#include "../GlowMatrix.hpp"
#include "../GlowParameter.hpp"

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
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    GlowNode * GlowTemplateBase::makeNode(int number) const
    {
        return new GlowNode(number, elementTag());
    }

    LIBEMBER_INLINE
    GlowMatrix * GlowTemplateBase::makeMatrix(int number) const
    {
        return new GlowMatrix(number, elementTag());
    }

    LIBEMBER_INLINE
    GlowFunction * GlowTemplateBase::makeFunction(int number) const
    {
        return new GlowFunction(number, elementTag());
    }

    LIBEMBER_INLINE
    GlowParameter * GlowTemplateBase::makeParameter(int number) const
    {
        return new GlowParameter(number, elementTag());
    }

}
}

#endif  // __LIBEMBER_GLOW_GLOWTEMPLATEBASE_IPP
