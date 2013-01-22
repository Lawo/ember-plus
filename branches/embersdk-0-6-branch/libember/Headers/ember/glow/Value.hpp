#ifndef __LIBEMBER_GLOW_VALUE_HPP
#define __LIBEMBER_GLOW_VALUE_HPP

#include "ember/ber/Type.hpp"
#include "detail/Variant.hpp"

namespace libember { namespace glow {
    /**
     * A class that may store several value types: Integer, Real and UTF8String.
     * The value is used by the parameter's value and default property. This class
     * also provides functionality to cast between the different data types.
     */
    class Value
    {
    public:
        /**
         * Initializes a Value instance that contains an integer value.
         * @param value The value to store
         */
        Value(int value);

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
         * Copy constructor.
         * @param other Instance to copy the value from.
         */
        Value(Value const& other);

        /**
         * Destructor
         */
        ~Value();

        /**
         * Returns the type of the value stored by this instance.
         * @return The type of the current value
         */
        ber::Type type() const;

        /**
         * Returns the current value as integer. 
         * @note If the data type is not an integer, the implementation will try to convert it.
         * @return The current value as integer
         */
        int toInteger() const;

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

    inline Value::Value(double value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(int value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(std::string const& value)
        : m_value(Variant::create(value))
    {}

    inline Value::Value(Value const& other)
        : m_value(other.m_value->addRef())
    {}

    inline Value::~Value()
    {
        m_value->releaseRef();
    }

    inline ber::Type Value::type() const
    {
        return m_value->type();
    }

    inline int Value::toInteger() const
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
