/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
     * 
     * To represent a null-value, simply use the default constructor. The type is then set
     * to ParameterType::None.
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
             * @param value The value to store.
             */
            Value(long value);

            /**
             * Initializes a Value instance that contains an real value.
             * @param value The value to store.
             */
            Value(double value);

            /**
             * Initializes a Value instance that contains a std::string value.
             * @param value The value to store/
             */
            Value(std::string const& value);

            /**
             * Initializes a Value instance that contains a std::string value. The passed
             * character pointer will be transformed into an std::string.
             * @param value The value to store.
             */
            Value(char const* value);

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
             * @param value the type erased ber value to read the value from.
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
             * Convenience method returning true if the underlying value is of type ber::Null.
             * In that case, type() returns ParameterType::None.
             * @return true, if the underlying type is ber::Null and. Otherwise, false.
             */
            bool isNull() const;

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
             * Returns the current value as a variant libember::ber::Value.
             * @return The current value as a variant libember::ber::Value.
             */
            ber::Value toBerValue() const;

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

        //SimianIgnore

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
                case ber::Type::Null:
                    m_value = Variant::create<void*>(0);
                    return;
            }
        }

        if (m_value == 0)
            m_value = Variant::create(0L);

        //EndSimianIgnore
    }

    inline bool Value::isNull() const
    {
        return type().value() == ParameterType::None;
    }

    inline ber::Value Value::toBerValue() const
    {
        switch(type().value())
        {
            case ParameterType::Integer:
                return ber::Value(toInteger());

            case ParameterType::Real:
                return ber::Value(toReal());

            case ParameterType::String:
                return ber::Value(toString());

            case ParameterType::Octets:
                return ber::Value(toOctets());

            case ParameterType::Boolean:
                return ber::Value(toBoolean());

            case ParameterType::None:
                return ber::Value(ber::Null());
        }

        return ber::Value();
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

    inline Value::Value(char const* value)
        : m_value(Variant::create(std::string(value)))
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
