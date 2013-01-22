#ifndef __LIBEMBER_GLOW_GLOWNODE_IPP
#define __LIBEMBER_GLOW_GLOWNODE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowNode::GlowNode(int number)
        : GlowContentElement(GlowType::Node, GlowTags::ElementDefault(), GlowTags::Node::Contents())
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Node::Number(), number));
    }

    LIBEMBER_INLINE
    GlowNode::GlowNode(int number, ber::Tag const& tag)
        : GlowContentElement(GlowType::Node, tag, GlowTags::Node::Contents())
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Node::Number(), number));
    }

    LIBEMBER_INLINE
    GlowNode::GlowNode(ber::Tag const& tag)
        : GlowContentElement(GlowType::Node, tag, GlowTags::Node::Contents())
    {}

    LIBEMBER_INLINE
    bool GlowNode::contains(NodeProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowNode::setDescription(std::string const& description)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::Node::Description();
        dom::VariantLeaf* leaf = util::ValueTypeToVariantLeaf::create(tag, description);

        content.insert(where, leaf);
    }

    LIBEMBER_INLINE
    void GlowNode::setIdentifier(std::string const& identifier)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::Node::Identifier();
        dom::VariantLeaf* leaf = util::ValueTypeToVariantLeaf::create(tag, identifier);

        content.insert(where, leaf);
    }

    LIBEMBER_INLINE
    void GlowNode::setRoot(bool isRoot)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::Node::IsRoot();
        dom::VariantLeaf* leaf = util::ValueTypeToVariantLeaf::create(tag, isRoot);

        content.insert(where, leaf);
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowNode::children()
    {
        Contents& content = contents();
        Contents::iterator const first = content.begin();
        Contents::iterator const last = content.end();
        ber::Tag const tag = GlowTags::Node::Children();
        dom::Node* collection = find<dom::Node>(first, last, tag);

        if (collection == 0)
        {
            collection = new GlowElementCollection(tag);
            content.insert(last, collection);
        }

        return dynamic_cast<GlowElementCollection*>(collection);
    }

    LIBEMBER_INLINE
    int GlowNode::number() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        ber::Tag const tag = GlowTags::Node::Number();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValueType::as(leaf, -1);
    }


    LIBEMBER_INLINE
    bool GlowNode::isRoot() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Node::IsRoot();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);
        
        return util::LeafToValueType::as(leaf, false);
    }

    LIBEMBER_INLINE
    std::string GlowNode::description() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Node::Description();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);
        
        return util::LeafToValueType::as(leaf, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowNode::identifier() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Node::Identifier();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);
        
        return util::LeafToValueType::as(leaf, std::string());
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowNode::children() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Node::Children();
        dom::Node const* collection = find<dom::Node>(first, last, tag);

        return dynamic_cast<GlowElementCollection const*>(collection);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWNODE_IPP
