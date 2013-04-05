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

#ifndef __LIBEMBER_BER_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_OBJECTIDENTIFIER_HPP

#include <algorithm>
#include <vector>
#include "../util/Api.hpp"

namespace libember { namespace ber
{
    /**
     * A simple template type that wraps an array of signed integer values representing a
     * relative object identifier.
     */
    class LIBEMBER_API ObjectIdentifier
    {
        typedef std::vector<std::size_t> Vector;

        public:
            typedef Vector::value_type value_type;
            typedef Vector::size_type size_type;
            typedef Vector::iterator iterator;
            typedef Vector::const_iterator const_iterator;

        public:
            /**
             * Default constructor. Initializes the wrapped object identifier
             * with an empty array.
             */
            ObjectIdentifier();

            /**
             * Constructor that copies the passed buffer containing a relative object identifier.
             * @param value the value to wrap.
             */
            template<typename InputIterator>
            ObjectIdentifier(InputIterator first, InputIterator last);

            /**
             * Initializes a new ObjectIdentifier which contains only one element.
             * @param value The single value to initialize the ObjectIdentifier with.
             */
            explicit ObjectIdentifier(value_type value);

            /**
             * Returns true if the ObjectIdentifier does not contain any elements.
             * @retunr True if the ObjectIdentifier does not contain any elements.
             */
            bool empty() const;

            /**
             * Returns the number of elements this oid contains.
             * @return The number of elements this oid contains.
             */
            size_type size() const;

            /**
             * Return a const iterator referring to the first identifier contained 
             * within this object identifier
             * @return A const iterator referring to the first identifier contained
             *      within this object identifier.
             */
            const_iterator begin() const;

            /**
             * Return a const iterator referring to the element one past the last
             * element in the sequence of identifiers contained within this
             * object identifier.
             * @return A const iterator referring to the element one past the last
             *      element in the sequence of identifiers contained within
             *      this object identifier.
             */
            const_iterator end() const;

            /**
             * Return an iterator referring to the first identifier contained 
             * within this object identifier
             * @return An iterator referring to the first identifier contained
             *      within this object identifier.
             */
            iterator begin();

            /**
             * Return an iterator referring to the element one past the last
             * element in the sequence of identifiers contained within this
             * object identifier.
             * @return An iterator referring to the element one past the last
             *      element in the sequence of identifiers contained within
             *      this object identifier.
             */
            iterator end();

            /**
             * Swap method that exchanges the object identifier stored within
             * this instance with the object identifier stored in @p other.
             * @param other a reference to the ObjectIdentifier instance this
             *      instance should exchange its contents with.
             */
            void swap(ObjectIdentifier& other);

            /**
             * Gets the sub-id at position @p index.
             * @param index the position of the sub-id to return.
             */
            std::size_t operator[](int index) const;

        private:
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif
            Vector m_items;
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
    };


    /**
     * Free version of swap to allow it's usage through ADL.
     * @param lhs a reference to the first instance.
     * @param rhs a reference to a second instance whose contents should be
     *      swapped with those of @p lhs.
     */
    void swap(ObjectIdentifier& lhs, ObjectIdentifier& rhs);


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    inline void swap(ObjectIdentifier& lhs, ObjectIdentifier& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename InputIterator>
    ObjectIdentifier::ObjectIdentifier(InputIterator first, InputIterator last)
    {
        m_items.assign(first, last);
    }

    inline bool ObjectIdentifier::empty() const
    {
        return m_items.empty();
    }

    inline ObjectIdentifier::const_iterator ObjectIdentifier::begin() const
    {
        return m_items.begin();
    }

    inline ObjectIdentifier::const_iterator ObjectIdentifier::end() const
    {
        return m_items.end();
    }

    inline ObjectIdentifier::iterator ObjectIdentifier::begin() 
    {
        return m_items.begin();
    }

    inline ObjectIdentifier::iterator ObjectIdentifier::end() 
    {
        return m_items.end();
    }

    inline ObjectIdentifier::size_type ObjectIdentifier::size() const
    {
        return m_items.size();
    }

    inline bool operator==(ObjectIdentifier const& left, ObjectIdentifier const& right)
    {
        if (left.size() == right.size())
        {
            ObjectIdentifier::const_iterator leftIt = left.begin();
            ObjectIdentifier::const_iterator rightIt = right.begin();
            ObjectIdentifier::const_iterator const leftLast = left.end();

            for (/* Nothing */; leftIt != leftLast; ++leftIt, ++rightIt)
            {
                if (*leftIt != *rightIt)
                    return false;
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool operator!=(ObjectIdentifier const& left, ObjectIdentifier const& right)
    {
        return !(left == right);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/ObjectIdentifier.ipp"
#endif


#endif  // __LIBEMBER_BER_OBJECTIDENTIFIER_HPP


