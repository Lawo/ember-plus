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

