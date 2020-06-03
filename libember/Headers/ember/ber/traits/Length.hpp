/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_LENGTH_HPP
#define __LIBEMBER_BER_TRAITS_LENGTH_HPP

#include "CodecTraits.hpp"
#include "../Length.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /*
     * Please note that the Length<> types intentionally don't have UniversalTagTraits
     * specialization because they can't be encoded as regular tagged values.
     */

    /** EncodingTraits specialization for length types. */
    template<typename LengthType>
    struct EncodingTraits<Length<LengthType> >
    {
        typedef Length<LengthType> value_type;
        typedef typename value_type::value_type underlying_type;

        static std::size_t encodedLength(value_type length)
        {
            return length.isIndefinite() ? 1U : EncodingTraits<underlying_type>::encodedLength(length.value);
        }

        static void encode(util::OctetStream& output, value_type length)
        {
            if (length.isIndefinite())
            {
                output.append(0x80U);
            }
            else if (length.value <= 0x7FU)
            {
                output.append(static_cast<util::OctetStream::value_type>(length.value));
            }
            else
            {
                output.append(static_cast<util::OctetStream::value_type>(0x80U | EncodingTraits<underlying_type>::encodedLength(length.value)));
                EncodingTraits<underlying_type>::encode(output, length.value);
            }
        }
    };

    /** DecodingTraits specialization for length types. */
    template<typename LengthType>
    struct DecodingTraits<Length<LengthType> >
    {
        typedef Length<LengthType> value_type;
        typedef typename value_type::value_type underlying_type;

        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&)> signature;

        static Length<LengthType> decode(util::OctetStream& input)
        {
            underlying_type length = input.front();
            input.consume();

            if ((length & 0x80U) != 0U)
            {
                std::size_t bytes = length & 0x7FU;
                if (bytes == 0U)
                {
                    return value_type::INDEFINITE;
                }
                else
                {
                    length = 0U;
                    for (/* Nothing */; bytes > 0U; bytes -= 1U)
                    {
                        length = ((length << 8U) | input.front());
                        input.consume();
                    }
                }
            }
            return length;
        }
    };
}
}

#endif // __LIBEMBER_BER_TRAITS_LENGTH_HPP

