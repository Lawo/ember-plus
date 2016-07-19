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

        static std::size_t encodedLength(value_type length)
        {
            if ((length.value < 0x80 || length.isIndefinite()))
            {
                return 1;
            }
            else
            {
                // Create a bit-mask that has the nine most significant bits set
                LengthType const value = length.value;
                LengthType mask = static_cast<LengthType>(~((static_cast<LengthType>(1U) << (((sizeof(LengthType) - 1) * 8) - 1)) - 1));
                std::size_t encodedLength = sizeof(LengthType);
                while ((encodedLength > 1) && ((value & mask) == 0))
                {
                    encodedLength -= 1;
                    mask >>= 8;
                }
                if ((value >> (encodedLength * 8 - 1)) != 0)
                {
                    encodedLength += 1;
                }
                return 1 + encodedLength;
            }
        }

        static void encode(util::OctetStream& output, value_type length)
        {
            if (length.isIndefinite())
            {
                output.append(0x80);
            }
            else if (length.value <= 0x7F)
            {
                output.append(static_cast<util::OctetStream::value_type>(length.value));
            }
            else
            {
                output.append(static_cast<util::OctetStream::value_type>(0x80 | ber::encodedLength(static_cast<int>(length.value))));
                ber::encode(output, static_cast<int>(length.value));
            }
        }
    };

    /** DecodingTraits specialization for length types. */
    template<typename LengthType>
    struct DecodingTraits<Length<LengthType> >
    {
        typedef Length<LengthType> value_type;
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
            LengthType length = input.front();
            input.consume();

            if ((length & 0x80) != 0)
            {
                std::size_t bytes = length & 0x7F;
                if (bytes == 0)
                {
                    return value_type::INDEFINITE;
                }
                else
                {
                    length = 0;
                    for (/* Nothing */; bytes > 0; bytes -= 1)
                    {
                        length = ((length << 8) | input.front());
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

