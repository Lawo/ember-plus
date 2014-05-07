/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

#include <string>
#include <vector>
#include "CodecTraits.hpp"
#include "../ObjectIdentifier.hpp"
#include "../detail/MultiByte.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /**
     * UniversalTagTraits specialization for object identifiers.
     */
    template<>
    struct UniversalTagTraits<ObjectIdentifier>
    {
        typedef std::string value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::RelativeObject);
        }
    };


    /** EncodingTraits specialization for object identifiers. */
    template<>
    struct EncodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;

        static std::size_t encodedLength(value_type const& value)
        {
            std::size_t length = 0;
            value_type::const_iterator first = value.begin();
            value_type::const_iterator const last = value.end();
            
            for (/* Nothing */; first != last; ++first)
            {
                length += detail::getMultiByteEncodedLength(*first);
            }

            return length;
        }

        static void encode(util::OctetStream& output, value_type const& value)
        {
            value_type::const_iterator first = value.begin();
            value_type::const_iterator const last = value.end();

            for (/* Nothing */; first != last; ++first)
            {
                detail::encodeMultibyte(output, *first);
            }
        }
    };

    /** DecodingTraits specialization for object identifiers. */
    template<>
    struct DecodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream& input, std::size_t size)
        {
            typedef ObjectIdentifier::value_type item_type;
            std::vector<item_type> items;
            while(size > 0)
            {
                item_type const item = static_cast<item_type>(detail::decodeMultibyte(input));
                std::size_t const encodedItemLength = detail::getMultiByteEncodedLength(item);

                items.push_back(item);
                size -= encodedItemLength;
            }

            return ObjectIdentifier(items.begin(), items.end());
        }
    };
}
}

#endif // __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

