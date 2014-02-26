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

#ifndef __LIBEMBER_BER_TRAITS_TAG_HPP
#define __LIBEMBER_BER_TRAITS_TAG_HPP

#include "CodecTraits.hpp"
#include "../detail/MultiByte.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /*
     * Please note that the Tag type intentionally doesn't have UniversalTagTraits
     * specialization because it can't be encoded as a regular values.
     */

    /** EncodingTraits specialization for tags. */
    template<>
    struct EncodingTraits<Tag>
    {
        typedef Tag value_type;

        static std::size_t encodedLength(value_type tag)
        {
            return 1U + ((tag.number() < 0x1F) ? 0U : detail::getMultiByteEncodedLength(tag.number()));
        }

        static void encode(util::OctetStream& output, value_type tag)
        {
            std::size_t const preamble = tag.preamble() & 0xE0;
            std::size_t const number = tag.number();

            if (number < 0x1F)
            {
                output.append(static_cast<util::OctetStream::value_type>(preamble | (number & 0x1F)));
            }
            else
            {
                output.append(static_cast<util::OctetStream::value_type>(preamble | 0x1F));
                detail::encodeMultibyte(output, number);
            }
        }
    };

    /** DecodingTraits specialization for tags. */
    template<>
    struct DecodingTraits<Tag>
    {
        typedef Tag value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&)> signature;

        static value_type decode(util::OctetStream& input)
        {
            util::OctetStream::value_type byte = input.front();
            input.consume();

            Tag::Preamble const preamble = (byte & ~0x1F) & 0xE0;
            Tag::Number number = byte & 0x1F;
            if (number == 0x1F)
            {
                number = static_cast<Tag::Number>(detail::decodeMultibyte(input));
            }

            return make_tag(preamble, number);
        }
    };
}
}

#endif // __LIBEMBER_BER_TRAITS_TAG_HPP

