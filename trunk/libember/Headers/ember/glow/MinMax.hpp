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

#ifndef __LIBEMBER_GLOW_MINMAX_HPP
#define __LIBEMBER_GLOW_MINMAX_HPP

#include "ParameterType.hpp"
#include "Variant.hpp"
#include "util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    /**
     * Stores an integer or a real value. This class is
     * used as minimum and maximum property of a parameter.
     */
    class MinMax
    {
        public:
            /**
             * Initialzes an integer value.
             * @param value Value to store.
             */
            MinMax(long value);

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
             * Initializes a new MinMax instance from the passed type erased ber value.
             * @param value The ber value to initialize this instance with.
             */
            MinMax(ber::Value const& value);

            /**
             * Destructor
             */
            ~MinMax();

            /**
             * Returns the data type of the current value.
             * @return The data type of the value.
             */
            ParameterType type() const;

            /**
             * Returns the current value as integer.
             * @return The current value as integer.
             */
            long toInteger() const;

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

    inline MinMax::MinMax(ber::Value const& value)
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
            }
        }

        if (m_value == 0)
            m_value = Variant::create(0L);
    }

    inline MinMax::MinMax(double value)
        : m_value(Variant::create(value))
    {}

    inline MinMax::MinMax(long value)
        : m_value(Variant::create(value))
    {}

    inline MinMax::MinMax(const MinMax &other)
        : m_value(other.m_value->addRef())
    {}

    inline MinMax::~MinMax()
    {
        m_value->releaseRef();
    }

    inline long MinMax::toInteger() const
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

    inline ParameterType MinMax::type() const
    {
        return m_value->type();
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
