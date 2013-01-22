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

#ifndef __LIBEMBER_GLOW_GLOWNODEBASE_IPP
#define __LIBEMBER_GLOW_GLOWNODEBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"

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
    void GlowNodeBase::setDescription(std::string const& description)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::NodeContents::Description();

        content.insert(where, new dom::VariantLeaf(tag, description));
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setIdentifier(std::string const& identifier)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::NodeContents::Identifier();

        content.insert(where, new dom::VariantLeaf(tag, identifier));
    }

    LIBEMBER_INLINE
    void GlowNodeBase::setRoot(bool isRoot)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::NodeContents::IsRoot();

        content.insert(where, new dom::VariantLeaf(tag, isRoot));
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowNodeBase::children()
    {
        Contents::iterator const first = begin();
        Contents::iterator const last = end();
        dom::Node* collection = find_node<dom::Node>(first, last, m_childrenTag);

        if (collection == 0)
        {
            collection = new GlowElementCollection(m_childrenTag);
            insert(last, collection);
        }

        return dynamic_cast<GlowElementCollection*>(collection);
    }


    LIBEMBER_INLINE
    bool GlowNodeBase::isRoot() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::NodeContents::IsRoot();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, tag);
        
        return util::ValueConverter::toValue(leaf, false);
    }

    LIBEMBER_INLINE
    std::string GlowNodeBase::description() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::NodeContents::Description();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, tag);
        
        return util::ValueConverter::toValue(leaf, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowNodeBase::identifier() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::NodeContents::Identifier();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, tag);
        
        return util::ValueConverter::toValue(leaf, std::string());
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowNodeBase::children() const
    {
        Contents::const_iterator const first = begin();
        Contents::const_iterator const last = end();
        dom::Node const* collection = find_node<dom::Node>(first, last, m_childrenTag);

        return dynamic_cast<GlowElementCollection const*>(collection);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWNODEBASE_IPP
