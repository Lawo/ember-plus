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

#ifndef __LIBEMBER_GLOW_FORMULA_HPP
#define __LIBEMBER_GLOW_FORMULA_HPP

#include <string>

namespace libember { namespace glow
{
    /**
     * A class that stores the terms to compute the display value from
     * a device value and vice versa.s
     */
    class Formula
    {
        public:
            typedef std::string value_type;
            typedef value_type& reference;
            typedef value_type const& const_reference;

            /**
             * Initializes a new Formula instance
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
}
}

#endif  // __LIBEMBER_GLOW_FORMULA_HPP
