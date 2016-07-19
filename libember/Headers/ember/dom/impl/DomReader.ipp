/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_IMPL_DOMREADER_IPP
#define __LIBEMBER_DOM_IMPL_DOMREADER_IPP

#include <memory>
#include <stdexcept>
#include "../../util/Inline.hpp"
#include "../VariantLeaf.hpp"
#include "../Set.hpp"
#include "../Sequence.hpp"
#include "../DomReader.hpp"
#include "../NodeFactory.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    DomReader::DomReader()
        : m_input(0), m_parentReader(0), m_length(0), m_outerLength(0)
        , m_bytesRead(0), m_bytesAvailable(0), m_handledEof(false), m_isContainer(false)
    {}

    LIBEMBER_INLINE
    DomReader::DomReader(DomReader* parentReader)
        : m_input(parentReader->m_input), m_parentReader(parentReader), m_length(0), m_outerLength(0)
        , m_bytesRead(0), m_bytesAvailable(parentReader->length()), m_handledEof(0), m_isContainer(false)
    {}

    LIBEMBER_INLINE
    Node* DomReader::decodeTree(util::OctetStream& input, NodeFactory const& factory)
    {
        m_input = &input;
        m_bytesAvailable = input.size();

        std::auto_ptr<Node> root;
        if (read())
        {
            root.reset(decodeNode(factory));

            if ((root.get() == 0) || !isContainer())
            {
                throw std::runtime_error("Root node is not a container");
            }
            DomReader tempReader(this);
            decodeTreeRecursive(tempReader, reinterpret_cast<Container*>(root.get()), factory);
        }

        m_input = 0;
        return root.release();
    }

    LIBEMBER_INLINE
    void DomReader::decodeTreeRecursive(DomReader& reader, Container* parent, NodeFactory const& factory)
    {
        while (reader.read())
        {
            std::auto_ptr<Node> node(reader.decodeNode(factory));
            if (node.get() != 0)
            {
                if (reader.isContainer())
                {
                    DomReader tempReader(&reader);
                    decodeTreeRecursive(tempReader, reinterpret_cast<Container*>(node.get()), factory);
                }

                parent->insert(parent->end(), node.get());
                node.release();
            }
        }
    }

    LIBEMBER_INLINE
    Node* DomReader::decodeNode(NodeFactory const& factory) 
    {
        ber::Type const type = ber::Type::fromTag(typeTag());
        ber::Tag const tag = this->applicationTag();

        if (!type.isApplicationDefined())
        {
            if (isContainer())
            {
                switch(type.value())
                {
                    case ber::Type::Set:
                        return new dom::Set(tag);

                    case ber::Type::Sequence:
                        return new dom::Sequence(tag);

                    default:
                        return 0;
                }
            }
            else
            {
                switch(type.value())
                {
                    case ber::Type::Boolean:
                        return new VariantLeaf(tag, decode<bool>());

                    case ber::Type::Integer:
                        if (length() > 4)
                            return new VariantLeaf(tag, decode<long>());
                        else
                            return new VariantLeaf(tag, decode<int>());

                    case ber::Type::Real:
                        return new VariantLeaf(tag, decode<double>());

                    case ber::Type::UTF8String:
                        return new VariantLeaf(tag, decode<std::string>());

                    case ber::Type::RelativeObject:
                        return new VariantLeaf(tag, decode<ber::ObjectIdentifier>());

                    case ber::Type::OctetString:
                        return new dom::VariantLeaf(tag, decode<ber::Octets>());

                    default:
                        skipCurrentItem();
                        break;
                }
                return 0;
            }
        }
        else
        {
            return factory.createApplicationDefinedNode(type, tag);
        }
    }

    LIBEMBER_INLINE
    DomReader::value_type DomReader::readByte()
    {
        value_type const value = m_input->front();
        m_input->consume();

        return value;
    }

    LIBEMBER_INLINE
    void DomReader::disposeCurrentTLV()
    {
        m_applicationTag = ber::make_tag(0, 0);
        m_length = 0;
        m_outerLength = 0;
        m_isContainer = false;
        m_typeTag = ber::make_tag(0, 0);
    }

    LIBEMBER_INLINE
    bool DomReader::eof() const
    {
        return (m_bytesRead >= m_bytesAvailable && (m_bytesAvailable != length_type::INDEFINITE)) || m_input->empty();
    }

    LIBEMBER_INLINE
    void DomReader::handleEof()
    {
        if (m_handledEof == false)
        {   
            m_handledEof = true;

            if (m_parentReader != 0)
                m_parentReader->m_bytesRead += m_bytesRead;
        }
    }

    LIBEMBER_INLINE
    void DomReader::skipCurrentItem()
    {
        m_input->consume(m_length);
    }

    LIBEMBER_INLINE
    bool DomReader::read()
    {
        disposeCurrentTLV();

        if (m_parentReader != 0 && eof())
        {
            handleEof();
            return false;
        }

        m_applicationTag = decodeTag();

        if (m_applicationTag.number() == 0 && m_applicationTag.preamble() == 0)
        {
            // Terminator of indefinite length field
            if (readByte() == 0 && readByte() == 0 && readByte() == 0)
            {
                handleEof();
                return false;
            }
            else
            {
                throw std::runtime_error("Invalid zero tag: 0x00");
            }
        }

        m_outerLength = decodeLength().value;

        if (m_outerLength == 0)
        {
            throw std::runtime_error("Zero outer length");
        }
        
        if (!(m_applicationTag.isContainer() && m_applicationTag.getClass() != ber::Class::Universal))
        {
            throw std::runtime_error("Implicit tag or universal outer tag found");
        }

        m_typeTag = decodeTag();
        m_length = decodeLength().value;

        if ((m_length == length_type::INDEFINITE) != (m_outerLength == length_type::INDEFINITE))
        {
            throw std::runtime_error("Outer and inner tag must use the same length form");
        }

        m_isContainer = m_typeTag.isContainer();
        m_applicationTag.setContainer(false);
        m_typeTag.setContainer(false);

        if (m_length == length_type::INDEFINITE && !m_isContainer)
        {
            throw std::runtime_error("Indefinite length form is only allowed on containers");
        }
        return true;
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_DOMREADER_IPP
