#ifndef __LIBEMBER_GLOW_UTIL_VALUETYPETOVARIANTLEAF_HPP
#define __LIBEMBER_GLOW_UTIL_VALUETYPETOVARIANTLEAF_HPP

#include "../../ber/Encoding.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../traits/ValueTypeToBerType.hpp"

namespace libember { namespace glow { namespace util
{
    /** 
     * Helper class that creates a variant leaf from a value.
     */
    struct ValueTypeToVariantLeaf
    {
        /** 
         * Creates a new variant leaf with an application tag and a value.
         * @param tag The application tag to set.
         * @param value The value to assign to the leaf.
         * @return Returns the leaf instance.
         */
        template<typename ValueType>
        static dom::VariantLeaf* create(ber::Tag const& tag, ValueType const& value)
        {
            dom::VariantLeaf* leaf = new dom::VariantLeaf(tag);

            leaf->value() = value;
            return leaf;
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_VALUETYPETOVARIANTLEAF_HPP
