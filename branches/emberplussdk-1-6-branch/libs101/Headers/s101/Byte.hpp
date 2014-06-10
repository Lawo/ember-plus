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

#ifndef __LIBS101_BYTE_HPP
#define __LIBS101_BYTE_HPP

namespace libs101 
{
    /**
     * Struct enumerating the bytes having a special meaning in 
     * the S101 framing.
     */ 
    struct Byte
    {
        enum _Domain
        {
            CE = 0xFD,
            BoF = 0xFE,
            EoF = 0xFF,
            XOR = 0x20,
            Invalid = 0xF8
        };

        typedef unsigned char value_type;
        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            Byte(_Domain value)
                : m_value(static_cast<value_type>(value))
            {}

            /**
             * Returns the value.
             * @return The value.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            /**
             * The byte value.
             */
            value_type m_value;
    };
}

#endif  // __LIBS101_BYTE_HPP

