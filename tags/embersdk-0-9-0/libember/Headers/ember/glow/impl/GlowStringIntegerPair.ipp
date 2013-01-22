#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/LeafToValueType.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStringIntegerPair::GlowStringIntegerPair(ber::Tag const& tag)
        : GlowContainer(GlowType::StringIntegerPair, tag)
    {}

    LIBEMBER_INLINE
    GlowStringIntegerPair::GlowStringIntegerPair(std::string const& name, int value)
        : GlowContainer(GlowType::StringIntegerPair)
    {
        ber::Tag const nameTag = GlowTags::StringIntegerPair::Name();
        ber::Tag const valueTag = GlowTags::StringIntegerPair::Value();

        insert(end(), util::ValueTypeToVariantLeaf::create(nameTag, name));
        insert(end(), util::ValueTypeToVariantLeaf::create(valueTag, value));
    }

    LIBEMBER_INLINE
    std::string GlowStringIntegerPair::name() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        ber::Tag const tag = GlowTags::StringIntegerPair::Name(); 
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValueType::as(leaf, std::string());
    }

    LIBEMBER_INLINE
    int GlowStringIntegerPair::value() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        ber::Tag const tag = GlowTags::StringIntegerPair::Value();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);
        
        return util::LeafToValueType::as(leaf, -1);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP
