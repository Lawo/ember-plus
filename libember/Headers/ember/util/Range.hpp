/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_RANGE_HPP
#define __LIBEMBER_UTIL_RANGE_HPP

#include <iterator>

namespace libember { namespace util
{

    /**
     * A simple pair of iterators that encapsulates a range of elements.
     * In addition to simply wrapping two iterators this class also provides an
     * interface that allows using a range in most contexts where a model of the
     * Container concept is required. Notable deviations from the Container
     * concept are:
     * - The associated types const_reference and const_pointer are not
     *   provided as they wouldn't make any sense anyway. Instead the associated
     *   types pointer and reference are const qualified in case the iterator
     *   type of the range is a constant iterator.
     */ 
    template<typename IteratorType>
    class Range
    {
        public:
            typedef typename std::iterator_traits<IteratorType>::value_type      value_type;
            typedef typename std::iterator_traits<IteratorType>::reference       reference;
            typedef typename std::iterator_traits<IteratorType>::pointer         pointer;
            typedef IteratorType                                                 iterator;
            typedef IteratorType                                                 const_iterator;

            typedef std::size_t                                                  size_type;
            typedef typename std::iterator_traits<IteratorType>::difference_type difference_type;

        public:
            /**
             * Default constructor. Initializes an empty range spanned by two
             * singular iterators.
             */ 
            Range();

            /**
             * Constructor that initializes a range spanning the elements
             * enclosed by @p first and @p last. 
             * @param first an iterator referring to the first element of the
             *      enclosed range.
             * @param last an iterator referring an element one past the last
             *      element of the enclosed range.
             */ 
            Range(iterator first, iterator last);

            /**
             * Constructor that initializes the range from another range type of
             * convertible iterators.
             * @param range a constant reference to the other range whose
             *      iterator type is convertible to the iterator type on which
             *      this range is based.
             * @note This constructor is intentionally non-explicit to allow for
             *      implicit conversion.
             */
            template<typename OtherRange>
            Range(OtherRange const& range);

            /**
             * Return whether or not the enclosed range is empty.
             * @return True if the enclosed range is empty, otherwise false.
             */
            bool empty() const; 

            /**
             * Return the number of elements enclosed by this range.
             * @return The number of elements enclosed by this range.
             */
            size_type size() const;

            /**
             * Return the number of elements that may be stored in this range,
             * without causing a reallocation of the internal storage.
             * @note This method is only provided to make this type a proper
             *      model of the container concept and always returns the number
             *      of elements enclosed by this range.
             * @return The number of elements that may be stored in this range,
             *      without causing a reallocation of the internal storage.
             */
            size_type capacity() const;
            
            /**
             * Return the maximum number of elements that may be stored in this
             * range.
             * @note This method is only provided to make this type a proper
             *      model of the container concept and always returns the number
             *      of elements enclosed by this range.
             * @return The maximum number of elements that may be stored in this
             *      range.
             */
            size_type max_size() const;

            /**
             * Return an iterator referring the first element of the enclosed
             * range.
             * @return An iterator referring the first element of the enclosed
             *      range.
             */
            iterator begin() const;

            /**
             * Return an iterator referring the first element one past the last
             * element of the enclosed range.
             * @return An iterator referring the first element one past the last
             *      element of the enclosed range.
             */
            iterator end() const;

            /**
             * Non throwing swap function that exchanges the contents of this
             * range with those of @p other.
             * @param other a reference to the range with which the contents
             *      of this range should be exchanged.
             */
            void swap(Range& other);

        private:
            iterator m_begin;
            iterator m_end;
    };

    template<typename IteratorType>
    bool operator==(Range<IteratorType> const& lhs, Range<IteratorType> const& rhs);

    template<typename IteratorType>
    bool operator!=(Range<IteratorType> const& lhs, Range<IteratorType> const& rhs);

    /**
     * Generator function that allows creating a range without having to explicitly
     * specify the iterator type.
     * @param first an iterator referring to the first element in the range.
     * @param last an iterator referring one past the last element in the range.
     * @return A Range instance that represents the range of elements specified by
     *      @p first and @p last.
     */
    template<typename IteratorType>
    Range<IteratorType> make_range(IteratorType first, IteratorType last);

    /**
     * Free version of swap to allow it's usage through ADL.
     * @param lhs a reference to the first instance.
     * @param rhs a reference to a second instance whose contents should be
     *      swapped with those of @p lhs.
     */
    template<typename IteratorType>
    void swap(Range<IteratorType>& lhs, Range<IteratorType>& rhs);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename IteratorType>
    inline Range<IteratorType>::Range()
        : m_begin(), m_end()
    {}

    template<typename IteratorType>
    inline Range<IteratorType>::Range(iterator first, iterator last)
        : m_begin(first), m_end(last)
    {}

    template<typename IteratorType>
    template<typename OtherRange>
    Range<IteratorType>::Range(OtherRange const& range)
        : m_begin(range.begin()), m_end(range.end())
    {}

    template<typename IteratorType>
    inline bool Range<IteratorType>::empty() const
    {
        return (m_begin == m_end);
    }

    template<typename IteratorType>
    inline typename Range<IteratorType>::size_type Range<IteratorType>::size() const
    {
        return static_cast<size_type>(std::distance(m_begin, m_end));
    }

    template<typename IteratorType>
    inline typename Range<IteratorType>::size_type Range<IteratorType>::capacity() const
    {
        return size();
    }
    
    template<typename IteratorType>
    inline typename Range<IteratorType>::size_type Range<IteratorType>::max_size() const
    {
        return size();
    }

    template<typename IteratorType>
    inline typename Range<IteratorType>::iterator Range<IteratorType>::begin() const
    {
        return m_begin;
    }

    template<typename IteratorType>
    inline typename Range<IteratorType>::iterator Range<IteratorType>::end() const
    {
        return m_end;
    }

    template<typename IteratorType>
    inline void Range<IteratorType>::swap(Range& other)
    {
        using std::swap;
        swap(m_begin, other.m_begin);
        swap(m_end,   other.m_end);
    }

    template<typename IteratorType>
    inline bool operator==(Range<IteratorType> const& lhs, Range<IteratorType> const& rhs)
    {
        return (lhs.begin() == rhs.begin()) && (lhs.end() == rhs.end());
    }

    template<typename IteratorType>
    inline bool operator!=(Range<IteratorType> const& lhs, Range<IteratorType> const& rhs)
    {
        return !(lhs == rhs);
    }
    
    template<typename IteratorType>
    inline Range<IteratorType> make_range(IteratorType first, IteratorType last)
    {
        return Range<IteratorType>(first, last);
    }

    template<typename IteratorType>
    inline void swap(Range<IteratorType>& lhs, Range<IteratorType>& rhs)
    {
        lhs.swap(rhs);
    }
}
}

#endif  // __LIBEMBER_UTIL_RANGE_HPP

