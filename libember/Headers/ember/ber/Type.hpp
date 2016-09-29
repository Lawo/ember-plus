/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TYPE_HPP
#define __LIBEMBER_BER_TYPE_HPP

#include "Tag.hpp"

namespace libember { namespace ber
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the various universal type tag numbers.
     */
    class Type
    {
        public:
            typedef unsigned int value_type;

            enum _Domain
            {
                None               = 0,

                Boolean            = 1,
                Integer            = 2,
                Bitstring          = 3,
                OctetString        = 4,
                Null               = 5,
                ObjectIdentifier   = 6,
                ObjectDescriptor   = 7,
                External           = 8,
                Real               = 9,
                Enumerated         = 10,
                EmbeddedPdv        = 11,
                UTF8String         = 12,
                RelativeObject     = 13,

                Sequence           = 16,
                Set                = 17,
                NumericString      = 18,
                PrintableString    = 19,
                TeletexString      = 20,
                VideotexString     = 21,
                IA5String          = 22,
                UTCTime            = 23,
                GeneralizedTime    = 24,
                GraphicString      = 25,
                VisibleString      = 26,
                GeneralString      = 27,
                UniversalString    = 28,
                UnspecifiedString  = 29,
                BMPString          = 30,

                LastUniversal      = 31
            };

        public:
            /**
             * Creates a new type instance which is being constructed from a
             * type tag. If the class type is not set to Universal, the type
             * will be marked as application specific.
             * @param typeTag The type tag to create the Type instance from.
             * @return The type created from the provided tag.
             */
            static Type fromTag(Tag const& typeTag)
            {
                return Type(typeTag.number(), typeTag.getClass() != ber::Class::Universal);
            }

            /**
             * Single argument constructor that initializes the instance with
             * the symbolic enumeration value passed in @p value.
             * @param value the symbolic enumeration value this instance should
             *      be initialized with.
             * @note Please note that this constructor is intentionally not
             *      marked explicit.
             */
            Type(_Domain value)
                : m_value(value)
            {}

            /**
             * Return the raw value currently represented by this instance.
             * @return The raw value currently represented by this instance.
             */
            value_type value() const
            {
                return m_value & ~ApplicationDefined;
            }

            /**
             * Returns true if this is an application defined type.
             * @return Returns true if this is an application defined type.
             */
            bool isApplicationDefined() const
            {
                return (m_value & ApplicationDefined) != 0;
            }

        private:
            /**
             * Constructor for application types that are inferred from a type tag.
             * @param value tag number indicating the object type.
             * @param isApplicationDefined Flag indicating wether this is an application defined type or not.
             */
            Type(value_type value, bool isApplicationDefined)
                : m_value(value | (isApplicationDefined ? static_cast<value_type>(ApplicationDefined) : 0U))
            {}

            enum 
            {
                ApplicationDefined = 0x80000000
            };

            value_type m_value;
    };

    inline bool operator<(Type const& left, Type const& right)
    {
        return (left.value() < right.value());
    }

    inline bool operator==(Type const& left, Type const& right)
    {
        return (left.value() == right.value());
    }

    inline bool operator!=(Type const& left, Type const& right)
    {
        return !(left == right);
    }

    inline bool operator<=(Type const& left, Type const& right)
    {
        return !(right < left);
    }

    inline bool operator>(Type const& left, Type const& right)
    {
        return (right < left);
    }

    inline bool operator>=(Type const& left, Type const& right)
    {
        return !(left < right);
    }

}
}

#endif  // __LIBEMBER_BER_TYPE_HPP

