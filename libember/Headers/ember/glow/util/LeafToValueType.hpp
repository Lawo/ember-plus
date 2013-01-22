#ifndef __LIBEMBER_GLOW_UTIL_LEAFTOVALUETYPE_HPP
#define __LIBEMBER_GLOW_UTIL_LEAFTOVALUETYPE_HPP

#include "../../ber/Encoding.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../traits/ValueTypeToBerType.hpp"

namespace libember { namespace glow { namespace util 
{
    /**
     * Helper structure used to extract the value from a leaf.
     */
    struct LeafToValueType
    {
        /** 
         * Compares the type of the leaf with the one provided by passing the default value.
         * If the types are equal, the value of the leaf will be returned. Otherwise, the 
         * default value will be returned.
         * @note The only types supported are Integer (int), Real (double) and UTF8String (std::string).
         * @param leaf Leaf to extract the value from.
         * @param defaultValue The value that will be returned if the types don't match or the leaf is null.
         * @return Returns the leaf value or null, if the types are incompatible.
         */
        template<typename ValueType>
        static ValueType as(dom::VariantLeaf const* leaf, ValueType const& defaultValue)
        {
            typedef traits::ValueTypeToBerType<ValueType> BerType;
            ber::Type const type = BerType::berType();

            return leaf != 0 && leaf->typeTag().number() == type.value()
                ? leaf->value().as<ValueType>()
                : defaultValue;
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_LEAFTOVALUETYPE_HPP
