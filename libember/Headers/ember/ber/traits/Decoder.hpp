/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_DECODER_HPP
#define __LIBEMBER_BER_TRAITS_DECODER_HPP

#include "CodecTraits.hpp"
#include "../Value.hpp"

namespace libember { namespace ber
{
    /**
     * Base class for all decoders.
     * Provides a virtual method to decode a concrete, typed value from a BER representation to
     * a type-erased Value instance.
     */
    struct Decoder
    {
        virtual Value decode(util::OctetStream& input, std::size_t encodedLength) const = 0;
        virtual ~Decoder();
    };

    /**
     * Specialization of Decoder.
     * Provides a virtual method to decode a concrete, typed value from a BER
     * representation to a type-erased Value instance.
     * @note Please note that this functionality could have also been included
     *      in the EncodingTraits template, but since this would couple the more
     *      generic static layer with the dynamic layer this functionality has
     *      been swapped out to a separate template.
     */
    template<typename ValueType>
    struct DecoderImpl : Decoder
    {
        typedef DecodingTraits<ValueType> DecodingTraitsType;

        /** @see Decoder::decode() */
        virtual Value decode(util::OctetStream& input, std::size_t encodedLength) const;
    };



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    inline Decoder::~Decoder()
    {}

    template<typename ValueType>
    inline Value DecoderImpl<ValueType>::decode(util::OctetStream& input, std::size_t encodedLength) const
    {
        ValueType const decodedValue = DecodingTraitsType::decode(input, encodedLength);
        return decodedValue;
    }

}
}

#endif  // __LIBEMBER_BER_TRAITS_DECODER_HPP

