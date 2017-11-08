/*
libember -- C++ 03 implementation of the Ember+ Protocol

Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_NULL_HPP
#define __LIBEMBER_BER_TRAITS_NULL_HPP

#include "CodecTraits.hpp"
#include "RegisterDecoder.hpp"
#include "../Null.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /**
    * UniversalTagTraits specialization for the null type.
    */
    template<>
    struct UniversalTagTraits<Null>
    {
        typedef Null value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::Null);
        }
    };

    /**
    * EncodingTraits specialization for the null type.
    */
    template<>
    struct EncodingTraits<Null>
    {
        typedef Null value_type;

        static std::size_t encodedLength(value_type)
        {
            return 0U;
        }

        static void encode(util::OctetStream&, value_type)
        {

        }
    };

    /**
    * DecodingTraits specialization for the null type.
    */
    template<>
    struct DecodingTraits<Null>
    {
        typedef Null value_type;

        /**
        * Traits type providing various infos on the decode functions signature.
        * Unfortunately C++03 does not yet support a library independent
        * typeof/decltype operation, which is why we have to
        * 1. explicitly repeat the signature here and
        * 2. cannot defer the introspection work to the freestanding decode
        *    function.
        */
        typedef meta::FunctionTraits<value_type(*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream&, std::size_t)
        {
            return value_type();
        }
    };

    /**
    * Helper template instance that makes the null decode function
    * dynamically available.
    */
    RegisterDecoder<Null> const _nullDecoderRegistrar;
}
}

#endif//__LIBEMBER_BER_TRAITS_NULL_HPP
