/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_DOMREADER_HPP
#define __LIBEMBER_DOM_DOMREADER_HPP

#include "../util/Api.hpp"
#include "../ber/Encoding.hpp"
#include "../ber/Length.hpp"

namespace libember { namespace dom
{
    class Container;
    class Node;
    class NodeFactory;

    /**
     * DomReader class which can be used to decode an encoded object tree.
     */ 
    class LIBEMBER_API DomReader
    {
        public:
            typedef util::OctetStream::size_type size_type;
            typedef util::OctetStream::value_type value_type;
            typedef ber::Length<size_type> length_type;

            /**
             * Constructor
             */
            DomReader();

            /**
             * Constructs a node structure from an octet stream. The stream must contain
             * the complete data of the previously encoded structure. The returned root node
             * must be deleted manually when it is no longer needed.
             * @param input Input stream containing the encoded data.
             * @param factory The node factory to use when a new item has been read.
             * @throw std::runtime_error when the decoded root node is not a container.
             */
            Node* decodeTree(util::OctetStream& input, NodeFactory const& factory);

        private:
            /**
             * Private constructor which is used to create a reader that decodes the 
             * content of a container node.
             * @param parentReader The parent reader that detected a container node.
             */
            explicit DomReader(DomReader* parentReader);

            /**
             * Decodes a single node. Uses the provided NodeFactory interface if
             * the decoded universal tag contains an application defined type.
             * @param factory Implementation of a NodeFactory which is able to
             *      create application defined types.
             * @return Returns the created node or null, if the current type is unknown.
             */
            Node* decodeNode(NodeFactory const& factory);

            /**
             * Decodes a value. This method uses the previously decoded (inner) length.
             * The caller must assure that the provide value type matches the encoded type.
             * @return The decoded value.
             */
            template<typename ValueType>
            ValueType decode();

            /**
             * Decodes a tag.
             * @return The decoded tag.
             */
            ber::Tag decodeTag();

            /**
             * Decodes a length.
             * @return The decoded length.
             */
            length_type decodeLength();

            /**
             * Returns the decoded application tag.
             * @return The decoded application tag.
             */
            ber::Tag applicationTag() const;

            /**
             * Returns the decoded type tag.
             * @return The decoded type tag.
             */
            ber::Tag typeTag() const;

            /**
             * Returns the decoded inner length.
             * @return The decoded inner length.
             */
            size_type length() const;

            /**
             * Returns true if the decoded type is a container.
             * @return True if the decoded type is a container, otherwise false.
             */
            bool isContainer() const;

            /**
             * Reads the total length, the application tag, the inner length and the type tag
             * of the next item. Returns true if a valid item has been detected.
             * @return True if another item can be decoded, otherwise false.
             * @throw std::runtime_error when an invalid tag or length has been detected.
             */
            bool read();

            /**
             * Checks for the end of the current stream.
             * @return True if more data is available, otherwise false.
             */
            bool eof() const;

            /**
             * Skips the decoding of the current item. This method is called when 
             * a type is unknown.
             */
            void skipCurrentItem();

            /**
             * Resets the current object information.
             */
            void disposeCurrentTLV();

            /**
             * Handles the end of stream.
             */
            void handleEof();

            /**
             * Reads a single byte and returns its value.
             * @return The value of the byte read.
             */
            value_type readByte();

            /**
             * Static method used to recursively decode a tree.
             * @param reader Current reader.
             * @param parent The parent container where newly created objects will be inserted to.
             * @param factory Node factory.
             */
            static void decodeTreeRecursive(DomReader& reader, Container* parent, NodeFactory const& factory);

        private:
            util::OctetStream* m_input;
            DomReader* m_parentReader;
            size_type m_length;
            size_type m_outerLength;
            size_type m_bytesRead;
            size_type m_bytesAvailable;
            ber::Tag m_applicationTag;
            ber::Tag m_typeTag;
            bool m_handledEof;
            bool m_isContainer;
    };
    
    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline ValueType DomReader::decode()
    {
        ValueType const value = ber::decode<ValueType>(*m_input, m_length);

        m_bytesRead += m_length;
        return value;
    }
    
    inline ber::Tag DomReader::decodeTag()
    {
        ber::Tag const value = ber::decode<ber::Tag>(*m_input);

        m_bytesRead += ber::encodedLength(value);
        return value;
    }
    
    inline DomReader::length_type DomReader::decodeLength()
    {
        length_type const value = ber::decode<length_type>(*m_input);

        m_bytesRead += ber::encodedLength(value);
        return value;
    }

    inline bool DomReader::isContainer() const
    {
        return m_isContainer;
    }

    inline ber::Tag DomReader::applicationTag() const
    {
        return m_applicationTag;
    }

    inline ber::Tag DomReader::typeTag() const
    {
        return m_typeTag;
    }

    inline DomReader::size_type DomReader::length() const
    {
        return m_length;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/DomReader.ipp"
#endif

#endif  // __LIBEMBER_DOM_DOMREADER_HPP

