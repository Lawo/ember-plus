/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWFUNCTIONBASE_IPP
#define __LIBEMBER_GLOW_GLOWFUNCTIONBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../util/Validation.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowFunctionBase::GlowFunctionBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag)
        : GlowContentElement(type, tag, contentsTag)
        , m_childrenTag(childrenTag)
    {}

    LIBEMBER_INLINE
    bool GlowFunctionBase::contains(FunctionProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowFunctionBase::setDescription(std::string const& description)
    {
        contents().set(GlowTags::FunctionContents::Description(), description);
    }

    LIBEMBER_INLINE
    void GlowFunctionBase::setIdentifier(std::string const& identifier)
    {
#ifndef LIBEMBER_DISABLE_IDENTIFIER_VALIDATION
        libember::glow::util::Validation::assertIdentifierValid(identifier);
#endif//LIBEMBER_DISABLE_IDENTIFIER_VALIDATION

        contents().set(GlowTags::FunctionContents::Identifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowFunctionBase::setTemplateReference(ber::ObjectIdentifier const& path)
    {
        contents().set(GlowTags::FunctionContents::TemplateReference(), path);
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowFunctionBase::arguments()
    {
        iterator const first = contents().begin();
        iterator const last = contents().end();
        iterator const result = util::find_tag(first, last, GlowTags::FunctionContents::Arguments());

        dom::Sequence* container = 0;

        if (result != last)
        {
            container = dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            container = new dom::Sequence(GlowTags::FunctionContents::Arguments());
            contents().set(container);
        }

        return container;
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowFunctionBase::result()
    {
        iterator const first = contents().begin();
        iterator const last = contents().end();
        iterator const result = util::find_tag(first, last, GlowTags::FunctionContents::Result());

        dom::Sequence* container = 0;

        if (result != last)
        {
            container = dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            container = new dom::Sequence(GlowTags::FunctionContents::Result());
            contents().set(container);
        }

        return container;
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowFunctionBase::children()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, m_childrenTag);
        if (result != last)
        {
            return dynamic_cast<GlowElementCollection*>(&*result);
        }
        else
        {
            GlowElementCollection* collection = new GlowElementCollection(m_childrenTag);
            insert(last, collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    std::string GlowFunctionBase::description() const
    {
        ber::Value const value = contents().get(GlowTags::FunctionContents::Description());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowFunctionBase::identifier() const
    {
        ber::Value const value = contents().get(GlowTags::FunctionContents::Identifier());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowFunctionBase::arguments() const
    {
        const_iterator const first = contents().begin();
        const_iterator const last = contents().end();
        const_iterator const result = util::find_tag(first, last, GlowTags::FunctionContents::Arguments());
        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowFunctionBase::result() const
    {
        const_iterator const first = contents().begin();
        const_iterator const last = contents().end();
        const_iterator const result = util::find_tag(first, last, GlowTags::FunctionContents::Result());
        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowFunctionBase::children() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, m_childrenTag);
        if (result != last)
        {
            return dynamic_cast<GlowElementCollection const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowFunctionBase::templateReference() const
    {
        ber::Value const value = contents().get(GlowTags::FunctionContents::TemplateReference());
        return util::ValueConverter::valueOf(value, ber::ObjectIdentifier());
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWFUNCTIONBASE_IPP
