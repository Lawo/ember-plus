#ifndef __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/LeafToValueType.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry()
        : GlowContainer(GlowType::StreamEntry)
    {}

    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(ber::Tag const& tag)
        : GlowContainer(GlowType::StreamEntry, tag)
    {}

    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(int streamIdentifier, int value)
        : GlowContainer(GlowType::StreamEntry)
    {
        setStreamIdentifier(streamIdentifier);
        setValue(value);
    }

    LIBEMBER_INLINE
    void GlowStreamEntry::setStreamIdentifier(int streamIdentifier)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::StreamIdentifier(), streamIdentifier));
    }

    LIBEMBER_INLINE
    void GlowStreamEntry::setValue(int value)
    {
        ber::Tag const tag = ber::make_tag(ber::Class::ContextSpecific, 0);
        insert(end(), util::ValueTypeToVariantLeaf::create(tag, value));
    }

    LIBEMBER_INLINE
    int GlowStreamEntry::streamIdentifier() const
    {
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(begin(), end(), GlowTags::StreamIdentifier());
        return util::LeafToValueType::as(leaf, -1);
    }

    LIBEMBER_INLINE
    int GlowStreamEntry::value() const
    {
        ber::Tag const tag = ber::make_tag(ber::Class::ContextSpecific, 0);
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(begin(), end(), tag);
        
        return util::LeafToValueType::as(leaf, -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
