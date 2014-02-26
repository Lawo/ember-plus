/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBS101_UTIL_ENUMCAST_HPP
#define __LIBS101_UTIL_ENUMCAST_HPP

namespace libs101 { namespace util
{
    template<typename EnumType>
    inline EnumType enum_cast(typename EnumType::value_type value)
    {
        return static_cast<EnumType>(value);
    }
}
}

#endif  // __LIBS101_UTIL_ENUMCAST_HPP

