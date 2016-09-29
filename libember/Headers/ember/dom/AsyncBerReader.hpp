/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_ASYNCBERREADER_HPP
#define __LIBEMBER_DOM_ASYNCBERREADER_HPP

#include <memory>
#include <deque>
#include "../util/Api.hpp"
#include "../util/OctetStream.hpp"
#include "../ber/Encoding.hpp"
#include "../ber/Length.hpp"

namespace libember { namespace dom
{
    class Container;
    class Node;
    class NodeFactory;

    /**
     * Base class for an asynchronous ember reader
     */
    class LIBEMBER_API AsyncBerReader
    {
        class LIBEMBER_API DecodeState;
        class LIBEMBER_API AsyncContainer;

        public:
            typedef util::OctetStream::size_type size_type;
            typedef util::OctetStream::value_type value_type;
            typedef ber::Length<size_type> length_type;

            /** Virtual destructor */
            virtual ~AsyncBerReader();

            /**
             * Resets the state of the reader
             */
            void reset();

            /**
             * Reads a single byte.
             * @param value The byte to decode.
             * @throw std::runtime_error if the end of a stream is reached while still decoding
             *      a container; when there is a mismatch with the decoded tag or length or
             *      when a memory exception occurs.
             */
            void read(value_type value);

            /*
             * Decodes the provided bytes.
             * @param first an iterator referring the first element of the sequence
             *        of elements to decode.
             * @param last an iterator referring to the element one past the last
             *        element of the sequence of elements to decode.
             * @throw std::runtime_error if the end of a stream is reached while still decoding
             *      a container; when there is a mismatch with the decoded tag or length or
             *      when a memory exception occurs.
             */
            template<typename InputIterator>
            void read(InputIterator first, InputIterator last);

        protected:
            /** Constructor */
            AsyncBerReader();

            /**
             * This abstract method is called when a new container has been 
             * completely decoded.
             */
            virtual void containerReady() = 0;

            /**
             * This abstract method is called when a new node has been
             * completely decoded.
             */
            virtual void itemReady() = 0;

            /**
             * This method is called by reset and may be overridden by the implementation.
             */
            virtual void resetImpl();

            /**
             * Returns the encoded length of the node currently being decoded.
             * @return The encoded length of the node currently being decoded.
             */
            size_type length() const;

            /**
             * Returns true if the current node is a container.
             * @return true, if the current node is a container.
             */
            bool isContainer() const;

            /**
             * Decodes a value from the value buffer.
             */
            template<typename ValueType>
            ValueType decode();

            /**
             * Creates a new node from the current buffer.
             * @param factory The application defined node factory.
             * @return A pointer to the newly created dom node.
             */
            dom::Node* decodeNode(dom::NodeFactory const& factory);

        private:
            /**
             * Decodes a tag. This method is called when the current decoding state 
             * is Tag.
             * @param value The current byte to decode.
             */
            bool readTagByte(value_type value);

            /**
             * Decodes a length. This method is called when the current decoding state 
             * is Length.
             * @param value The current byte to decode.
             */
            bool readLengthByte(value_type value);

            /**
             * Decodes a value. This method is called when the current decoding state 
             * is Value.
             * @param value The current byte to decode.
             */
            bool readValueByte(value_type value);

            /**
             * Decodes the terminator of a container with indefinite length. This method is called 
             * when the current decoding state is Terminator.
             * @param value The current byte to decode.
             */
            bool readTerminatorByte(value_type value);

            /**
             * Resets the state of the decoder, including the current buffer.
             * @param state The new decoding state to set.
             */
            void reset(DecodeState const& state);

            /**
             * Preloads a value by copying the current buffer into a 
             * value buffer. Resets the decoder state afterwards.
             */
            void preloadValue();

            /**
             * Pushes a new AsyncContainer on the stack with the current application tag,
             * type tag and encoded length property.
             * @throw std::runtime_error when the push operation fails for any reason.
             */
            void pushContainer();

            /**
             * Pops the current container from the stack, assigns the application tag, the 
             * type tag, the length and the container flag to this reader instance, and
             * invokes the itemReady method.
             * When no container is currently stored in the stack, this method returns false.
             */
            bool popContainer();

            /**
             * Resets all fields storing the tag, length and value.
             */
            void disposeCurrentTLV();

        private:
            /**
             * A scoped enumeration type containing the symbolic names
             * for the various decoding states of the async reader.
             */
            class DecodeState
            {
                public:
                    enum _Domain
                    {
                        Tag,
                        Length,
                        Value,
                        Terminator
                    };

                    typedef std::size_t value_type;

                    /**
                     * Non-explicit single argument constructor to allow implicit conversion.
                     * @param value the value from _Domain with which to initialize this
                     *      instance.
                     */
                    DecodeState(_Domain value)
                        : m_value(value)
                    {}

                    /**
                     * Return the raw value currently represented by this instance.
                     * @return The raw value currently represented by this instance.
                     */
                    value_type value() const
                    {
                        return m_value;
                    }

                private:
                    value_type m_value;
            };

            /**
             * Represents a pending container that is currently being read.
             */
            class AsyncContainer
            {
                public:
                    typedef size_t size_type;

                    /**
                     * Initializes a new AsyncContainer.
                     * @param tag The application tag of the container.
                     * @param type The type tag of the container.
                     * @param length The encoded length of the container.
                     */
                    AsyncContainer(ber::Tag const& tag, ber::Tag const& type, size_type length)
                        : m_tag(tag)
                        , m_type(type)
                        , m_length(length)
                        , m_bytesRead(0)
                    {}

                    /**
                     * Increments the bytesRead property of this instance by the provided amount.
                     * @param amount Amount to add to the current value of bytesRead.
                     */
                    void incrementBytesRead(size_type amount = 1)
                    {
                        m_bytesRead += amount;
                    }

                    /**
                     * Sets the encoded length property of this container.
                     * @param length The new length value to set.
                     */ 
                    void setLength(size_type length)
                    {
                        m_length = length;
                    }

                    /**
                     * Returns the number of bytes that have already been read by this container.
                     * @return The number of bytes read.
                     */
                    size_type bytesRead() const
                    {
                        return m_bytesRead;
                    }

                    /**
                     * Tests if all bytes of this container have already been read.
                     * @return True if all bytes have been read, false if there are
                     *      still bytes available.
                     */
                    bool eof() const
                    {
                        return m_bytesRead >= m_length 
                            && m_length != libember::ber::Length<size_type>::INDEFINITE;
                    }

                    /**
                     * Returns the application tag of this container.
                     * @return The application tag of this container.
                     */
                    ber::Tag const& tag() const
                    {
                        return m_tag;
                    }

                    /**
                     * Returns the type tag of this container.
                     * @return The type tag of this container.
                     */
                    ber::Tag const& type() const
                    {
                        return m_type;
                    }

                    /**
                     * Returns the encoded length of this container.
                     * @return The encoded length of this container.
                     */
                    size_type length() const
                    {
                        return m_length;
                    }

                private:
                    ber::Tag m_tag;
                    ber::Tag m_type;
                    size_type m_length;
                    size_type m_bytesRead;
            };

            typedef std::deque<AsyncContainer> AsyncContainerStack;

        private:
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif
            AsyncContainerStack m_stack;
            util::OctetStream m_buffer;
            util::OctetStream m_valueBuffer;
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
            DecodeState m_decodeState;
            size_type m_bytesRead;
            size_type m_bytesExpected;
            size_type m_valueLength;
            bool m_isContainer;
            ber::Tag m_appTag;
            ber::Tag m_typeTag;
            size_type m_length;
            size_type m_outerLength;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void AsyncBerReader::read(InputIterator first, InputIterator last)
    {
        for( /* Nothing */; first != last; ++first)
        {
            read(*first);
        }
    }

    template<typename ValueType>
    inline ValueType AsyncBerReader::decode()
    {
        ValueType const value = ber::decode<ValueType>(m_valueBuffer, m_valueLength);
        return value;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/AsyncBerReader.ipp"
#endif

#endif  // __LIBEMBER_DOM_ASYNCBERREADER_HPP

