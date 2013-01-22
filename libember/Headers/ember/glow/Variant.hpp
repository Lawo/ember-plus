#ifndef __LIBEMBER_GLOW_VARIANT_HPP
#define __LIBEMBER_GLOW_VARIANT_HPP

#include <sstream>
#include "../ber/Type.hpp"

namespace libember { namespace glow 
{
    /**
     * Helper class that may contain different data types (string, int, double) and 
     * is able to convert them to any other type supported.
     * This class is used by MinMax and Value.
     * @note This should not be used directly. Use MinMax or Value instead.
     */
    struct Variant
    {
    public:
        /** 
         * Factory method used to create a new instance of a variant.
         * @param value The value to store in the variant class.
         * @return Returns the created Variant instance.
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
        virtual int toInteger() const = 0;

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
         * Returns the value type. The supported types are Integer, Real and UTF8String.
         * @return The value type.
         */
        ber::Type const& type() const;

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
        explicit Variant(ber::Type const& type);

    private:
        unsigned long m_refCount;
        ber::Type m_type;
    };


    /******************************************************
     * Inline implementation                              *
     ******************************************************/

    inline Variant::Variant(ber::Type const& type)
        : m_refCount(1)
        , m_type(type)
    {}

    inline Variant::~Variant()
    {}

    inline ber::Type const& Variant::type() const
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
            friend struct libember::glow::Variant;

            typedef std::string value_type;
            public:
                /**
                 * Tries to convert the string into an integer.
                 */
                int toInteger() const
                {
                    std::stringstream stream(m_value);
                    int integer = 0;

                    stream >> integer;
                    return integer;
                }

                /**
                 * Tries to convet the string into a double.
                 */
                double toReal() const
                {
                    std::stringstream stream(m_value);
                    double real = 0.0;

                    stream >> real;
                    return real;
                }

                /**
                 * Returns the string
                 */
                std::string toString() const
                {
                    return m_value;
                }

            private:
                /**
                 * Constructor, initializes the variant with a string value.
                 * @param value Value to store.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ber::Type::UTF8String)
                    , m_value(value)
                {}

            private:
                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
        };


        /**
         * Variant specialization for the integer type.
         */
        template<>
        struct VariantImpl<int> : Variant
        {
            friend struct libember::glow::Variant;

            typedef int value_type;

            public:
                /**
                 * Returns the value.
                 */
                int toInteger() const
                {
                    return m_value;
                }

                /**
                 * Returns the value as double.
                 */
                double toReal() const
                {
                    return m_value;
                }

                /**
                 * Converts the value into a string and returns it.
                 */
                std::string toString() const
                {
                    std::stringstream stream;
                    stream << m_value;

                    return stream.str();
                }

            private:
                /**
                 * Constructor initializing the variant with an integer.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ber::Type::Integer)
                    , m_value(value)
                {}

            private:
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
            friend struct libember::glow::Variant;

            typedef double value_type;
            public:
                /**
                 * Returns the value as integer.
                 */
                int toInteger() const
                {
                    return static_cast<int>(m_value);
                }

                /**
                 * Returns the double value.
                 */
                double toReal() const
                {
                    return m_value;
                }

                /**
                 * Converts the value into a string and returns it.
                 */
                std::string toString() const
                {
                    std::stringstream stream;
                    stream << m_value;

                    return stream.str();
                }

            private:
                /**
                 * Constructor initializing the variant with the provided double value.
                 * @param value Value to initialize the variant with.
                 */
                VariantImpl(value_type const& value)
                    : Variant(ber::Type::Real)
                    , m_value(value)
                {}

            private:
                /** Prohibit assignment */
                VariantImpl& operator=(VariantImpl const&);

            private:
                value_type const m_value;
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

#endif // __LIBEMBER_GLOW_VARIANT_HPP
