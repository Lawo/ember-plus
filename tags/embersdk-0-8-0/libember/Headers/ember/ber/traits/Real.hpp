#ifndef __LIBEMBER_BER_TRAITS_REAL_HPP
#define __LIBEMBER_BER_TRAITS_REAL_HPP

#include <limits>
#include "CodecTraits.hpp"
#include "RegisterDecoder.hpp"
#include "Integral.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    namespace detail
    {
        /**
         * Common helper implementation for floating point types of all
         * bit-widths.
         */
        struct RealUniversalTagTraits
        {
            static Tag universalTag()
            {
                return make_tag(Class::Universal, Type::Real);
            }
        };

        /**
         * Common helper implementation for floating point types of all
         * bit-widths.
         */
        template<typename RealType>
        struct RealEncodingTraits
        {
            typedef RealType value_type;

            /**
             * Normalize the passed mantissa as required by CER and DER
             * and return it.
             * @param mantissa the unnormalized mantissa
             * @return The normalized mantissa.
             */
            static long long normalizeMantissa(long long mantissa)
            {
                while ((mantissa & 0xFF) == 0x00)
                {
                    mantissa >>= 8;
                }
                while ((mantissa & 0x01) == 0x00)
                {
                    mantissa >>= 1;
                }
                return mantissa;
            }

            static std::size_t encodedLength(value_type value)
            {
                if ((value == +std::numeric_limits<value_type>::infinity()) ||
                    (value == -std::numeric_limits<value_type>::infinity()))
                {
                    return 1;
                }
                else
                {
                    double const real = value;
                    unsigned long long const quadWord = *reinterpret_cast<unsigned long long const*>(&real);
                    if (quadWord != 0)
                    {
                        long long const exponent = ((0x7FF0000000000000ULL & quadWord) >> 52) - 1023;
                        long long const mantissa =  (0x000FFFFFFFFFFFFFULL & quadWord) | 0x0010000000000000ULL;
                        return 1 + ber::encodedLength(exponent) + ber::encodedLength(normalizeMantissa(mantissa));
                    }
                }
                return 0;
            }

            static void encode(util::OctetStream& output, value_type value)
            {
                if (value == +std::numeric_limits<value_type>::infinity())
                {
                    // 0x40 Indicates positive infinity
                    output.append(0x40);            
                }
                else if (value == -std::numeric_limits<value_type>::infinity())
                {
                    // 0x41 Indicates positive infinity
                    output.append(0x41);            
                }
                else
                {
                    double const real = value;
                    unsigned long long const quadWord = *reinterpret_cast<const unsigned long long *>(&real);

                    if (quadWord != 0)
                    {
                        long long const exponent = ((0x7FF0000000000000ULL & quadWord) >> 52) - 1023;
                        long long const mantissa =  (0x000FFFFFFFFFFFFFULL & quadWord) | 0x0010000000000000ULL;

                        std::size_t const exponentLength = ber::encodedLength(exponent);
                        unsigned char const preamble = static_cast<unsigned char>(0x80 | (exponentLength - 1) | (((quadWord & 0x8000000000000000ULL) != 0) ? 0x40 : 0x00));
                        
                        output.append(preamble);
                        ber::encode(output, exponent);
                        ber::encode(output, normalizeMantissa(mantissa));
                    }
                }
            }
        };


        /**
         * Common helper implementation for floating point types of all
         * bit-widths.
         */
        template<typename RealType>
        struct RealDecodingTraits
        {
            typedef RealType value_type;
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

            static long long denormalizeMantissa(long long mantissa)
            {
                while ((mantissa & 0x000FF00000000000LL) == 0)
                {
                    mantissa <<= 8;
                }
                while ((mantissa & 0x0010000000000000LL) == 0)
                {
                    mantissa <<= 1;
                }
                mantissa &= 0x0FFFFFFFFFFFFFULL;
                return mantissa;
            }

            static value_type decode(util::OctetStream& input, std::size_t encodedLength)
            {
                if (encodedLength == 0)
                {
                    return static_cast<value_type>(0.0);
                }

                util::OctetStream::value_type const preamble = input.front();
                input.consume();

                // + Infinity
                if (encodedLength == 1)
                {
                    if (preamble == 0x40)
                    {
                        return +std::numeric_limits<value_type>::infinity();
                    }
                    if (preamble == 0x41)
                    {
                        return -std::numeric_limits<value_type>::infinity();
                    }
                }

                std::size_t const exponentLength = 1 + (preamble & 3);
                util::OctetStream::value_type const sign = preamble & 0x40;
                util::OctetStream::value_type const ff = (preamble >> 2) & 3;

                long long const exponent = ber::decode<long long>(input, exponentLength);
                long long const mantissa = denormalizeMantissa(ber::decode<long long>(input, encodedLength - exponentLength - 1) << ff);

                unsigned long long const quadWord = static_cast<unsigned long long>(((exponent + 1023) << 52) | mantissa) | ((sign != 0) ? 0x8000000000000000ULL : 0ULL);
                return static_cast<value_type>(*reinterpret_cast<double const*>(&quadWord));
            }
        };
    }

    /** UniversalTagTraits specialization for the float type. */
    template<>
    struct UniversalTagTraits<float>
        : detail::RealUniversalTagTraits
    {};

    /** EncodingTraits specialization for the float type. */
    template<>
    struct EncodingTraits<float>
        : detail::RealEncodingTraits<float>
    {};

    /** DecodingTraits specialization for the float type. */
    template<>
    struct DecodingTraits<float>
        : detail::RealDecodingTraits<float>
    {};

    /** UniversalTagTraits specialization for the double type. */
    template<>
    struct UniversalTagTraits<double>
        : detail::RealUniversalTagTraits
    {};

    /** EncodingTraits specialization for the double type. */
    template<>
    struct EncodingTraits<double>
        : detail::RealEncodingTraits<double>
    {};

    /** DecodingTraits specialization for the double type. */
    template<>
    struct DecodingTraits<double>
        : detail::RealDecodingTraits<double>
    {};

    /**
     * Helper template instances that makes the decode function
     * for the various floating point types dynamically available.
     */
    RegisterDecoder<float > const _floatDecoderRegistrar;
    RegisterDecoder<double> const _doubleDecoderRegistrar;
}
}

#endif  // __LIBEMBER_BER_TRAITS_REAL_HPP

