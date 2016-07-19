/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

