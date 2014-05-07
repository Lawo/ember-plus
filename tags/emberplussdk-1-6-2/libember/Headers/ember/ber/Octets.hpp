/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_BER_OCTETS_HPP
#define __LIBEMBER_BER_OCTETS_HPP

#include <vector>

namespace libember { namespace ber 
{
    /**
     * Represents a byte array, called OctetString in BER
     */
    class Octets
    {
        typedef std::vector<unsigned char> ByteVector;
        public:
            typedef ByteVector::iterator iterator;
            typedef ByteVector::const_iterator const_iterator;
            typedef ByteVector::size_type size_type;
            typedef ByteVector::value_type value_type;

            /** Default constructor */
            Octets();

            /** Copy constructor
             * @param other The instance to copy the data from
             */
            Octets(Octets const& other);

            /**
             * Initializes a new instance of Octets with the provided buffer
             * @param first First item to copy
             * @param last First item beyond the buffer to copy
             */
            template<typename InputIterator>
            Octets(InputIterator first, InputIterator last);

            /**
             * Returns the number of bytes this instance stores.
             */
            size_type size() const;

            /**
             * Returns a reference to the first item of the buffer.
             * @return A reference to the firs titem of the buffer.
             */
            iterator begin();

            /**
             * Returns a reference to the first item beyond the buffer.
             * @return A reference to the first item beyond the buffer.
             */
            iterator end();

            /**
             * Returns a const reference to the first item of the buffer.
             * @return A const reference to the firs titem of the buffer.
             */
            const_iterator begin() const;

            /**
             * Returns a const reference to the first item beyond the buffer.
             * @return A const reference to the first item beyond the buffer.
             */
            const_iterator end() const;

        private:
            ByteVector m_data;
    };

    /**************************************************************************/
    /* Inline implementation                                                  */
    /**************************************************************************/

    inline Octets::Octets()
    {
    }

    inline Octets::Octets(Octets const& other)
        : m_data(other.begin(), other.end())
    {
    }

    template<typename InputIterator>
    inline Octets::Octets(InputIterator first, InputIterator last)
        : m_data(first, last)
    {
    }

    inline Octets::size_type Octets::size() const
    {
        return m_data.size();
    }

    inline Octets::iterator Octets::begin()
    {
        return m_data.begin();
    }

    inline Octets::iterator Octets::end() 
    {
        return m_data.end();
    }

    inline Octets::const_iterator Octets::begin() const 
    {
        return m_data.begin();
    }

    inline Octets::const_iterator Octets::end() const 
    {
        return m_data.end();
    }
}
}

#endif  // __LIBEMBER_BER_OCTETS_HPP
