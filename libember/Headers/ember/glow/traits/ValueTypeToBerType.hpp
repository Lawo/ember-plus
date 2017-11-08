/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
#define __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP

#include <string>
#include "../../ber/ObjectIdentifier.hpp"
#include "../../ber/Octets.hpp"
#include "../../ber/Null.hpp"
#include "../../ber/Type.hpp"

namespace libember { namespace glow { namespace traits 
{
    /** 
     * Structure used to determine the corresponding ber type of a value type.
     * Currently the supported types are bool, int, double, ber::Octets, ber::ObjectIdentifier and std::string.
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
     * Specialization for the long type.
     */
    template<>
    struct ValueTypeToBerType<long>
    {
        static ber::Type berType()
        {
            return ber::Type::Integer;
        }
    };

    /** 
     * Specialization for the float type.
     */
    template<>
    struct ValueTypeToBerType<float>
    {
        static ber::Type berType()
        {
            return ber::Type::Real;
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

    /**
     * Specialization for the ber::Octets type
     */
    template<>
    struct ValueTypeToBerType<ber::Octets>
    {
        static ber::Type berType()
        {
            return ber::Type::OctetString;
        }
    };

    /**
     * Specialization for the ber::ObjectIdentifier type
     */
    template<>
    struct ValueTypeToBerType<ber::ObjectIdentifier>
    {
        static ber::Type berType()
        {
            return ber::Type::RelativeObject;
        }
    };

    /**
    * Specialization for the ber::Null type
    */
    template<>
    struct ValueTypeToBerType<ber::Null>
    {
        static ber::Type berType()
        {
            return ber::Type::Null;
        }
    };
}
}
}

#endif  // __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
