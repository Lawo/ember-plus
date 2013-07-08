#ifndef __TINYEMBER_GADGET_COLLECTION_H
#define __TINYEMBER_GADGET_COLLECTION_H

#include <memory>
#include <list>

namespace gadget
{
    /**
     * Simple collection class which is used by the node class to store child nodes and parameters.
     */
    template<typename ValueType>
    class Collection
    {
        typedef std::list<ValueType> Container;
        public:
            typedef typename Container::value_type value_type;
            typedef typename Container::size_type size_type;
            typedef typename Container::iterator iterator;
            typedef typename Container::const_iterator const_iterator;

        public:
            /**
             * Initializes a new Collection with the specified collection of elements.
             * @param first Reference to the first element to copy.
             * @param last Reference to the first element beyond the collection of items to copy.
             */
            template<typename InputIterator>
            Collection(InputIterator first, InputIterator last);

            /**
             * Initializes an empty collection
             */
            Collection();

            /**
             * Returns the number of elements the collection contains.
             * @return The number of elements the collection contains.
             */
            size_type size() const;

            /**
             * Returns a reference to the first element of this collection.
             * @return A reference to the first element of this collection.
             */
            iterator begin();

            /**
             * Returns an iterator referring to the element one past the last element
             * of this collection.
             * @return An iterator referring to the element one past the last element
             * of this collection.
             */
            iterator end();

            /**
             * Returns a reference to the first element of this collection.
             * @return A reference to the first element of this collection.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator referring to the element one past the last element
             * of this collection.
             * @return An iterator referring to the element one past the last element
             * of this collection.
             */
            const_iterator end() const;

            /**
             * Returns true if the collection is empty.
             * @return true if the collection is empty.
             */
            bool empty() const;

            /**
             * Returns true if the collection contains at least one element.
             * @return true if the collection contains at least one element.
             */
            bool any() const;

            /**
             * Inserts a new element at the specified location.
             * @param where An iterator pointing the location where the value should be inserted.
             * @param value The value to add to the collection.
             */
            void insert(iterator where, ValueType value);

            /**
             * Removes an item at the specified location.
             * @param where An iterator pointing to the element to remove.
             */
            void remove(iterator where);

        private:
            Container m_container;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename ValueType>
    template<typename InputIterator>
    inline Collection<ValueType>::Collection(InputIterator first, InputIterator last)
        : m_container(first, last)
    {
    }

    template<typename ValueType>
    inline Collection<ValueType>::Collection()
    {
    }

    template<typename ValueType>
    inline bool Collection<ValueType>::empty() const
    {
        return size() == 0;
    }

    template<typename ValueType>
    inline bool Collection<ValueType>::any() const
    {
        return size() > 0;
    }

    template<typename ValueType>
    inline typename Collection<ValueType>::size_type Collection<ValueType>::size() const
    {
        return m_container.size();
    }

    template<typename ValueType>
    inline typename Collection<ValueType>::iterator Collection<ValueType>::begin()
    {
        return m_container.begin();
    }

    template<typename ValueType>
    inline typename Collection<ValueType>::iterator Collection<ValueType>::end()
    {
        return m_container.end();
    }

    template<typename ValueType>
    inline typename Collection<ValueType>::const_iterator Collection<ValueType>::begin() const
    {
        return m_container.begin();
    }

    template<typename ValueType>
    inline typename Collection<ValueType>::const_iterator Collection<ValueType>::end() const
    {
        return m_container.end();
    }

    template<typename ValueType>
    inline void Collection<ValueType>::insert(iterator where, ValueType value)
    {
        m_container.insert(where, value);
    }

    template<typename ValueType>
    inline void Collection<ValueType>::remove(iterator where)
    {
        m_container.erase(where);
    }
}

#endif//__TINYEMBER_GADGET_COLLECTION_H
