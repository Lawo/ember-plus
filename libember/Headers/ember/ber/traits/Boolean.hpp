/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_BOOLEAN_HPP
#define __LIBEMBER_BER_TRAITS_BOOLEAN_HPP

#include "CodecTraits.hpp"
#include "RegisterDecoder.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{

    /**
     * UniversalTagTraits specialization for the boolean type.
     */
    template<>
    struct UniversalTagTraits<bool>
    {
        typedef bool value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::Boolean);
        }
    };

    /**
     * EncodingTraits specialization for the boolean type.
     */
    template<>
    struct EncodingTraits<bool>
    {
        typedef bool value_type;

        static std::size_t encodedLength(value_type)
        {
            return 1U;
        }

        static void encode(util::OctetStream& output, value_type value)
        {
            output.append(value ? 0xFF : 0x00);
        }
    };

    /**
     * DecodingTraits specialization for the boolean type.
     */
    template<>
    struct DecodingTraits<bool>
    {
        typedef bool value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream& input, std::size_t)
        {
            util::OctetStream::value_type const byte = input.front();
            input.consume();
            return (byte != 0);
        }
    };

    /**
     * Helper template instance that makes the boolean decode function
     * dynamically available.
     */
    RegisterDecoder<bool> const _booleanDecoderRegistrar;
}
}

#endif // __LIBEMBER_BER_TRAITS_BOOLEAN_HPP

