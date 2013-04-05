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

#ifndef __LIBEMBER_GLOW_VALUE_HPP
#define __LIBEMBER_GLOW_VALUE_HPP

#include "ParameterType.hpp"
#include "Variant.hpp"
#include "../ber/Encoding.hpp"
#include "../ber/Octets.hpp"
#include "../ber/Value.hpp"
#include "util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    /**
     * A class that may store several value types: Integer, Real, Bool, Octets and UTF8String.
     * The value is used by the parameter's value and default property. This class
     * also provides functionality to cast between the different data types.
     */
    class Value
    {
        public:
            /**
             * Default constructor, which initializes the type with ParameterType::None.
             */
            Value();

            /**
             * Initializes a Value instance that contains an integer value.
             * @param value The value to store
             */
            Value(long value);

            /**
             * Initializes a Value instance that contains an real value.
             * @param value The value to store
             */
            Value(double value);

            /**
             * Initializes a Value instance that contains a std::string value.
             * @param value The value to store
             */
            Value(std::string const& value);

            /**
             * Initializes a Value instance that contains a byte array, called Octet String.
             * @param value The value to store
             */
            Value(ber::Octets const& value);

            /**
             * Initializes a Value instance that contains a boolean value
             * @param value The value to store
             */
            Value(bool value);

            /**
             * Copy constructor.
             * @param other Instance to copy the value from.
             */
            Value(Value const& other);

            /**
             * Initializes a new Value instance from a type erased ber::Value.
             * @param The type erased ber value to read the value from.
             */
            Value(ber::Value const& value);

            /**
             * Destructor
             */
            ~Value();

            /**
             * Returns the type of the value stored by this instance.
             * @return The type of the current value
             */
            ParameterType type() const;

            /**
             * Returns the current value as integer. 
             * @note If the data type is not an integer, the implementation will try to convert it.
             * @return The current value as integer
             */
            long toInteger() const;

            /**
             * Returns the current value as double. 
             * @note If the data type is not a double, the implementation will try to convert it.
             * @return The current value as double
             */
            double toReal() const;

            /**
             * Returns the current value as string. 
             * @note If the data type is not a string, the implementation will try to convert it.
             * @return The current value as string
             */
            std::string toString() const;

            /**
             * Returns the current value as Octets.
             * @note If the data type is not Octets, the implementation will try to convert it. If 
             *      the conversion fails, an empty Octet string will be returned.
             * @return The current value as octet string
             */
            ber::Octets toOctets() const;

            /**
             * Returns the current value as boolean value
             * @note If the data type is not a string, the implementation will try to convert it.
             * @return The current value as boolean value
             */
            bool toBoolean() const;

            /**
             * Swaps the value of this instance with the one passed.
             * @param other Instance to exchange the data with.
             */
            void swap(Value& other);

            /**
             * Assignment operator
             * @param other Instance to copy the value from.
             */
            Value& operator=(Value other);

        private:
            Variant* m_value;
    };

    /******************************************************
     * Inline Implementation                              *
     ******************************************************/

    inline Value::Value()
        : m_value(Variant::create<void*>(0))
    {

    }

    inline Value::Value(ber::Value const& value)
        : m_value(0)
    {
        ber::Tag const type = value.universalTag();
        if (type.getClass() == ber::Class::Universal)
        {
            switch(type.number())
            {
                case ber::Type::Integer:
                    m_value = Variant::create(util::ValueConverter::valueOf(value, long(0)));
                    return;
                case ber::Type::Real:
                    m_value = Variant::create(util::ValueConverter::valueOf(value, double(0.0)));
                    return;
                case ber::Type::UTF8String:
                    m_value = Variant::create(util::ValueConverter::valueOf(value, std::string()));
                    return;
                case ber::Type::OctetString:
                    m_value = Variant::create(util::ValueConverter::valueOf(value, ber::Octets()));
                    return;
                case ber::Type::Boolean:
                    m_value = Variant::create(util::ValueConverter::valueOf(value, false));
                    return;
            }
        }

        if (m_value == 0)
            m_value = Variant::create(0L);
    }

    inline Value::Value(double value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(long value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(std::string const& value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(ber::Octets const& value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(bool value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(Value const& other)
        : m_value(other.m_value->addRef())
    {}

    inline Value::~Value()
    {
        m_value->releaseRef();
    }

    inline ParameterType Value::type() const
    {
        return m_value->type();
    }

    inline long Value::toInteger() const
    {
        return m_value->toInteger();
    }

    inline double Value::toReal() const
    {
        return m_value->toReal();
    }

    inline std::string Value::toString() const
    {
        return m_value->toString();
    }

    inline ber::Octets Value::toOctets() const
    {
        return m_value->toOctets();
    }

    inline bool Value::toBoolean() const
    {
        return m_value->toBoolean();
    }

    inline void Value::swap(Value& other)
    {
        std::swap(m_value, other.m_value);
    }

    inline Value& Value::operator=(Value other)
    {
        swap(other);
        return *this;
    }

    inline void swap(Value& lhs, Value& rhs)
    {
        lhs.swap(rhs);
    }
}
}

#endif  // __LIBEMBER_GLOW_VALUE_HPP
