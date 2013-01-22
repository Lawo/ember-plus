#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_IPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number, ber::Tag const& tag)
        : GlowElement(GlowType::Command, tag)
    {
        ber::Tag const numberTag = GlowTags::Command::Number();
        dom::Node* leaf = util::ValueTypeToVariantLeaf::create(numberTag, number.value());

        insert(begin(), leaf);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number)
        : GlowElement(GlowType::Command)
    {
        ber::Tag const numberTag = GlowTags::Command::Number();
        dom::Node* leaf = util::ValueTypeToVariantLeaf::create(numberTag, number.value());

        insert(begin(), leaf);
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(ber::Tag const& tag)
        : GlowElement(GlowType::Command, tag)
    {}

    LIBEMBER_INLINE
    int GlowCommand::number() const
    {
        ber::Tag const tag = GlowTags::Command::Number();
        const_iterator const first = begin();
        const_iterator const last = end();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValueType::as(leaf, -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_IPP
