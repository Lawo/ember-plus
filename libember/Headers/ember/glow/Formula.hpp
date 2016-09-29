/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_FORMULA_HPP
#define __LIBEMBER_GLOW_FORMULA_HPP

#include <string>

namespace libember { namespace glow
{
    /**
     * A class that stores the terms to compute the display value from
     * a device value and vice versa.
     */
    class Formula
    {
        public:
            typedef std::string value_type;
            typedef value_type& reference;
            typedef value_type const& const_reference;

            /**
             * Initializes a new Formula instance.
             * @param providerToConsumer The term that can be used to compute the device value
             *      from a display value.
             * @param consumerToProvider The term that can be used to compute the display value
             *      from a device value.
             */
            Formula(const_reference providerToConsumer, const_reference consumerToProvider);

            /**
             * Returns the term to use when transforming a consumer (device) value into a provider (display) value.
             * @return The term to use when transforming a consumer (device) value into a provider (display) value.
             */
            const_reference consumerToProvider() const;

            /**
             * Returns the term to use when transforming a provider (display) value into a consumer (device) value.
             * @return The term to use when transforming a provider (display) value into a consumer (device) value.
             */
            const_reference providerToConsumer() const;

        private:
            value_type m_consumerToProvider;
            value_type m_providerToConsumer;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline Formula::Formula(const_reference providerToConsumer, const_reference consumerToProvider)
        : m_consumerToProvider(consumerToProvider)
        , m_providerToConsumer(providerToConsumer)
    {}

    inline Formula::const_reference Formula::consumerToProvider() const
    {
        return m_consumerToProvider;
    }

    inline Formula::const_reference Formula::providerToConsumer() const
    {
        return m_providerToConsumer;
    }

    inline bool operator ==(Formula const& lhs, Formula const& rhs)
    {
        return lhs.consumerToProvider() == rhs.consumerToProvider()
            && lhs.providerToConsumer() == rhs.providerToConsumer();
    }

    inline bool operator !=(Formula const& lhs, Formula const& rhs)
    {
        return !(lhs == rhs);
    }
}
}

#endif  // __LIBEMBER_GLOW_FORMULA_HPP
