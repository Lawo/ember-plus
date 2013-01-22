#ifndef __LIBEMBER_GLOW_UTIL_LEAFTOMINMAX_HPP
#define __LIBEMBER_GLOW_UTIL_LEAFTOMINMAX_HPP

#include "../../dom/VariantLeaf.hpp"
#include "../MinMax.hpp"

namespace libember { namespace glow { namespace util {
    /**
     * Helper structure used to create a MinMax instance from a leaf value.
     */
    struct LeafToMinMax
    {
        /** 
         * Creates a new MinMax instance from the provided variant leaf. The
         * leaf contain either a real or an integer value, otherwise this
         * method returns 0.
         * @param leaf The leaf to get the value from.
         * @return Returns the extracted value. The type of MinMax may be integer or real.
         */
        static MinMax as(dom::VariantLeaf const* leaf)
        {
            if (leaf != 0)
            {
                ber::Value const& value = leaf->value();
                ber::Tag const typeTag = leaf->typeTag();

                switch(typeTag.number())
                {
                    case ber::Type::Integer:
                        return MinMax(value.as<int>());

                    case ber::Type::Real:
                        return MinMax(value.as<double>());
                }
            }

            return MinMax(0);
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_LEAFTOMINMAX_HPP
