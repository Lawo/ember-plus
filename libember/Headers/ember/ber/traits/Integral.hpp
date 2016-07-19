/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_INTEGRAL_HPP
#define __LIBEMBER_BER_TRAITS_INTEGRAL_HPP

#include "CodecTraits.hpp"
#include "RegisterDecoder.hpp"
#include "../../meta/FunctionTraits.hpp"
#include "../../meta/Signedness.hpp"

namespace libember { namespace ber
{
    namespace detail
    {
        /**
         * Common helper implementation for integer types of all bit-widths.
         * May be used for signed and unsigned types alike.
         */
        struct IntegerUniversalTagTraits
        {
            static Tag universalTag()
            {
                return make_tag(Class::Universal, Type::Integer);
            }
        };

        template<typename IntegralType>
        struct SingleByteEncodedLength
        {
            typedef IntegralType value_type;

            static std::size_t encodedLength(value_type)
            {
                return 1;
            }
        };

        template<typename IntegralType>
        struct UnsignedEncodedLength
        {
            typedef IntegralType value_type;

            static std::size_t encodedLength(value_type value)
            {
                // Create a bit-mask that has the eight most significant bits set
                value_type mask = static_cast<value_type>(value_type(0xFF) << (sizeof(value_type) * 8 - 8));
                std::size_t length = sizeof(IntegralType);

                for (; (value & mask) == 0 && length > 1; mask >>= 8)
                {
                    length -= 1;
                }
                return length;
            }
        };

        template<typename IntegralType>
        struct SignedEncodedLength
        {
            typedef IntegralType value_type;

            static std::size_t encodedLength(value_type value)
            {
                typedef typename meta::MakeUnsigned<value_type>::type unsigned_type;

                bool const isPositive = (value >= 0); 

                // Create a bit-mask that has the nine most significant bits set
                unsigned_type mask = static_cast<unsigned_type>(~((static_cast<unsigned_type>(1U) << (((sizeof(IntegralType) - 1) * 8) - 1)) - 1));
                unsigned_type const unsignedValue = static_cast<unsigned_type>(value);

                std::size_t length = sizeof(value_type);
                while ((length > 1) && ((unsignedValue & mask) == (isPositive ? 0 : mask)))
                {
                    length -= 1;
                    mask >>= 8;
                }
                if (isPositive && ((unsignedValue >> (length * 8 - 1)) != 0))
                {
                    length += 1;
                }
                return length;
            }
        };

        template<typename IntegralType>
        struct IntegerEncode
        {
            typedef IntegralType value_type;

            static void encode(util::OctetStream& output, value_type value)
            {
                std::size_t const length = encodedLength(value);
                std::size_t bits = length * 8;

                while (bits > 0)
                {
                    bits -= 8;
                    output.append(static_cast<util::OctetStream::value_type>((value >> bits) & 0xFF));
                }
            }
        };

        /**
         * Common helper implementation for integer types of all bit-widths.
         * May be used for signed and unsigned types alike.
         */
        template<typename IntegralType>
        struct IntegerDecodingTraits
        {
            typedef IntegralType value_type;
            /**
             * Traits type providing various infos on the decode functions
             * signature.
             * Unfortunately C++03 does not yet support a library independent
             * typeof/decltype operation, which is why we have to
             * 1. explicitly repeat the signature here and
             * 2. cannot defer the introspection work to the freestanding
             *    decode function.
             */
            typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

            static value_type decode(util::OctetStream& input, std::size_t encodedLength)
            {
                typedef typename meta::MakeUnsigned<value_type>::type unsigned_type;
                value_type value = 0;
                for (std::size_t index = 0; index < encodedLength; ++index)
                {
                    util::OctetStream::value_type byte = input.front();
                    if(meta::IsSigned<value_type>())
                    {
                        if ((index == 0) && (static_cast<unsigned_type>(byte) & 0x80U))
                        {
                            value = static_cast<value_type>(-1);
                        }
                    }
                    value = static_cast<value_type>((value << 8) | byte);
                    input.consume();
                }

                return value;
            }
        };
    }

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<char>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<char>
        : detail::IntegerEncode<char>, detail::SingleByteEncodedLength<char>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<char>
        : detail::IntegerDecodingTraits<char>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<unsigned char>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<unsigned char>
        : detail::IntegerEncode<unsigned char>, detail::SingleByteEncodedLength<unsigned char>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<unsigned char>
        : detail::IntegerDecodingTraits<unsigned char>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<short>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<short>
        : detail::IntegerEncode<short>, detail::SignedEncodedLength<short>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<short>
        : detail::IntegerDecodingTraits<short>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<unsigned short>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<unsigned short>
        : detail::IntegerEncode<unsigned short>, detail::UnsignedEncodedLength<unsigned short>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<unsigned short>
        : detail::IntegerDecodingTraits<unsigned short>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<int>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<int>
        : detail::IntegerEncode<int>, detail::SignedEncodedLength<int>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<int>
        : detail::IntegerDecodingTraits<int>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<unsigned int>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<unsigned int>
        : detail::IntegerEncode<unsigned int>, detail::UnsignedEncodedLength<unsigned int>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<unsigned int>
        : detail::IntegerDecodingTraits<unsigned int>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<long>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<long>
        : detail::IntegerEncode<long>, detail::SignedEncodedLength<long>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<long>
        : detail::IntegerDecodingTraits<long>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<unsigned long>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<unsigned long>
        : detail::IntegerEncode<unsigned long>, detail::UnsignedEncodedLength<unsigned long>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<unsigned long>
        : detail::IntegerDecodingTraits<unsigned long>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<long long>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<long long>
        : detail::IntegerEncode<long long>, detail::SignedEncodedLength<long long>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<long long>
        : detail::IntegerDecodingTraits<long long>
    {};
    

    /** UniversalTagTraits specialization for the signed char type. */
    template<>
    struct UniversalTagTraits<unsigned long long>
        : detail::IntegerUniversalTagTraits
    {};

    /** EncodingTraits specialization for the signed char type. */
    template<>
    struct EncodingTraits<unsigned long long>
        : detail::IntegerEncode<unsigned long long>, detail::UnsignedEncodedLength<unsigned long long>
    {};

    /** DecodingTraits specialization for the signed char type. */
    template<>
    struct DecodingTraits<unsigned long long>
        : detail::IntegerDecodingTraits<unsigned long long>
    {};
    

    /**
     * Helper template instances that makes the decode function
     * for the various intrgral types dynamically available.
     */
    RegisterDecoder<         char     > const _charDecoderRegistrar;
    RegisterDecoder<unsigned char     > const _unsignedCharDecoderRegistrar;
    RegisterDecoder<         short    > const _shortDecoderRegistrar;
    RegisterDecoder<unsigned short    > const _unsignedShortDecoderRegistrar;
    RegisterDecoder<         int      > const _intDecoderRegistrar;
    RegisterDecoder<unsigned int      > const _unsignedIntDecoderRegistrar;
    RegisterDecoder<         long     > const _longDecoderRegistrar;
    RegisterDecoder<unsigned long     > const _unsignedLongDecoderRegistrar;
    RegisterDecoder<         long long> const _longLongDecoderRegistrar;
    RegisterDecoder<unsigned long long> const _unsignedLongLongDecoderRegistrar;
}
}

#endif // __LIBEMBER_BER_TRAITS_INTEGRAL_HPP

