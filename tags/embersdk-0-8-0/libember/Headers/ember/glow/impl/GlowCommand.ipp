#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_IPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number, ber::Tag const& tag)
        : GlowElement(GlowType::Command, tag)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number(), number.value()));
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(CommandType const& number)
        : GlowElement(GlowType::Command)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number(), number.value()));
    }

    LIBEMBER_INLINE
    GlowCommand::GlowCommand(ber::Tag const& tag)
        : GlowElement(GlowType::Command, tag)
    {}

    LIBEMBER_INLINE
    int GlowCommand::number() const
    {
        return util::LeafToValueType::as(find<dom::VariantLeaf>(begin(), end(), GlowTags::Number()), -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_IPP
