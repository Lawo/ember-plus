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

#ifndef __LIBEMBER_BER_CLASS_HPP
#define __LIBEMBER_BER_CLASS_HPP

namespace libember { namespace ber
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the various tag classes.
     */
    class Class
    {
        public:
            typedef unsigned char value_type;

            enum _Domain
            {
                Universal           = 0x00,         //  Predefined Types for BER ( do not use! )
                Application         = 0x40,         //  Application specific Tags that have the same meaning wherever seen and used.
                ContextSpecific     = 0x80,         //  Tags that have a meaning specific to the location where seen and used.
                Private             = 0xC0,         //  Special version of Context Specific Tag. 

                Mask                = 0xC0
            };

        public:
            /**
             * Non-explicit single argument constructor to allow implicit conversion.
             * @param value the value from _Domain with which to initialize this
             *      instance.
             */
            Class(_Domain value)
                : m_value(static_cast<value_type>(value))
            {}

            /**
             * Return the raw value currently represented by this instance.
             * @return The raw value currently represented by this instance.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };

    inline bool operator<(Class const& left, Class const& right)
    {
        return (left.value() < right.value());
    }

    inline bool operator==(Class const& left, Class const& right)
    {
        return (left.value() == right.value());
    }

    inline bool operator!=(Class const& left, Class const& right)
    {
        return !(left == right);
    }

    inline bool operator<=(Class const& left, Class const& right)
    {
        return !(right < left);
    }

    inline bool operator>(Class const& left, Class const& right)
    {
        return (right < left);
    }

    inline bool operator>=(Class const& left, Class const& right)
    {
        return !(left < right);
    }
}
}

#endif  // __LIBEMBER_BER_CLASS_HPP


