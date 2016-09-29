/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_DREFITERATOR_HPP
#define __LIBEMBER_UTIL_DREFITERATOR_HPP

#include <iterator>
#include "../meta/RemovePointer.hpp"

namespace libember { namespace util
{
    /**
     * An iterator adapter that reduces one level of indirection by deferring
     * the references returned by another iterator.
     * This may for example be used to make a sequence of pointers appear as a
     * sequence of values.
     */
    template<typename IteratorType>
    class DerefIterator
    {
        public:
            typedef typename std::iterator_traits<IteratorType>::iterator_category  iterator_category;
            typedef typename meta::RemovePointer<
                        typename std::iterator_traits<IteratorType>::value_type
                    >::type                                                         value_type;
            typedef value_type*                                                     pointer;
            typedef value_type&                                                     reference;
            typedef typename std::iterator_traits<IteratorType>::difference_type    difference_type;

        public:
            /**
             * Default constructor. Initializes this instance in a singular
             * state.
             */
            DerefIterator();

            /**
             * Single argument constructor that initializes the instance with
             * the specified iterator.
             * @param it the iterator to initializes this instance with.
             * @note This constructor is intentionally marked non explicit.
             */
            DerefIterator(IteratorType it);

            /**
             * Overloaded dereference operator. Returns a reference to the
             * element this iterator refers to. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined. 
             * @return A reference to the element referred to by this iterator.
             */
            reference operator*() const;

            /**
             * Overloaded member access operator. Returns a pointer to the
             * element this iterator refers to. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined. 
             * @return A pointer to the element referred to by this iterator.
             */
            pointer operator->() const;

            /**
             * Overloaded offset dereference operator.
             * @param n the number of steps to offset this iterator by before
             *      dereferencing the referred element.
             * @return A reference to the element at offset @p n from the
             *      element referred to by this iterator..
             */
            reference operator[](difference_type n) const;

            /**
             * Overloaded pre increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A reference to this instance.
             */
            DerefIterator& operator++();

            /**
             * Overloaded post increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A DerefIterator referring to the current, not
             *      incremented position.
             */
            DerefIterator operator++(int);

            /**
             * Overloaded pre increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A reference to this instance.
             */
            DerefIterator& operator--();

            /**
             * Overloaded post increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A DerefIterator referring to the current, not
             *      incremented position.
             */
            DerefIterator operator--(int);

            /**
             * Overloaded increment assignment operator. Advances the currently
             * wrapped iterator by @p n steps.
             * @param n the number of steps to advance the wrapped iterator by.
             * @return A reference to this instance.
             */
            DerefIterator& operator+=(difference_type n);

            /**
             * Overloaded decrement assignment operator. Retrogress the currently
             * wrapped iterator by @p n steps.
             * @param n the number of steps to retrogress the wrapped iterator by.
             * @return A reference to this instance.
             */
            DerefIterator& operator-=(difference_type n);

            /**
             * Inplace swap function that exchanges the state of this instance
             * with the state of @p other. This method is guaranteed not to
             * throw any exceptions.
             * @param other a reference to the DerefIterator instance
             *      whose contents should be exchanged with those of this
             *      instance.
             */
            void swap(DerefIterator& other);

            /**
             * Return a copy of the wrapped iterator.
             * @return A copy of the wrapped iterator.
             */
            IteratorType wrappedIterator() const;

        private:
            IteratorType m_iter;
    };

    /**
     * Equality comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise false.
     */
    template<typename IteratorType>
    bool operator==(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Inequality comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return False if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise true.
     */
    template<typename IteratorType>
    bool operator!=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Greater than comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs is greater than @p rhs, otherwise false.
     */
    template<typename IteratorType>
    bool operator>(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Less than comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs is less than @p rhs, otherwise false.
     */
    template<typename IteratorType>
    bool operator<(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Greater than or equal comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs is greater than or equal to @p rhs, otherwise false.
     */
    template<typename IteratorType>
    bool operator>=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Less than or equal comparison operator for DerefIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs is less than or equal to @p rhs, otherwise false.
     */
    template<typename IteratorType>
    bool operator<=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Advance by n operator for DerefIterator specializations.
     * @param lhs the iterator that should be advanced.
     * @param rhs the number of elements @p lhs should be advanced by.
     * @return An iterator referring the element that is located @p rhs elements
     *      after the element referred to by @p lhs.
     */
    template<typename IteratorType>
    DerefIterator<IteratorType> operator+(DerefIterator<IteratorType> lhs, typename DerefIterator<IteratorType>::difference_type rhs);

    /**
     * Advance by n operator for DerefIterator specializations.
     * @param lhs the number of elements @p rhs should be advanced by.
     * @param rhs the iterator that should be advanced.
     * @return An iterator referring the element that is located @p lhs elements
     *      after the element referred to by @p rhs.
     */
    template<typename IteratorType>
    DerefIterator<IteratorType> operator+(typename DerefIterator<IteratorType>::difference_type lhs, DerefIterator<IteratorType> rhs);

    /**
     * Retrogress by n operator for DerefIterator specializations.
     * @param lhs the iterator that should be advanced.
     * @param rhs the number of elements @p lhs should be retrogressed by.
     * @return An iterator referring the element that is located @p rhs elements
     *      before the element referred to by @p lhs.
     */
    template<typename IteratorType>
    DerefIterator<IteratorType> operator-(DerefIterator<IteratorType> lhs, typename DerefIterator<IteratorType>::difference_type rhs);

    /**
     * Difference operator for DerefIterator specializations.
     * @param lhs a constant reference to an iterator.
     * @param rhs a constant reference to another iterator.
     * @return The distance between @p rhs and @p lhs.
     */
    template<typename IteratorType>
    typename DerefIterator<IteratorType>::difference_type operator-(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs);

    /**
     * Free version of swap to allow it's usage through ADL.
     * @param lhs a reference to the first instance.
     * @param rhs a reference to a second instance whose contents should be
     *      swapped with those of @p lhs.
     */
    template<typename IteratorType>
    void swap(DerefIterator<IteratorType>& lhs, DerefIterator<IteratorType>& rhs);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/


    template<typename IteratorType>
    inline DerefIterator<IteratorType>::DerefIterator()
        : m_iter()
    {}

    template<typename IteratorType>
    inline DerefIterator<IteratorType>::DerefIterator(IteratorType it)
        : m_iter(it)
    {}

    template<typename IteratorType>
    inline typename DerefIterator<IteratorType>::reference DerefIterator<IteratorType>::operator*() const
    {
        return *(*m_iter);
    }

    template<typename IteratorType>
    inline typename DerefIterator<IteratorType>::pointer DerefIterator<IteratorType>::operator->() const
    {
        return *m_iter;
    }

    template<typename IteratorType>
    inline typename DerefIterator<IteratorType>::reference DerefIterator<IteratorType>::operator[](difference_type n) const
    {
        return *m_iter[n];
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType>& DerefIterator<IteratorType>::operator++()
    {
        ++m_iter;
        return *this;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType> DerefIterator<IteratorType>::operator++(int)
    {
        DerefIterator const result(*this);
        ++(*this);
        return result;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType>& DerefIterator<IteratorType>::operator--()
    {
        --m_iter;
        return *this;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType> DerefIterator<IteratorType>::operator--(int)
    {
        DerefIterator const result(*this);
        --(*this);
        return result;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType>& DerefIterator<IteratorType>::operator+=(difference_type n)
    {
        m_iter += n;
        return *this;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType>& DerefIterator<IteratorType>::operator-=(difference_type n)
    {
        m_iter -= n;
        return *this;
    }

    template<typename IteratorType>
    inline void DerefIterator<IteratorType>::swap(DerefIterator& other)
    {
        using std::swap;
        swap(m_iter, other.m_iter);
    }

    template<typename IteratorType>
    inline IteratorType DerefIterator<IteratorType>::wrappedIterator() const
    {
        return m_iter;
    }


    template<typename IteratorType>
    inline bool operator==(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() == rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline bool operator!=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() != rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline bool operator>(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() > rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline bool operator<(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() < rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline bool operator>=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() >= rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline bool operator<=(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return (lhs.wrappedIterator() <= rhs.wrappedIterator());
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType> operator+(DerefIterator<IteratorType> lhs, typename DerefIterator<IteratorType>::difference_type rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType> operator+(typename DerefIterator<IteratorType>::difference_type lhs, DerefIterator<IteratorType> rhs)
    {
        rhs += lhs;
        return rhs;
    }

    template<typename IteratorType>
    inline DerefIterator<IteratorType> operator-(DerefIterator<IteratorType> lhs, typename DerefIterator<IteratorType>::difference_type rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    template<typename IteratorType>
    inline typename DerefIterator<IteratorType>::difference_type operator-(DerefIterator<IteratorType> const& lhs, DerefIterator<IteratorType> const& rhs)
    {
        return lhs.wrappedIterator() - rhs.wrappedIterator();
    }

    template<typename IteratorType>
    inline void swap(DerefIterator<IteratorType>& lhs, DerefIterator<IteratorType>& rhs)
    {
        return lhs.swap(rhs);
    }

}
}

#endif  // __LIBEMBER_UTIL_DREFITERATOR_HPP

