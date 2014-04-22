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

#ifndef __LIBEMBER_GLOW_GLOWNODEBASE_IPP
#define __LIBEMBER_GLOW_GLOWNODEBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../util/Validation.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowNodeBase::GlowNodeBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag)
        : GlowContentElement(type, tag, contentsTag)
        , m_childrenTag(childrenTag)
    {}

    LIBEMBER_INLINE
    bool GlowNodeBase::contains(NodeProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setSchemaIdentifiers(std::string const& identifiers)
    {
        contents().set(GlowTags::NodeContents::SchemaIdentifiers(), identifiers);
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setDescription(std::string const& description)
    {
        contents().set(GlowTags::NodeContents::Description(), description);
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setIdentifier(std::string const& identifier)
    {
#ifndef LIBEMBER_DISABLE_IDENTIFIER_VALIDATION
        libember::glow::util::Validation::assertIdentifierValid(identifier);
#endif//LIBEMBER_DISABLE_IDENTIFIER_VALIDATION

        contents().set(GlowTags::NodeContents::Identifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setRoot(bool isRoot)
    {
        contents().set(GlowTags::NodeContents::IsRoot(), isRoot);
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setIsOnline(bool isOnline)
    {
        contents().set(GlowTags::NodeContents::IsOnline(), isOnline);
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowNodeBase::children()
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
    bool GlowNodeBase::isRoot() const
    {
        ber::Value const value = contents().get(GlowTags::NodeContents::IsRoot());
        return util::ValueConverter::valueOf(value, false);
    }

    LIBEMBER_INLINE
    std::string GlowNodeBase::description() const
    {
        ber::Value const value = contents().get(GlowTags::NodeContents::Description());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowNodeBase::identifier() const
    {
        ber::Value const value = contents().get(GlowTags::NodeContents::Identifier());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowNodeBase::schemaIdentifiers() const
    {
        ber::Value const value = contents().get(GlowTags::NodeContents::SchemaIdentifiers());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    bool GlowNodeBase::isOnline() const
    {
        ber::Value const value = contents().get(GlowTags::NodeContents::IsOnline());
        return util::ValueConverter::valueOf(value, true);
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowNodeBase::children() const
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

#endif  // __LIBEMBER_GLOW_GLOWNODEBASE_IPP
