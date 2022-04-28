/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_DETAIL_MULTIBYTE_HPP
#define __LIBEMBER_BER_DETAIL_MULTIBYTE_HPP

#include "../../meta/EnableIf.hpp"
#include "../../meta/Signedness.hpp"
#include "../../util/OctetStream.hpp"

namespace libember { namespace ber { namespace detail
{
    /**
     * Return how many 7-bit words are required to encode an n-bit
     * unsigned integer value as a multibyte word.
     * @param value the unsigned integer value whose encoded length
     *      should be determined.
     * @return The number of words required to encode @p value.
     */
    template<typename ValueType>
    typename meta::EnableIf<
                meta::IsUnsigned<ValueType>,
                std::size_t
    >::type getMultiByteEncodedLength(ValueType value);

    /**
     * Encode an n-bit unsigned integer value as a multibyte word.
     * @param output a reference to the octet stream to which the value
     *      should be encoded..
     * @param value the unsigned integer value that should be encoded.
     */
    template<typename ValueType>
    typename meta::EnableIf<
                meta::IsUnsigned<ValueType>
    >::type encodeMultibyte(util::OctetStream& output, ValueType value);

    /**
     * Decode an n-bit unsigned integer encoded in multibyte form.
     * @param input a reference to the octet stream from which the value
     *      should be decoded..
     * @return The decoded value.
     */
    std::pair<unsigned long long, std::size_t> decodeMultibyte(util::OctetStream& input);


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline typename meta::EnableIf<
                        meta::IsUnsigned<ValueType>,
                        std::size_t
    >::type getMultiByteEncodedLength(ValueType value)
    {
        std::size_t const bitWords = (sizeof(ValueType) * 8) / 7;
        std::size_t i = 1;
        while (i <= bitWords)
        {
            ValueType const mask = ~((static_cast<ValueType>(1) << (7 * i)) - 1);
            if ((value & mask) == 0)
            {
                break;
            }
            i += 1;
        }
        return i;
    }

    template<typename ValueType>
    inline typename meta::EnableIf<
                meta::IsUnsigned<ValueType>
    >::type encodeMultibyte(util::OctetStream& output, ValueType value)
    {
        std::size_t const bitWords = (sizeof(ValueType) * 8) / 7;
        for (std::size_t i = 0; i < bitWords; ++i)
        {
            std::size_t const wordPos = (7 * (bitWords - i));
            ValueType const mask = ~((static_cast<ValueType>(1) << wordPos) - 1);
            if ((value & mask) != 0)
            {
                output.append(0x80 | static_cast<util::OctetStream::value_type>((value >> wordPos) & 0x7F));
            }
        }
        output.append(static_cast<util::OctetStream::value_type>(value & 0x7F));
    }

    inline std::pair<unsigned long long, std::size_t> decodeMultibyte(util::OctetStream& input)
    {
        unsigned long long result = 0;
        std::size_t byteCount = 0;
        util::OctetStream::value_type byte = 0;
        do
        {
            byte = input.front();   
            input.consume();
            ++byteCount;
            result = (result << 7) | (byte & ~0x80);
        } while ((byte & 0x80) != 0);
        return std::pair<unsigned long long, std::size_t>(result, byteCount);
    }

}
}
}

#endif  // __LIBEMBER_BER_DETAIL_MULTIBYTE_HPP

