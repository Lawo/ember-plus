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
#include "traits/ValueConverterTraits.hpp"

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
             * Tries to cast to passed ber value to the specified value type.
             * If the ber value is of a different type, the provided default value will be returned.
             * @param value The ber value to read the value from.
             * @param default_ The default value to return when the conversion fails.
             * @return The concrete value or the default value, if the conversion failed.
             */
            template<typename ValueType>
            static ValueType valueOf(ber::Value const& value, ValueType const& default_)
            {
                if (value)
                {
                    ber::Type const type = glow::traits::ValueTypeToBerType<ValueType>::berType();
                    if (value.universalTag().number() == type.value())
                    {
                        return ValueConverterTraits<ValueType>::valueOf(value, default_);
                    }
                }
                return default_;
            }

            /**
             * Tries to extract the provided leaf's value and returns it.
             * @param leaf The leaf to get the value from.
             * @param default_ The default value to return when the conversion fails or the leaf is null.
             * @return Returns the extracted value on success or the passed default value if an error occurred.
             */
            template<typename ValueType>
            static ValueType valueOf(dom::VariantLeaf const* leaf, ValueType const& default_)
            {
                ber::Type const type = glow::traits::ValueTypeToBerType<ValueType>::berType();
                if (leaf != 0 && leaf->typeTag().number() == type.value())
                {
                    ber::Value const value = leaf->value();
                    if (value)
                    {
                        return ValueConverterTraits<ValueType>::valueOf(value, default_);
                    }
                }
                return default_;
            }

            /**
             * Tries to convert the passed node to a dom::VariantLeaf and extract its value.
             * @param node The node to get the value from.
             * @param default_ The default value to return when the conversion fails or the leaf is null.
             * @return Returns the extracted value on success or the passed default value if an error occurred.
             */
            template<typename ValueType>
            static ValueType valueOf(dom::Node const* node, ValueType const& default_)
            {
                return valueOf(dynamic_cast<dom::VariantLeaf const*>(node), default_);
            }

            /**
             * Tries to cast the node to a VariantLeaf and returns its value.
             * @param node The node to get the type erased value from.
             * @return The type erased value of the specified node.
             */
            static ber::Value valueOf(dom::Node const* node)
            {
                return valueOf(dynamic_cast<dom::VariantLeaf const*>(node));
            }

            /**
             * Returns the type erased value of the passed leaf node.
             * @param leaf The leaf to get the value from.
             * @return The type erased value of the specified node.
             */
            static ber::Value valueOf(dom::VariantLeaf const* leaf)
            {
                return (leaf != 0) ? leaf->value() : ber::Value();
            }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_VALUECONVERTER_HPP


