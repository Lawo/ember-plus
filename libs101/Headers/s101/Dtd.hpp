/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_DTD_HPP
#define __LIBS101_DTD_HPP

namespace libs101
{
    /**
     * Struct enumerating the DTD types available.
     */ 
    struct Dtd
    {
        enum _Domain
        {
            Glow = 1
        };


        typedef unsigned char value_type;
        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            Dtd(_Domain value)
                : m_value(static_cast<value_type>(value))
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit Dtd(value_type value)
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
            value_type m_value;
    };

    inline bool operator==(Dtd const& left, Dtd const& right)
    {
        return (left.value() == right.value());
    }

    inline bool operator!=(Dtd const& left, Dtd const& right)
    {
        return !(left == right);
    }
}

#endif  // __LIBS101_DTD_HPP

