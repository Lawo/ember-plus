#ifndef __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
#define __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP

#include <string>
#include "../../ber/Type.hpp"

namespace libember { namespace glow { namespace traits 
{
    /** 
     * Structure used to determine the corresponding ber type of a value type.
     * Currently the supported types are bool, int, double and std::string.
     */
    template<typename ValueType>
    struct ValueTypeToBerType;

    /** 
     * Specialization for the bool type.
     */
    template<>
    struct ValueTypeToBerType<bool>
    {
        static const ber::Type berType() 
        {
            return ber::Type::Boolean;
        }
    };

    /** 
     * Specialization for the integer type.
     */
    template<>
    struct ValueTypeToBerType<int>
    {
        static ber::Type berType()
        {
            return ber::Type::Integer;
        }
    };

    /** 
     * Specialization for the real/double type.
     */
    template<>
    struct ValueTypeToBerType<double>
    {
        static ber::Type berType()
        {
            return ber::Type::Real;
        }
    };

    /** 
     * Specialization for the std::string type.
     */
    template<>
    struct ValueTypeToBerType<std::string>
    {
        static ber::Type berType()
        {
            return ber::Type::UTF8String;
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
