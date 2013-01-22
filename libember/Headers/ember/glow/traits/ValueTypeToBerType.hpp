/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
#define __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP

#include <string>
#include "../../ber/ObjectIdentifier.hpp"
#include "../../ber/Octets.hpp"
#include "../../ber/Type.hpp"

namespace libember { namespace glow { namespace traits 
{
    /** 
     * Structure used to determine the corresponding ber type of a value type.
     * Currently the supported types are bool, int, double, ber::Octets and std::string.
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
}
}
}

#endif  // __LIBEMBER_GLOW_TRAITS_VALUETYPETOBERTYPE_HPP
