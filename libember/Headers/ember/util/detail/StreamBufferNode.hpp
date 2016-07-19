/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_DETAIL_STREAMBUFFERNODE_HPP
#define __LIBEMBER_UTIL_DETAIL_STREAMBUFFERNODE_HPP

#include <algorithm>
#include "../../meta/RemoveCV.hpp"
#include "../../meta/TransferCV.hpp"

namespace libember { namespace util { namespace detail
{

    /**
     * A node type representing an element of a linked list of consecutive
     * fixed size typed storage areas.
     */
    template<typename ValueType, unsigned short ChunkSize>
    class StreamBufferNode
    {
        public:
            typedef ValueType           value_type;

            typedef value_type&         reference;
            typedef value_type const&   const_reference;
            typedef value_type*         pointer;
            typedef value_type const*   const_pointer;

            typedef std::size_t         size_type;
            typedef unsigned short      cursor_type;

        public:
            /**
             * Default constructor that initializes all internal pointers
             * to null and leaves the internal storage area uninitialized.
             */ 
            StreamBufferNode();

            /**
             * Copy constructor that initializes the instance as a shallow
             * copy of @p other. In this context shallow means that only
             * the nodes contents and cursors are copied and the next-pointer
             * ins initialized with 0.
             */
            StreamBufferNode(StreamBufferNode const& other);

            /**
             * Return the maximum number of elements the node may store.
             * @return The maximum number of elements the node may store.
             */
            size_type capacity() const;

            /**
             * Return the current number of elements stored in this node.
             * @return The current number of elements stored in this node.
             */
            size_type size() const;

            /**
             * Return whether or not this node is currently empty.
             * @return True if this node is currently empty, otherwise false.
             */
            bool empty() const;

            /**
             * Return a reference to the next pointer of this node.
             * @return A reference to the next pointer of this node.
             */
            StreamBufferNode*& next();

            /**
             * Return the const-qualified next pointer of this node.
             * @return The const-qualified next pointer of this node.
             */
            StreamBufferNode const* next() const;

            /**
             * Return a reference to the cursor referring to the first
             * element of the range of valid elements within this node.
             * @return A reference to the cursor referring to the first
             *      element of the range of valid elements within this
             *      node.
             */
            cursor_type& first();

            /**
             * Return a cursor referring to the first element of the range
             * of valid elements within this node.
             * @return A cursor referring to the first element of the range
             *      of valid elements within this node.
             */
            cursor_type first() const;

            /**
             * Return a reference to the cursor referring to the element
             * one past the last element of the range of valid elements
             * within this node.
             * @return A reference to the cursor referring to the element
             *      one past the last element of the range of valid elements
             *      within this node.
             */
            cursor_type& last();

            /**
             * Return a cursor referring to the element one past the last
             * element of the range of valid elements within this node.
             * @return A cursor referring to the element one past the last
             *      element of the range of valid elements within this node.
             */
            cursor_type last() const;

            /**
             * Return a reference to the element stored at the cursor position
             * passed in @p pos.
             * @param pos a cursor referring to the element, to which a
             *      reference should be returned.
             * @return A reference to the requested element.
             * @note Please note that this method does not perform any bounds
             *      checking. It is the responsibility of the caller to assert
             *      that the specified cursor position refers to a valid
             *      element.
             */
            reference at(cursor_type pos);

            /**
             * Return the element stored at the cursor position passed
             * in @p pos.
             * @param pos a cursor referring to the element, to which a
             *      reference should be returned.
             * @return The value stored at the specified cursor position.
             * @note Please note that this method does not perform any bounds
             *      checking. It is the responsibility of the caller to assert
             *      that the specified cursor position refers to a valid
             *      element.
             */
            const_reference at(cursor_type pos) const;

            /**
             * Append a single element to the range of valid elements within
             * this node.
             * @param value the value that should be appended to the range of
             *      valid elements within this node.
             * @return True if enough space was available to append the element,
             *      otherwise false.
             */
            bool append(value_type value);

            /**
             * Append a sequence of elements to the range of valid elements within
             * this node.
             * @param first an input iterator referring to the first element in the
             *      sequence of elements to append.
             * @param last an input iterator referring to the first element one past
             *      the last element in the sequence of elements to append.
             * @return An input iterator referring to the first element in the
             *      sequence specified by @p first and @p last that was not appended
             *      to this range of valid elements within this node.
             */
            template<typename InputIterator>
            InputIterator append(InputIterator first, InputIterator last);

            /**
             * Consume up to @p howMany elements from the front of the sequence stored
             * in this node and return how many elements have been consumed.
             * @param howMany the number of elements that should be consume.
             * @return The number of elements that have been consumed.
             */
            size_type consume(size_type howMany);

        private:
            /**
             * Private unimplemented assignment operator to disallow
             * assignments from one node to another.
             */
            StreamBufferNode& operator=(StreamBufferNode const&);

        private:
            StreamBufferNode* m_next;
            cursor_type m_first;
            cursor_type m_last;

            enum
            {
                CAPACITY = (ChunkSize - sizeof(StreamBufferNode*) - sizeof(cursor_type) - sizeof(cursor_type)) / sizeof(value_type)
            };

            value_type m_data[CAPACITY];
    };



   /**************************************************************************/
   /* Mandatory inline implementation                                        */
   /**************************************************************************/

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferNode<ValueType, ChunkSize>::StreamBufferNode()
        : m_next(0), m_first(0), m_last(0)
    {}
    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferNode<ValueType, ChunkSize>::StreamBufferNode(StreamBufferNode const& other)
        : m_next(0), m_first(other.m_first), m_last(other.m_last)
    {
        std::copy(other.m_data, other.m_data + other.m_last, m_data);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::size_type StreamBufferNode<ValueType, ChunkSize>::capacity() const
    {
        using std::max;
        return max(0, CAPACITY - m_last);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::size_type StreamBufferNode<ValueType, ChunkSize>::size() const
    {
        using std::max;
        return static_cast<size_type>(max(0, m_last - m_first));
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline bool StreamBufferNode<ValueType, ChunkSize>::empty() const
    {
        return (m_first == m_last);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferNode<ValueType, ChunkSize>*& StreamBufferNode<ValueType, ChunkSize>::next()
    {
        return m_next;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferNode<ValueType, ChunkSize> const* StreamBufferNode<ValueType, ChunkSize>::next() const
    {
        return m_next;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::cursor_type& StreamBufferNode<ValueType, ChunkSize>::first()
    {
        return m_first;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::cursor_type StreamBufferNode<ValueType, ChunkSize>::first() const
    {
        return m_first;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::cursor_type& StreamBufferNode<ValueType, ChunkSize>::last()
    {
        return m_last;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::cursor_type StreamBufferNode<ValueType, ChunkSize>::last() const
    {
        return m_last;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::reference StreamBufferNode<ValueType, ChunkSize>::at(cursor_type pos)
    {
        return m_data[pos];
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferNode<ValueType, ChunkSize>::const_reference StreamBufferNode<ValueType, ChunkSize>::at(cursor_type pos) const
    {
        return m_data[pos];
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline bool StreamBufferNode<ValueType, ChunkSize>::append(value_type value)
    {
        bool const spaceAvailable = (m_last < CAPACITY);
        if (spaceAvailable)
            m_data[m_last++] = value;
        return spaceAvailable;
    }

    template<typename ValueType, unsigned short ChunkSize>
    template<typename InputIterator>
    inline InputIterator StreamBufferNode<ValueType, ChunkSize>::append(InputIterator first, InputIterator last)
    {
        while ((first != last) && (m_last < CAPACITY))
        {
            m_data[m_last] = *first;
            ++first;
            ++m_last;
        }
        return first;
    }

    template<typename ValueType, unsigned short ChunkSize>
    typename StreamBufferNode<ValueType, ChunkSize>::size_type StreamBufferNode<ValueType, ChunkSize>::consume(size_type howMany)
    {
        using std::min;
        cursor_type const toConsume = static_cast<cursor_type>(min(size(), howMany));
        m_first += toConsume;
        return toConsume;
    }
}
}
}


#endif  // __LIBEMBER_UTIL_DETAIL_STREAMBUFFERNODE_HPP


