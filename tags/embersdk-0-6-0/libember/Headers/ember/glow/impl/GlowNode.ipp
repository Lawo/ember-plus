#ifndef __LIBEMBER_GLOW_GLOWNODE_IPP
#define __LIBEMBER_GLOW_GLOWNODE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    GlowNode::GlowNode(int number)
        : GlowContentElement(GlowType::Node, GlowTags::NodeContents)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number, number));
    }

    LIBEMBER_INLINE
    GlowNode::GlowNode(int number, ber::Tag const& tag)
        : GlowContentElement(GlowType::Node, GlowTags::NodeContents, tag)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number, number));
    }

    LIBEMBER_INLINE
    GlowNode::GlowNode(ber::Tag const& tag)
        : GlowContentElement(GlowType::Node, tag)
    {}

    LIBEMBER_INLINE
    bool GlowNode::contains(GlowProperty const& property) const
    {
        if (property.value == GlowProperty::Number)
        {
            return find<dom::VariantLeaf>(begin(), end(), GlowTags::Number) != 0;
        }
        else
        {
            return contents().contains(property);
        }
    }

    LIBEMBER_INLINE
    void GlowNode::setDescription(std::string const& description)
    {
        Contents& content = contents();
        content.insert(content.begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Description, description));
    }

    LIBEMBER_INLINE
    void GlowNode::setIdentifier(std::string const& identifier)
    {
        Contents& content = contents();
        content.insert(content.begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Identifier, identifier));
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowNode::children()
    {
        Contents& content = contents();
        dom::Node* collection = find<dom::Node>(content.begin(), content.end(), GlowTags::Children);

        if (collection == 0)
        {
            collection = new GlowElementCollection(GlowTags::Children);
            content.insert(content.end(), collection);
        }

        return dynamic_cast<GlowElementCollection*>(collection);
    }

    LIBEMBER_INLINE
    int GlowNode::number() const
    {
        return util::LeafToValueType::as(find<dom::VariantLeaf>(begin(), end(), GlowTags::Number), -1);
    }

    LIBEMBER_INLINE
    std::string GlowNode::description() const
    {
        Contents const& content = contents();
        return util::LeafToValueType::as(find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Description), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowNode::identifier() const
    {
        Contents const& content = contents();
        return util::LeafToValueType::as(find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Identifier), std::string());
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowNode::children() const
    {
        Contents const& content = contents();
        dom::Node const* collection = find<dom::Node>(content.begin(), content.end(), GlowTags::Children);

        return dynamic_cast<GlowElementCollection const*>(collection);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWNODE_IPP
