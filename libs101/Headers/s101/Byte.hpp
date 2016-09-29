/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_BYTE_HPP
#define __LIBS101_BYTE_HPP

//SimianIgnore

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
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit Byte(value_type value)
                : m_value(value)
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

//EndSimianIgnore

#endif  // __LIBS101_BYTE_HPP

