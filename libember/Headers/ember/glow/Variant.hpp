/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_VARIANT_HPP
#define __LIBEMBER_GLOW_VARIANT_HPP

#include <sstream>
#include "ParameterType.hpp"
#include "../ber/Octets.hpp"

//SimianIgnore

namespace libember { namespace glow 
{
    /**
     * Helper class that may contain different data types (string, int, double) and 
     * is able to convert them to any other type supported.
     * This class is used by MinMax and Value.
     * @note This class should not be used directly. Use MinMax or Value instead.
     */
    struct Variant
    {
    public:
        /** 
         * Factory method used to create a new instance of a variant.
         * @param value The value to store in the variant class.
         * @return The created Variant instance.
         * @note The Variant class uses reference counting, so release Ref must be called
         *      in order to indicate that the instance when it is no longer needed.
         */
        template<typename ValueType>
        static Variant* create(ValueType const value);

        /** 
         * Returns the internal value as integer. If the internal type is not an
         * integer, the implementation will try to convert it.
         * @return Returns the internal value as integer.
         */
        virtual long toInteger() const = 0;

        /** 
         * Returns the internal value as double. If the internal type is not a
         * double, the implementation will try to convert it.
         * @return Returns the internal value as double.
         */
        virtual double toReal() const = 0;

        /** 
         * Returns the internal value as string. If the internal type is not a
         * string, the implementation will try to convert it.
         * @return Returns the internal value as string.
         */
        virtual std::string toString() const = 0;

        /** 
         * Returns the internal value as octet string. If the internal type is not a
         * string, the implementation will try to convert it.
         * @return Returns the internal value as octet string.
         */
        virtual ber::Octets toOctets() const = 0;

        /** 
         * Returns the internal value as bool. If the internal type is not a
         * bool, the implementation will try to convert it.
         * @return Returns the internal value as bool.
         */
        virtual bool toBoolean() const = 0;

        /**
         * Returns the value type. 
         * @return The value type.
         */
        ParameterType const& type() const;

        /**
         * Increments the internal reference counter by one.
         * @return Returns this instance.
         */
        Variant* addRef();

        /**
         * Decrements the internal reference counter by one. If the counter reaches 0,
         * the instance will delete itself.
         */
        void releaseRef();

    protected:
        /**
         * Destructor
         */
        virtual ~Variant();

        /**
         * Constructor
         * @param type The type this variant currently stores.
         */
        explicit Variant(ParameterType const& type);

    private:
        unsigned long m_refCount;
        ParameterType m_type;
    };


    /******************************************************
     * Inline implementation                              *
     ******************************************************/

    inline Variant::Variant(ParameterType const& type)
        : m_refCount(1)
        , m_type(type)
    {}

    inline Variant::~Variant()
    {}

    inline ParameterType const& Variant::type() const
    {
        return m_type;
    }

    inline Variant* Variant::addRef()
    {
        m_refCount += 1;
        return this;
    }

    inline void Variant::releaseRef()
    {
        m_refCount -= 1;

        if (m_refCount == 0)
        {
            delete this;
        }
    }


    namespace detail 
    {
        /** 
         * VariantTemplate declaration. The template specializations implement the 
         * Variant base class.
         **/
        template<typename ValueType>
        struct VariantImpl;


        /**
         * Variant specialization for the std::string type.
         */
        template<>
        struct VariantImpl<std::string> : Variant
        {
            friend struct Variant;
            typedef std::string value_type;
            public:
                virtual long toInteger() const
                {
                    std::stringstream stream(m_value);
                    int integer = 0;

                    stream >> integer;
                    return integer;
                }

                virtual double toReal() const
                {
                    std::stringstream stream(m_value);
                    double real = 0.0;

                    stream >> real;
                    return real;
                }

                virtual std::string toString() const
                {
                    return m_value;
                }

                virtual ber::Octets toOctets() const
                {
                    return ber::Octets(m_value.begin(), m_value.end());
                }

                virtual bool toBoolean() const
                {
                    return m_value.size() > 0;
                }

            private:
                /**
                 * Constructor, initializes the variant with a string value.
                 * @param value Value to store.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ParameterType::String)
                    , m_value(value)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
        };


        /**
         * Variant specialization for the integer type.
         */
        template<>
        struct VariantImpl<long> : Variant
        {
            friend struct Variant;
            typedef int value_type;
            public:
                virtual long toInteger() const
                {
                    return m_value;
                }

                virtual double toReal() const
                {
                    return m_value;
                }

                virtual std::string toString() const
                {
                    std::stringstream stream;
                    stream << m_value;

                    return stream.str();
                }

                virtual ber::Octets toOctets() const
                {
                    return ber::Octets();
                }

                virtual bool toBoolean() const
                {
                    return m_value != 0;
                }

            private:
                /**
                 * Constructor initializing the variant with an integer.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ParameterType::Integer)
                    , m_value(value)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
        };


        /**
         * Variant specialization for the double type.
         */
        template<>
        struct VariantImpl<double> : Variant
        {
            friend struct Variant;
            typedef double value_type;
            public:
                virtual long toInteger() const
                {
                    return static_cast<long>(m_value);
                }

                virtual double toReal() const
                {
                    return m_value;
                }

                virtual std::string toString() const
                {
                    std::stringstream stream;
                    stream << m_value;

                    return stream.str();
                }

                virtual ber::Octets toOctets() const
                {
                    return ber::Octets();
                }

                virtual bool toBoolean() const
                {
                    return m_value != 0.0;
                }

            private:
                /**
                 * Constructor initializing the variant with the provided double value.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ParameterType::Real)
                    , m_value(value)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
            };


        /**
         * Variant specialization for the boolean type.
         */
        template<>
        struct VariantImpl<bool> : Variant
        {
            friend struct Variant;
            typedef bool value_type;
            public:
                virtual long toInteger() const
                {
                    return m_value ? 1 : 0;
                }

                virtual double toReal() const
                {
                    return m_value ? 1.0 : 0.0;
                }

                virtual std::string toString() const
                {
                    return m_value ? "true" : "false";
                }

                virtual ber::Octets toOctets() const
                {
                    return ber::Octets();
                }

                virtual bool toBoolean() const
                {
                    return m_value;
                }

            private:
                /**
                 * Constructor initializing the variant with the provided boolean value.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ParameterType::Boolean)
                    , m_value(value)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
            };


        /**
         * Variant specialization for the Octets type.
         */
        template<>
        struct VariantImpl<ber::Octets> : Variant
        {
            friend struct Variant;
            typedef ber::Octets value_type;
            public:
                virtual long toInteger() const
                {
                    return 0;
                }

                virtual double toReal() const
                {
                    return 0.0;
                }

                virtual std::string toString() const
                {
                    return std::string();
                }

                virtual ber::Octets toOctets() const
                {
                    return m_value;
                }

                virtual bool toBoolean() const
                {
                    return m_value.size() > 0;
                }

            private:
                /**
                 * Constructor initializing the variant with the provided octet string.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ParameterType::Octets)
                    , m_value(value)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
            };


        /**
         * Variant specialization for the void type, which is used when no value exists.
         */
        template<>
        struct VariantImpl<void*> : Variant
        {
            friend struct Variant;
            typedef void* value_type;
            public:
                virtual long toInteger() const
                {
                    return 0;
                }

                virtual double toReal() const
                {
                    return 0.0;
                }

                virtual std::string toString() const
                {
                    return std::string();
                }

                virtual ber::Octets toOctets() const
                {
                    return ber::Octets();
                }

                virtual bool toBoolean() const
                {
                    return false;
                }

            private:
                /**
                 * Constructor initializing the variant with the provided value.
                 * This void overload swallows all values without storing anything.
                 */
                template<typename T>
                VariantImpl(T const&)
                    : Variant(ParameterType::None)
                {}

                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);
            };
    }
    

    /**
     * Implementation of the Variant factory.
     */
    template<typename ValueType>
    inline Variant* Variant::create(ValueType const value)
    {
        return new detail::VariantImpl<ValueType>(value);
    }
}
}

//EndSimianIgnore

#endif // __LIBEMBER_GLOW_VARIANT_HPP
