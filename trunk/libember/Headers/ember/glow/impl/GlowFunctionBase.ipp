/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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
}
}

#endif  // __LIBEMBER_GLOW_GLOWFUNCTIONBASE_IPP
