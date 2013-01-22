#ifndef __LIBEMBER_GLOW_UTIL_LEAFTOVALUE_HPP
#define __LIBEMBER_GLOW_UTIL_LEAFTOVALUE_HPP

#include "../../ber/Type.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../Value.hpp"

namespace libember { namespace glow { namespace util {
    /**
     * Helper class used to create a Value instance from a leaf node.
     */
    struct LeafToValue
    {
        /** 
         * Creates a new Value instance from the provided variant leaf. The
         * leaf contain either a real, an integer or a std::string value, otherwise this
         * method returns 0.
         * @param leaf The leaf to get the value from.
         * @return Returns the extracted value. The type of Value may be integer, real or std::string.
         */
        static Value as(dom::VariantLeaf const* leaf)
        {
            if (leaf != 0)
            {
                ber::Value const& value = leaf->value();
                ber::Tag const typeTag = leaf->typeTag();

                switch(typeTag.number())
                {
                    case ber::Type::Integer:
                        return Value(value.as<int>());

                    case ber::Type::Real:
                        return Value(value.as<double>());

                    case ber::Type::UTF8String:
                        return Value(value.as<std::string>());
                }
            }

            return Value(0);
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_LEAFTOVALUE_HPP
