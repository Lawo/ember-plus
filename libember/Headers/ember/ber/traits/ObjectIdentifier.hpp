/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

#include <string>
#include <vector>
#include "CodecTraits.hpp"
#include "../ObjectIdentifier.hpp"
#include "../detail/MultiByte.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /**
     * UniversalTagTraits specialization for object identifiers.
     */
    template<>
    struct UniversalTagTraits<ObjectIdentifier>
    {
        typedef std::string value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::RelativeObject);
        }
    };


    /** EncodingTraits specialization for object identifiers. */
    template<>
    struct EncodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;

        static std::size_t encodedLength(value_type const& value)
        {
            std::size_t length = 0;
            value_type::const_iterator first = value.begin();
            value_type::const_iterator const last = value.end();
            
            for (/* Nothing */; first != last; ++first)
            {
                length += detail::getMultiByteEncodedLength(*first);
            }

            return length;
        }

        static void encode(util::OctetStream& output, value_type const& value)
        {
            value_type::const_iterator first = value.begin();
            value_type::const_iterator const last = value.end();

            for (/* Nothing */; first != last; ++first)
            {
                detail::encodeMultibyte(output, *first);
            }
        }
    };

    /** DecodingTraits specialization for object identifiers. */
    template<>
    struct DecodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream& input, std::size_t size)
        {
            typedef ObjectIdentifier::value_type item_type;
            std::vector<item_type> items;
            while(size > 0)
            {
                std::pair<unsigned long long, std::size_t> encodeResult = detail::decodeMultibyte(input);
                items.push_back(static_cast<item_type>(encodeResult.first));
                size -= encodeResult.second;
            }

            return ObjectIdentifier(items.begin(), items.end());
        }
    };
}
}

#endif // __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

