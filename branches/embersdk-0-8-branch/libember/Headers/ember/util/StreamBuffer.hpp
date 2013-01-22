#ifndef __LIBEMBER_UTIL_STREAMBUFFER_HPP
#define __LIBEMBER_UTIL_STREAMBUFFER_HPP

#include "detail/StreamBufferIterator.hpp"

namespace libember { namespace util
{
    /**
     * A simple generic container that manages a queue of elements without the
     * overhead of a simple deque. It allows appending elements to the back of
     * the controlled sequence, forward iteration over all elements in the
     * sequence and consuming elements from the front of the sequence.
     */
    template<typename ValueType, unsigned short ChunkSize = 256>
    class StreamBuffer
    {
        public:
            typedef ValueType         value_type;

            typedef std::size_t       size_type;
            typedef value_type&       reference;
            typedef value_type const& const_reference;
            typedef value_type*       pointer;
            typedef value_type const* const_pointer;

            typedef detail::StreamBufferIterator<ValueType, ChunkSize>          iterator;
            typedef detail::StreamBufferIterator<ValueType const, ChunkSize>    const_iterator;

        public:
            /** Default constructor, initializes an empty stream buffer. */
            StreamBuffer();

            /**
             * Copy constructor that initializes the instance with a copy
             * of the buffer contents of @p other.
             * @param other a constant reference to the StreamBuffer instance
             *      whose buffer contents should be copied.
             */
            StreamBuffer(StreamBuffer const& other);

            /** Destructor. Releases memory allocated to the controlled sequence. */
            ~StreamBuffer();

            /**
             * Clear the stream buffer. Releasing all memory allocated to the
             * controlled sequence.
             */
            void clear();

            /**
             * Returns whether or not the buffer is currently empty.
             * @return True if if the buffer is currently empty, otherwise false.
             */
            bool empty() const;

            /**
             * Returns the current number of items stored in the buffer.
             * @return The current number of items stored in the buffer.
             */
            size_type size() const;

            /**
             * Returns the first item in the stream
             * @return The value of the first item. If the stream buffer is
             *      currently empty the behaviour of his method is undefined.
             */
            value_type front() const;

            /**
             * Returns a constant iterator referring to the first element of
             * the stream buffer.
             * @return A constant iterator referring to the first element of
             *      the stream buffer.
             */
            const_iterator begin() const;

            /**
             * Return an iterator referring to the first element of the
             * stream buffer.
             * @return An iterator referring to the first element of the
             *      stream buffer.
             */
            iterator begin();

            /**
             * Return a constant iterator referring to the element one past
             *      the last element of the stream buffer.
             * @return A constant iterator referring to the element one past
             *      the last element of the stream buffer.
             */
            const_iterator end() const;

            /**
             * Return an iterator referring to the element one past the last
             * element of the stream buffer.
             * @return An iterator referring to the element one past the last
             *      element of the stream buffer.
             */
            iterator end();

            /**
             * Appends a sequence of elements referred to by @p first and @p last
             * to the back of this buffer.
             * @param first an iterator referring the first element of the sequence
             *        of elements to add.
             * @param last an iterator referring to the element one past the last
             *        element of the sequence of elements to add.
             */
            template<typename InputIterator>
            void append(InputIterator first, InputIterator last);

            /**
             * Appends a single element to the back of this buffer.
             * @param value the value to add.
             */
            void append(value_type value);

            /** 
             * Removes the specified number of elements from the front of
             * this stream.
             * @param howMany the number of items to remove. This parameter is
             *      defaulted to 1, meaning only a single element will be
             *      removed, when this method is called without any arguments.
             * @return The number of items that have been removed. This value
             *      less than or equal to @p howMany. 
             */
            size_type consume(size_type howMany = 1);

            /**
             * Removes all elements from the beginning up to the position
             * provided via the iterator.
             * @param last an iterator referring the first element in this buffer
             *      that should not be consumed.
             * @return The number of elements that were removed from this buffer.
             */
            size_type consume(iterator last);

            /**
             * Exchange the contents of this stream buffer with those of
             * @p other. This operation is guaranteed not to throw an exception.
             * @param other a reference to the stream buffer instance the contents
             *      of this instance should be exchanged with.
             */
            void swap(StreamBuffer& other);

            /**
             * Overloaded assignment operator. Copies the buffer contents of
             * @p other into this instance.
             * @param other the Stream buffer whose contents to copy.
             * @return A reference referring to this instance.
             */
            StreamBuffer& operator=(StreamBuffer other);

        private:
            typedef detail::StreamBufferNode<ValueType, ChunkSize> node_type;

        private:
            /**
             * Helper method that makes sure that the buffer contains at
             * least one node item.
             * @return True if a new node was created, otherwise false.
             */
            bool assureHead();

            /**
             * Append a new node to the tail of the current chunk list and
             * return it.
             * @return A pointer to the newly allocated node.
             */
            node_type* grow();

            /**
             * Remove the current head node from the current chunk list and
             * return a pointer to the new head node.
             * @return A pointer to the new head node.
             */
            node_type* shrink();

            /** 
             * Calls the allocator's allocate method in order to allocate memory for a new chunk.
             * @return Returns the allocated node.
             */
            static node_type* allocate();

            /**
             * Helper function that encapsulates the underlying method
             * of destroying and deallocating a node instance dynamically
             * allocated through a call to allocate.
             * @param node a pointer to the node that should be destroyed
             *      and deallocated.
             */
            static void deallocate(node_type* node);

        private:
            node_type *m_head;
            node_type *m_tail;
   };


   /**************************************************************************/
   /* Mandatory inline implementation                                        */
   /**************************************************************************/

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBuffer<ValueType, ChunkSize>::StreamBuffer()
        : m_head(0), m_tail(0)
    {}

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBuffer<ValueType, ChunkSize>::StreamBuffer(StreamBuffer const& other)
        : m_head(0), m_tail(0)
    {
        node_type const* currentSource = other.m_head;
        while (currentSource != 0)
        {
            node_type* const newNode = new node_type(*currentSource);
            if (m_head != 0)
            {
                m_head->next() = newNode;
            }
            else
            {
                m_head = newNode;
            }
            m_tail = newNode;
            currentSource = currentSource->next();
        }
    }


    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBuffer<ValueType, ChunkSize>::~StreamBuffer()
    {
        for (node_type* it = m_head; it != 0; /* Nothing */)
        {
            node_type* const current = it;
            it = current->next();
            deallocate(current);
        }
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline void StreamBuffer<ValueType, ChunkSize>::clear()
    {
        StreamBuffer empty;
        swap(empty);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline bool StreamBuffer<ValueType, ChunkSize>::empty() const
    {
        return (m_head == 0);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::size_type StreamBuffer<ValueType, ChunkSize>::size() const
    {
        size_type result = 0;
        for (node_type const* current = m_head; current != 0; current = current->next())
        {
            result += current->size();
        }
        return result;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline void StreamBuffer<ValueType, ChunkSize>::append(value_type value)
    {
        assureHead();
        if (!m_tail->append(value))
        {
            grow()->append(value);
        }
    }

    template<typename ValueType, unsigned short ChunkSize>
    template<typename InputIterator>
    inline void StreamBuffer<ValueType, ChunkSize>::append(InputIterator first, InputIterator last)
    {
        assureHead();
        InputIterator current = m_tail->append(first, last);
        while (current != last)
        {
            current = grow()->append(current, last);
        }
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::size_type StreamBuffer<ValueType, ChunkSize>::consume(size_type howMany)
    {
        size_type toConsume = howMany;
        while ((m_head != 0) && (toConsume > 0))
        {
            toConsume -= m_head->consume(toConsume); 
            if (m_head->empty())
            {
                shrink();
            }
        }
        return (howMany - toConsume);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::size_type StreamBuffer<ValueType, ChunkSize>::consume(iterator last)
    {
        size_type consumed = 0;
        while ((m_head != 0) && (m_head != last.node()))
        {
            consumed += m_head->size();
            shrink();
        }
        if (m_head != 0)
        {
            using std::min;
            typename iterator::cursor_type const endCursor = min(last.cursor(), m_head->last());
            if (endCursor > m_head->first())
            {
                size_type const remainder = endCursor - m_head->first();
                consumed += m_head->consume(remainder);
            }
            if (m_head->empty())
            {
                shrink();
            }
        }
        return consumed;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline void StreamBuffer<ValueType, ChunkSize>::swap(StreamBuffer& other)
    {
        using std::swap;
        swap(m_head, other.m_head);
        swap(m_tail, other.m_tail);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline StreamBuffer<ValueType, ChunkSize>& StreamBuffer<ValueType, ChunkSize>::operator=(StreamBuffer other)
    {
        swap(other);
        return *this;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::value_type StreamBuffer<ValueType, ChunkSize>::front() const
    {
        return m_head->at(m_head->first());
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::const_iterator StreamBuffer<ValueType, ChunkSize>::begin() const
    {
        node_type const* const head = m_head;
        return const_iterator(head, (head != 0) ? head->first() : 0);
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::iterator StreamBuffer<ValueType, ChunkSize>::begin() 
    {
        return (m_head != 0) ? iterator(m_head, m_head->first()) : end();
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::const_iterator StreamBuffer<ValueType, ChunkSize>::end() const
    {
        return const_iterator();
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::iterator StreamBuffer<ValueType, ChunkSize>::end()
    {
        return iterator();
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline bool StreamBuffer<ValueType, ChunkSize>::assureHead()
    {
        bool const createHead = (m_head == 0);
        if (createHead)
        {
            m_head = allocate();
            m_tail = m_head;
        }
        return createHead;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::node_type* StreamBuffer<ValueType, ChunkSize>::grow()
    {
        if (!assureHead())
        {
            node_type* const newNode = allocate();
            m_tail->next() = newNode;
            m_tail = newNode;
        }
        return m_tail;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::node_type* StreamBuffer<ValueType, ChunkSize>::shrink()
    {
        if (m_head != 0)
        {
            node_type* const detached = m_head;
            m_head = m_head->next();
            deallocate(detached);
            if (m_head == 0)
            {
                m_tail = 0;
            }
        }
        return m_head;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline void StreamBuffer<ValueType, ChunkSize>::deallocate(node_type* node)
    {
        delete node;
    }

    template<typename ValueType, unsigned short ChunkSize>
    inline typename StreamBuffer<ValueType, ChunkSize>::node_type* StreamBuffer<ValueType, ChunkSize>::allocate()
    {
        return new node_type();
    }
}
}

#endif  // __LIBEMBER_UTIL_STREAMBUFFER_HPP

