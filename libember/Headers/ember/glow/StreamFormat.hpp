/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_STREAMFORMAT_HPP
#define __LIBEMBER_GLOW_STREAMFORMAT_HPP

//SimianIgnore

namespace libember { namespace glow 
{
    /**
     * Structure listing the available stream formats.
     */
    struct StreamFormat
    {
        public:
            enum _Domain
            {
                UnsignedInt8 = 0,                   /* 00000 00 0 */
                UnsignedInt16BigEndian = 2,         /* 00000 01 0 */
                UnsignedInt16LittleEndian = 3,      /* 00000 01 1 */
                UnsignedInt32BigEndian = 4,         /* 00000 10 0 */
                UnsignedInt32LittleEndian = 5,      /* 00000 10 1 */
                UnsignedInt64BigEndian = 6,         /* 00000 11 0 */
                UnsignedInt64LittleEndian = 7,      /* 00000 11 1 */

                SignedInt8 = 8,                     /* 00001 00 0 */
                SignedInt16BigEndian = 10,          /* 00001 01 0 */
                SignedInt16LittleEndian = 11,       /* 00001 01 1 */
                SignedInt32BigEndian = 12,          /* 00001 10 0 */
                SignedInt32LittleEndian = 13,       /* 00001 10 1 */
                SignedInt64BigEndian = 14,          /* 00001 11 0 */
                SignedInt64LittleEndian = 15,       /* 00001 11 1 */

                IeeeFloat32BigEndian = 20,          /* 00010 10 0 */
                IeeeFloat32LittleEndian = 21,       /* 00010 10 1 */
                IeeeFloat64BigEndian = 22,          /* 00010 11 0 */
                IeeeFloat64LittleEndian = 23        /* 00010 11 1 */
            };

            typedef std::size_t value_type;

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            StreamFormat(_Domain value)
                : m_value(value)
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit StreamFormat(value_type value)
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
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_GLOW_STREAMFORMAT_HPP

