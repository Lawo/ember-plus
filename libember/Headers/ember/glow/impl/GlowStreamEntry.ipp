#ifndef __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/LeafToValueType.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(ber::Tag const& tag)
        : GlowContainer(GlowType::StreamEntry, tag)
    {}

    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(int streamIdentifier, int value)
        : GlowContainer(GlowType::StreamEntry)
    {
        ber::Tag const idTag = GlowTags::StreamEntry::StreamIdentifier();
        ber::Tag const valueTag = GlowTags::StreamEntry::StreamValue();

        insert(end(), util::ValueTypeToVariantLeaf::create(idTag, streamIdentifier));
        insert(end(), util::ValueTypeToVariantLeaf::create(valueTag, value));
    }

    LIBEMBER_INLINE
    int GlowStreamEntry::streamIdentifier() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        ber::Tag const tag = GlowTags::StreamEntry::StreamIdentifier(); 
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValueType::as(leaf, -1);
    }

    LIBEMBER_INLINE
    int GlowStreamEntry::value() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        ber::Tag const tag = GlowTags::StreamEntry::StreamValue();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);
        
        return util::LeafToValueType::as(leaf, -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
