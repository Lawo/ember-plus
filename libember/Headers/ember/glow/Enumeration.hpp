/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_ENUMERATION_HPP
#define __LIBEMBER_GLOW_ENUMERATION_HPP

#include <algorithm>
#include <string>
#include <vector>

namespace libember { namespace glow 
{
    namespace detail
    {
        /**
         * Templated struct which is used to create enumeration entries 
         * from either a pair of type std::pair<std::string, int> or an std::string.
         */
        template<typename InputIterator, typename ValueType>
        struct EnumEntryFactory
        {
        };

        /**
         * Specialization of the EnumEntryFactory which simply copies the 
         * entries provided.
         */
        template<typename InputIterator>
        struct EnumEntryFactory<InputIterator, std::pair<std::string, int> > 
        {
            /**
             * Copies the provided buffer.
             * @note The provided output must be able to consume as many items 
             *      as required.
             * @param first Points to the first item in the buffer which contains the enumeration
             *      entries.
             * @param last Points to the first item beyond the enumeration-entry buffer.
             * @param output Iterator to assign the provided entries to. Must support the pre-increment
             *      operator.
             */
            template<typename OutputIterator>
            static void produce(InputIterator first, InputIterator last, OutputIterator output)
            {
                for (; first != last; ++first, ++output)
                {
                    *output = *first;
                }
            }
        };

        /**
         * Specialization of the EnumEntryFactory which generates indexed enumeration entries
         * from a collection of strings. 
         */
        template<typename InputIterator>
        struct EnumEntryFactory<InputIterator, std::string> 
        {
            /**
             * Creates indexed enumeration entries from the provided strings and assigns them
             * to the provided output iterator.
             * @note The provided output must be able to consume as many items
             *      as required.
             * @param first Points to the first item in the string collection.
             * @param last Points to the first item beyond the string collection.
             * @param output Iterator to assign the generated entries to.
             */
            template<typename OutputIterator>
            static void produce(InputIterator first, InputIterator last, OutputIterator output)
            {
                for (int index = 0; first != last; ++first, ++output, ++index)
                {
                    *output = std::make_pair(*first, index);
                }
            }
        };
    }

    /**
     * Stores an indexed enumeration. Each entry is a tuple of type std::pair<string, int>, where the integer
     * indicates the device value or index and the string contains the display name of the 
     * enumerated entry.
     */
    class Enumeration
    {
        typedef std::vector<std::pair<std::string, int> > EntryContainer;
        public:
            typedef EntryContainer::value_type value_type;
            typedef EntryContainer::size_type size_type;
            typedef EntryContainer::iterator iterator;
            typedef EntryContainer::const_iterator const_iterator;
            typedef EntryContainer::reference reference;
            typedef EntryContainer::const_reference const_reference;

        public:
            /**
             * Initializes a new Enumeration. The InputIterator may be of two types, either
             * a collection of std::string or std::pair<std::string, int>.
             * @param first The first item of the enumeration entries to copy.
             * @param last Points to the first item beyond the enumeration entry collection.
             */
            template<typename InputIterator>
            Enumeration(InputIterator first, InputIterator last);

            /**
             * Initializes a new Enumeration and copies the elements from an existing Enumeration.
             * @param other Enumeration to copy the values from
             */
            Enumeration(Enumeration const& other);

            /**
             * Returns the number of enumeration entries.
             * @return The number of enumeration entries.
             */
            size_type size() const;

            /**
             * Returns an iterator to the first enumeration entry.
             * @return An iterator to the first enumeration entry.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator pointing to the first item beyond the entry collection.
             * @return An iterator pointing to the first item beyond the entry collection.
             */
            const_iterator end() const;

            /**
             * Returns the entry at the specified index.
             * @param index The index of the item to return.
             * @return A const reference of the item at the specified position.
             */
            const_reference operator[](int index) const;

            /**
             * Assignment operator, copies the elements of the provided Enumeration instance.
             * @param other Enumeration to copy the values from.
             */
            Enumeration& operator=(Enumeration const& other);

        private:
            EntryContainer m_items;
    };


    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline Enumeration::Enumeration(InputIterator first, InputIterator last)
        : m_items(std::distance(first, last), std::make_pair(std::string(), 0))
    {
        typedef detail::EnumEntryFactory<InputIterator, typename std::iterator_traits<InputIterator>::value_type> Factory;
        Factory::produce(first, last, m_items.begin());
    }

    inline Enumeration::Enumeration(Enumeration const& other)
        : m_items(other.begin(), other.end())
    {}

    inline Enumeration& Enumeration::operator=(Enumeration const& other)
    {
        m_items.assign(other.begin(), other.end());
        return *this;
    }

    inline Enumeration::size_type Enumeration::size() const
    {
        return m_items.size();
    }

    inline Enumeration::const_iterator Enumeration::begin() const
    {
        return m_items.begin();
    }

    inline Enumeration::const_iterator Enumeration::end() const
    {
        return m_items.end();
    }

    inline Enumeration::const_reference Enumeration::operator[](int index) const
    {
        return m_items[index];
    }

    inline bool operator ==(Enumeration const& lhs, Enumeration const& rhs)
    {
        Enumeration::size_type const lhsSize = lhs.size();
        Enumeration::size_type const rhsSize = rhs.size();

        if (lhsSize == rhsSize)
        {
            for (Enumeration::size_type i = 0; i < lhsSize; ++i)
            {
                Enumeration::const_reference lhsEntry = lhs[i];
                Enumeration::const_reference rhsEntry = rhs[i];

                if (lhsEntry.second != rhsEntry.second
                ||  lhsEntry.first != rhsEntry.first)
                {
                    return false;
                }
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool operator !=(Enumeration const& lhs, Enumeration const& rhs)
    {
        return !(lhs == rhs);
    }
}
}

#endif//__LIBEMBER_GLOW_ENUMERATION_HPP
