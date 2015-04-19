#ifndef __LIBFORMULA_TRAITS_STRINGSTREAMCONVERTER_HPP
#define __LIBFORMULA_TRAITS_STRINGSTREAMCONVERTER_HPP

#include <memory>
#include <vector>
#include <cstdlib>
#include "../Types.hpp"

#pragma warning( push )
#pragma warning( disable : 4996 )
#define _SCL_SECURE_NO_WARNINGS

namespace libformula { namespace traits
{
    namespace detail
    {
        /**
         * Converts a string into a long type value.
         */
        template<typename ValueType>
        struct StringToIntegral
        {
            typedef ValueType value_type;
            template<typename InputIterator>
            static value_type parse(InputIterator first, InputIterator last)
            {
                if (first != last)
                {
                    auto const sign = *first == '-' ? -1 : +1;
                    auto result = 0;
                    if (*first == '-' || *first == '+')
                        ++first;

                    for(/* Nothing */; first != last; ++first)
                    {
                        result = result * 10 + (*first - '0');
                    }
                    return value_type(result * sign);
                }
                else
                {
                    return value_type(0);
                }
            };
        };

        /**
         * Converts a string into a real value.
         * This implementaton doesn't allocate any memory, except when the string length
         * exceeds 32 characters. Otherwise, a small array is allocated on the stack.
         */
        template<typename ValueType>
        struct StringToDecimal
        {
            enum { BufferSize = 32, }; 

            typedef ValueType value_type;

            template<typename InputIterator>
            static value_type parse(InputIterator first, InputIterator last)
            {
                typedef typename std::iterator_traits<InputIterator>::value_type type;

                auto const distance = std::distance(first, last);
                if (distance < BufferSize)
                {
                    type buffer[BufferSize];
                    auto it = std::copy(first, last, buffer);
                    *it = type(0);
                    return static_cast<value_type>(std::strtod(buffer, nullptr));
                }
                else
                {
                    auto buffer = std::vector<type>(distance + 1, type(0));
                    auto it = std::copy(first, last, buffer.begin());
                    *it = type(0);
                    auto const result = std::strtod(buffer.data(), nullptr);
                    return static_cast<value_type>(result);
                }
            };
        };
    }

    /**
     * Templated struct to convert a substring into a long or a double value.
     */
    template<typename ValueType>
    struct StringStreamConverter
    {
        /**
         * Parses the input string and converts it into the provided value type,
         * which may either be long or double.
         * @note For value types that are not supported, 0 is being returned.
         * @param first Points to the first element of the string buffer.
         * @param last Points to the first element in the string buffer that 
         *      should not be converted.
         */
        typedef ValueType value_type;
        template<typename InputIterator>
        static value_type parse(InputIterator first, InputIterator last)
        {
            return value_type(0);
        }
    };

    /**
     * Converts a substring into a long value.
     */
    template<>
    struct StringStreamConverter<long> : detail::StringToIntegral<long>
    {};

    /**
     * Converts a substring into an integer value.
     */
    template<>
    struct StringStreamConverter<int> : detail::StringToIntegral<int>
    {};

    /**
     * Converts a substring into a short value.
     */
    template<>
    struct StringStreamConverter<short> : detail::StringToIntegral<short>
    {};

    /**
     * Converts a substring into a char value.
     */
    template<>
    struct StringStreamConverter<char> : detail::StringToIntegral<char>
    {};

    /**
     * Converts a substring into a double value.
     */
    template<>
    struct StringStreamConverter<double> : detail::StringToDecimal<double>
    {};

    /**
     * Converts a substring into a floating point value.
     */
    template<>
    struct StringStreamConverter<float> : detail::StringToDecimal<float>
    {};
}
}

#pragma warning( pop )
#undef _SCL_SECURE_NO_WARNINGS

#endif  // __LIBFORMULA_TRAITS_STRINGSTREAMCONVERTER_HPP
