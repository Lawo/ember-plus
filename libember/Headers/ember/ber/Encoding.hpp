/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_ENCODING_HPP
#define __LIBEMBER_BER_ENCODING_HPP

#include "traits/CodecTraits.hpp"
#include "traits/Tag.hpp"
#include "traits/Length.hpp"
#include "traits/Octets.hpp"
#include "traits/ObjectIdentifier.hpp"
#include "traits/Boolean.hpp"
#include "traits/Integral.hpp"
#include "traits/Null.hpp"
#include "traits/Real.hpp"
#include "traits/StdString.hpp"
#include "Value.hpp"

namespace libember { namespace ber
{
    /**
     * Generic encoded length for an entire frame, consisting of the universal
     * BER tag corresponding to ValueType, the data length and the data passed
     * in @p value.
     * @param value the payload value that should be encoded.
     * @return The number of bytes required to encode the frame.
     */
    template<typename ValueType>
    inline std::size_t encodedFrameLength(ValueType value)
    {
        std::size_t const valueLength = encodedLength(value);
        std::size_t const frameLength = encodedLength(universalTag<ValueType>()) + encodedLength(make_length(valueLength)) + valueLength; 
        return frameLength;
    }

    template<>
    inline std::size_t encodedFrameLength<Value>(Value value)
    {
        std::size_t const valueLength = value.encodedLength();
        std::size_t const frameLength = encodedLength(value.universalTag()) + encodedLength(make_length(valueLength)) + valueLength; 
        return frameLength;
    }

    /**
     * Generic encode function that encodes an entire frame, consisting of the
     * universal BER tag corresponding to ValueType, the data length and the data
     * passed in @p value.
     * @param output a reference to a stream buffer to which the frame should
     *      be encoded.
     * @param value the payload value that should be encoded.
     */
    template<typename ValueType>
    inline void encodeFrame(util::OctetStream& output, ValueType value)
    {
        encode(output, universalTag<ValueType>());
        encode(output, make_length(encodedLength(value)));
        encode(output, value);
    }

    template<>
    inline void encodeFrame<Value>(util::OctetStream& output, Value value)
    {
        encode(output, value.universalTag());
        encode(output, make_length(value.encodedLength()));
        value.encode(output);
    }
}
}

#endif  // __LIBEMBER_ENCODING_HPP

