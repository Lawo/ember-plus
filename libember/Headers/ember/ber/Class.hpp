/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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


