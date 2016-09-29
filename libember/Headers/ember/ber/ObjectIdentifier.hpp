/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_OBJECTIDENTIFIER_HPP

#include <algorithm>
#include <deque>
#include "../util/Api.hpp"

namespace libember { namespace ber
{
    /**
     * A simple template type that wraps an array of signed integer values representing a
     * relative object identifier.
     */
    class LIBEMBER_API ObjectIdentifier
    {
        typedef std::deque<std::size_t> Container;

        public:
            typedef Container::value_type value_type;
            typedef Container::size_type size_type;
            typedef Container::reference reference;
            typedef Container::const_reference const_reference;
            typedef Container::iterator iterator;
            typedef Container::const_iterator const_iterator;

        public:
            /**
             * Default constructor. Initializes the wrapped object identifier
             * with an empty array.
             */
            ObjectIdentifier();

            /**
             * Constructor that copies the passed buffer containing a relative object identifier.
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
             * @return True if the ObjectIdentifier does not contain any elements.
             */
            bool empty() const;

            /**
             * Returns the number of elements this oid contains.
             * @return The number of elements this oid contains.
             */
            size_type size() const;

            /**
             * Returns reference to the first element of this oid.
             * @return Reference to the first element of this oid.
             */
            reference front();

            /**
             * Returns reference to the first element of this oid.
             * @return Reference to the first element of this oid.
             */
            const_reference front() const;

            /**
             * Returns reference to the last element of this oid.
             * @return Reference to the last element of this oid.
             */
            reference back();

            /**
             * Returns reference to the last element of this oid.
             * @return Reference to the last element of this oid.
             */
            const_reference back() const;

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
             * Return the sub-identifier at position @p index.
             * @param index the position of the sub-identifier to return.
             * @return The sub-identifier at position @p index.
             * @note Please note that this method performs no bounds checking
             *      on the supplied index. The behaviour of this method is
             *      undefined if the supplied index is not within the valid
             *      range.
             */
            value_type operator[](int index) const;

            /**
             * Appends the given element value to the end of the oid.
             * @param value The value to append to the oid.
             */
            void push_back(value_type value);

            /**
             * Prepends the given element value to the beginning of the oid.
             * @param value The value to prepend to the oid.
             */
            void push_front(value_type value);

            /**
             * Removes the last element of the oid.
             */
            void pop_back();

            /**
             * Removes the first element of the oid.
             */
            void pop_front();

        private:
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif
            Container m_items;
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

    /**
     * Equality comparison operator for object idtentifiers.
     * @param lhs a constant reference to the first instance to be compared
     *      for equality.
     * @param rhs a constant reference to the second instance to be compared
     *      for equality.
     * @return True if @p left and @p right represent the same relative
     *      reference, otherwise false.
     */
    bool operator==(ObjectIdentifier const& lhs, ObjectIdentifier const& rhs);

    /**
     * Inequality comparison operator for object idtentifiers.
     * @param lhs a constant reference to the first instance to be compared
     *      for inequality.
     * @param rhs a constant reference to the second instance to be compared
     *      for inequality.
     * @return True if @p left and @p right represent the different relative
     *      references, otherwise false.
     */
    bool operator!=(ObjectIdentifier const& lhs, ObjectIdentifier const& rhs);



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

    inline ObjectIdentifier::reference ObjectIdentifier::front()
    {
        return m_items.front();
    }

    inline ObjectIdentifier::const_reference ObjectIdentifier::front() const
    {
        return m_items.front();
    }

    inline ObjectIdentifier::reference ObjectIdentifier::back()
    {
        return m_items.back();
    }

    inline ObjectIdentifier::const_reference ObjectIdentifier::back() const
    {
        return m_items.back();
    }

    inline void ObjectIdentifier::pop_back()
    {
        return m_items.pop_back();
    }

    inline void ObjectIdentifier::pop_front()
    {
        return m_items.pop_front();
    }

    inline bool operator!=(ObjectIdentifier const& lhs, ObjectIdentifier const& rhs)
    {
        return !(lhs == rhs);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/ObjectIdentifier.ipp"
#endif


#endif  // __LIBEMBER_BER_OBJECTIDENTIFIER_HPP


