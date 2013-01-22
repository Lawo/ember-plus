#ifndef __TINYEMBER_GADGET_COLLECTION_H
#define __TINYEMBER_GADGET_COLLECTION_H

#include <memory>
#include <list>

namespace gadget
{
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
            template<typename InputIterator>
            Collection(InputIterator first, InputIterator last);

            Collection();

            size_type size() const;

            iterator begin();

            iterator end();

            const_iterator begin() const;

            const_iterator end() const;

            bool empty() const;

            bool any() const;

            void insert(iterator where, ValueType value);

            void remove(iterator where);

        private:
            Container m_container;
    };

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
