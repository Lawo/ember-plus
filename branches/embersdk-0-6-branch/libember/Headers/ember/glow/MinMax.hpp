#ifndef __LIBEMBER_GLOW_MINMAX_HPP
#define __LIBEMBER_GLOW_MINMAX_HPP

#include "../ber/Type.hpp"
#include "detail/Variant.hpp"

namespace libember { namespace glow {
    /**
     * A class that may store an integer or a real value. It is
     * used as minimum and maximum property of a parameter.
     */
    class MinMax
    {
    public:
        /**
         * Initialzes an integer value.
         * @param value Value to store.
         */
        MinMax(int value);

        /**
         * Initialzes a real value.
         * @param value Value to store.
         */
        MinMax(double value);

        /**
         * Copy constructor.
         * @param other Instance to copy the value from.
         */
        MinMax(MinMax const& other);

        /**
         * Destructor
         */
        ~MinMax();

        /**
         * Returns the data type of the current value.
         * @return The data type of the value.
         */
        ber::Type type() const;

        /**
         * Returns the current value as integer.
         * @return The current value as integer.
         */
        int toInteger() const;

        /**
         * Returns the current value as real.
         * @return The current value as real.
         */
        double toReal() const;

        /**
         * Exchanges the values of this instance and the one provided.
         * @param other MinMax instance to exchange the value with.
         */
        void swap(MinMax& other);

        /**
         * Assignment operator.
         * @param other Instance to copy the value from.
         */
        MinMax& operator=(MinMax other);

    private:
        Variant* m_value;
    };

    /******************************************************
     * Inline Implementation                              *
     ******************************************************/

    inline MinMax::MinMax(double value)
        : m_value(Variant::create(value))
    {}

    inline MinMax::MinMax(int value)
        : m_value(Variant::create(value))
    {}

    inline MinMax::MinMax(const MinMax &other)
        : m_value(other.m_value->addRef())
    {}

    inline MinMax::~MinMax()
    {
        m_value->releaseRef();
    }

    inline int MinMax::toInteger() const
    {
        return m_value->toInteger();
    }

    inline double MinMax::toReal() const
    {
        return m_value->toReal();
    }

    inline void MinMax::swap(MinMax &other)
    {
        std::swap(m_value, other.m_value);
    }

    inline MinMax& MinMax::operator=(MinMax other)
    {
        swap(other);
        return *this;
    }

    inline void swap(MinMax& lhs, MinMax& rhs)
    {
        lhs.swap(rhs);
    }
}
}

#endif  // __LIBEMBER_GLOW_MINMAX_HPP
