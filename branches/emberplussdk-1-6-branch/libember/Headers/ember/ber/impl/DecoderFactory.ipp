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

#ifndef __LIBEMBER_BER_IMPL_DECODERFACTORY_IPP
#define __LIBEMBER_BER_IMPL_DECODERFACTORY_IPP

#include <stdexcept>
#include "../../util/Inline.hpp"
#include "../Length.hpp"
#include "../Encoding.hpp"

namespace libember { namespace ber
{
    LIBEMBER_INLINE
    Value DecoderFactory::decode(util::OctetStream& input) const
    {
        typedef Length<unsigned long> LengthType;

        Tag const universalTag = ber::decode<Tag>(input);
        if (universalTag.getClass() != Class::Universal)
        {
            throw std::runtime_error("Expected a universal tag. But found a different tag instead."); 
        }
       
        DecoderMap::const_iterator const it = m_decoderMap.find(universalTag);
        if (it == m_decoderMap.end())
        {
            throw std::runtime_error("Encountered universal tag for which no suitable decoder is available."); 
        }

        LengthType const length = ber::decode<LengthType>(input);
        return it->second->decode(input, length.value);
    }

    LIBEMBER_INLINE
    DecoderFactory::DecoderFactory()
        : m_decoderMap()
    {}

    LIBEMBER_INLINE
    void DecoderFactory::registerDecoder(Tag universalTag, Decoder* decoder)
    {
        m_decoderMap.insert(std::make_pair(universalTag, decoder));
    }


    LIBEMBER_API
    LIBEMBER_INLINE
    DecoderFactory& decoderFactory()
    {
        static DecoderFactory theDecoderFactory;
        return theDecoderFactory;
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    Value decode(util::OctetStream& input)
    {
        return Value(decoderFactory().decode(input));
    }
}
}

#endif  // __LIBEMBER_BER_IMPL_DECODERFACTORY_IPP

