/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_UTIL_VALUECONVERTER_HPP
#define __LIBEMBER_GLOW_UTIL_VALUECONVERTER_HPP

#include "../../ber/Encoding.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../traits/ValueTypeToBerType.hpp"

namespace libember { namespace glow { namespace util
{
    /**
     * Helper class to extract the value of a variant leaf if it matches the type specified
     * via a template parameter.
     */
    struct ValueConverter
    {
        public:
            /**
             * Tries to extract the provided leaf's value and returns it.
             * @param leaf The leaf to get the value from.
             * @param defaultval The default value to return when the conversion fails or the leaf is null.
             * @return Returns the extracted value on success or the passed default value if an error occurred.
             */
            template<typename ValueType>
            static ValueType toValue(dom::VariantLeaf const* leaf, ValueType const& defaultval)
            {
                typedef traits::ValueTypeToBerType<ValueType> typetraits;
                ber::Type const type = typetraits::berType();

                return (leaf != 0 && leaf->typeTag().number() == type.value())
                    ? leaf->value().template as<ValueType>()
                    : defaultval;
            };
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_VALUECONVERTER_HPP


