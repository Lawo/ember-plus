/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_DETAIL_STREAMBUFFERITERATOR_HPP
#define __LIBEMBER_UTIL_DETAIL_STREAMBUFFERITERATOR_HPP

#include <iterator>
#include "../../meta/RemoveCV.hpp"
#include "../../meta/TransferCV.hpp"
#include "StreamBufferNode.hpp"

//SimianIgnore

namespace libember { namespace util
{
    /** Forward declaration. */
    template<typename ValueType, unsigned short ChunkSize>
    class StreamBuffer;
}
}

namespace libember { namespace util { namespace detail
{
    /** Forward declaration. */
    template<typename ValueType, unsigned short ChunkSize>
    class StreamBufferIterator;
    
    /**
     * Equality comparison operator for StreamBufferIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise false.
     */
    template<typename ValueType, unsigned short ChunkSize>
    bool operator==(StreamBufferIterator<ValueType, ChunkSize> const& lhs, StreamBufferIterator<ValueType, ChunkSize> const& rhs);

    /**
     * Inequality comparison operator for StreamBufferIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return False if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise true.
     */
    template<typename ValueType, unsigned short ChunkSize>
    bool operator!=(StreamBufferIterator<ValueType, ChunkSize> const& lhs, StreamBufferIterator<ValueType, ChunkSize> const& rhs);

    /** 
     * Const iterator class, used to iterate through the current elements of the output stream
     */
    template<typename ValueType, unsigned short ChunkSize>
    class StreamBufferIterator
    {
        public:
            typedef std::forward_iterator_tag                   iterator_category;
            typedef typename meta::RemoveConst<ValueType>::type value_type;
            typedef ValueType&                                  reference;
            typedef ValueType*                                  pointer;
            typedef std::ptrdiff_t                              difference_type;

            typedef typename meta::TransferConst<
                        ValueType, StreamBufferNode<value_type, ChunkSize>
                    >::type                                     node_type;
            typedef typename node_type::cursor_type             cursor_type;

        public:
            /**
             * Default constructor. Initializes the instance in a singular state.
             */
            StreamBufferIterator();

            /**
             * Constructor, creates a new iterator instance at the specified position.
             * @param node a pointer to the node to start from
             * @param position the position within the node's data. Note that this
             *      value must be a position between the nodes first and last item.
             */
            StreamBufferIterator(node_type* node, cursor_type position);

            /**
             * Overloaded pre increment operator. Advances the current iterator
             * position by one so that it refers to the element following the
             * current element in the controlled sequence. If this iterator is
             * in a singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A reference to this instance.
             */
            StreamBufferIterator& operator++();

            /**
             * Post-increment operator. Returns a copy of the current state and moves
             * to the next position in the stream.
             * @return Returns an iterator pointing to the current position in the stream
             */
            StreamBufferIterator operator++(int);

            /**
             * Dereference operator. 
             * @return Returns the value stored at the current position.
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

        protected:
            friend class StreamBuffer<ValueType, ChunkSize>;
            friend bool operator==<>(StreamBufferIterator<ValueType, ChunkSize> const&, StreamBufferIterator<ValueType, ChunkSize> const&);

            /**
             * Helper method for friend functions and classes.
             * Return the node that contains the element this iterator refers to.
             * @return The node that contains the element this iterator
             *      refers to.
             */
            node_type* node() const
            {
                return m_node;
            }

            /**
             * Helper method for friend functions and classes.
             * Return the cursor specifying the element this iterator refers to.
             * @return The cursor specifying the element this iterator
             *      refers to.
             */
            cursor_type cursor() const
            {
                return m_position;
            }

        private:
            node_type* m_node;
            cursor_type m_position;
    };



    /**************************************************************************/
    /* Mandatory inline implementation                                         */
    /**************************************************************************/

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferIterator<ValueType, ChunkSize>::StreamBufferIterator()
        : m_node(0), m_position(0)
    {}

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferIterator<ValueType, ChunkSize>::StreamBufferIterator(node_type* head, cursor_type cursor)
        : m_node(head), m_position(cursor)
    {}

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferIterator<ValueType, ChunkSize>::reference StreamBufferIterator<ValueType, ChunkSize>::operator*() const
    {
        return m_node->at(m_position);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBufferIterator<ValueType, ChunkSize>::pointer StreamBufferIterator<ValueType, ChunkSize>::operator->() const
    {
        return &m_node->at(m_position);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferIterator<ValueType, ChunkSize>& StreamBufferIterator<ValueType, ChunkSize>::operator++()
    {
        if (m_node != 0)
        {
            m_position += 1;
            if (m_position >= m_node->last())
            {
                m_node = m_node->next();
                m_position = (m_node != 0) ? m_node->first() : 0;
            }
        }
        return *this;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBufferIterator<ValueType, ChunkSize> StreamBufferIterator<ValueType, ChunkSize>::operator++(int)
    {
        StreamBufferIterator const current(*this);
        ++(*this);
        return current;
    }

    template<typename ValueType, unsigned short ChunkSize>
    bool operator==(StreamBufferIterator<ValueType, ChunkSize> const& lhs, StreamBufferIterator<ValueType, ChunkSize> const& rhs) 
    {
        return (lhs.node() == rhs.node()) && (lhs.cursor() == rhs.cursor());
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline bool operator!=(StreamBufferIterator<ValueType, ChunkSize> const& lhs, StreamBufferIterator<ValueType, ChunkSize> const& rhs) 
    {
        return !(lhs == rhs);
    }

}
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_UTIL_DETAIL_STREAMBUFFERITERATOR_HPP

