/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_OCTETS_HPP
#define __LIBEMBER_BER_TRAITS_OCTETS_HPP

#include "CodecTraits.hpp"
#include "RegisterDecoder.hpp"
#include "../Octets.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{

    /**
     * UniversalTagTraits specialization for the Octects type.
     */
    template<>
    struct UniversalTagTraits<Octets>
    {
        typedef Octets value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::OctetString);
        }
    };

    /**
     * EncodingTraits specialization for the Octects type.
     */
    template<>
    struct EncodingTraits<Octets>
    {
        typedef Octets value_type;

        static std::size_t encodedLength(value_type const& value)
        {
            return value.size();
        }

        static void encode(util::OctetStream& output, value_type const& value)
        {
            output.append(value.begin(), value.end());
        }
    };

    /**
     * DecodingTraits specialization for the std::string type.
     */
    template<>
    struct DecodingTraits<Octets>
    {
        typedef Octets value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream& input, std::size_t encodedLength)
        {
            util::OctetStream::iterator const begin = input.begin();
            util::OctetStream::iterator const end = input.end();
            util::OctetStream::iterator copyEnd = begin;

            value_type::size_type reservedSize = 0;
            for (reservedSize = 0; (reservedSize < encodedLength) && (copyEnd != end); ++reservedSize)
            {
                ++copyEnd;
            }

            Octets const octets(begin, copyEnd);
            input.consume(copyEnd);
            return octets;
        }
    };

    /**
     * Helper template instance that makes the Octets decode function
     * dynamically available.
     */
    RegisterDecoder<Octets> const _octetsDecoderRegistrar;
}
}

#endif // __LIBEMBER_BER_TRAITS_OCTETS_HPP
