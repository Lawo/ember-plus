/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_IMPL_ASYNCBERREADER_IPP
#define __LIBEMBER_DOM_IMPL_ASYNCBERREADER_IPP

#include <stdexcept>
#include "../../util/Inline.hpp"
#include "../VariantLeaf.hpp"
#include "../Set.hpp"
#include "../Sequence.hpp"
#include "../NodeFactory.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    AsyncBerReader::AsyncBerReader()
        : m_buffer(0)
        , m_valueBuffer(0)
        , m_decodeState(DecodeState::Tag)
        , m_bytesRead(0)
        , m_bytesExpected(0)
        , m_valueLength(0)
        , m_isContainer(false)
        , m_length(0)
        , m_outerLength(0)
    {}

    LIBEMBER_INLINE
    AsyncBerReader::~AsyncBerReader()
    {
        reset();
    }

    LIBEMBER_INLINE
    void AsyncBerReader::reset()
    {
        {
            AsyncContainerStack emptyContainerStack;
            emptyContainerStack.swap(m_stack);
        }
        disposeCurrentTLV();
        reset(DecodeState::Tag);
        resetImpl();
    }

    LIBEMBER_INLINE
    void AsyncBerReader::read(value_type value)
    {
        m_buffer.append(value);

        if (!m_stack.empty())
        {
            AsyncContainer& currentContainer = m_stack.back();
            currentContainer.incrementBytesRead();
        }

        bool isEofOk = false;

        switch(m_decodeState.value())
        {
            case DecodeState::Tag:
                isEofOk = readTagByte(value);
                break;

            case DecodeState::Length:
                isEofOk = readLengthByte(value);
                break;

            case DecodeState::Value:
                isEofOk = readValueByte(value);
                break;

            case DecodeState::Terminator:
                isEofOk = readTerminatorByte(value);
                break;
        }

        while (!m_stack.empty() && m_stack.back().eof())
        {
            if (!isEofOk)
            {
                throw std::runtime_error("Unexpected end of container");
            }
            popContainer();
        }
    }

    LIBEMBER_INLINE
    void AsyncBerReader::resetImpl()
    {}

    LIBEMBER_INLINE
    AsyncBerReader::size_type AsyncBerReader::length() const
    {
        return m_length;
    }

    LIBEMBER_INLINE
    bool AsyncBerReader::isContainer() const
    {
        return m_isContainer;
    }

    LIBEMBER_INLINE
    dom::Node* AsyncBerReader::decodeNode(dom::NodeFactory const& factory)
    {
        ber::Type const type = ber::Type::fromTag(m_typeTag);
        ber::Tag const tag = m_appTag;

        if (!type.isApplicationDefined())
        {
            if (m_isContainer)
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
                        return new dom::VariantLeaf(tag, decode<bool>());

                    case ber::Type::Integer:
                        if (m_length > 4)
                            return new dom::VariantLeaf(tag, decode<long>());
                        else
                            return new dom::VariantLeaf(tag, decode<int>());

                    case ber::Type::Real:
                        return new dom::VariantLeaf(tag, decode<double>());

                    case ber::Type::UTF8String:
                        return new dom::VariantLeaf(tag, decode<std::string>());

                    case ber::Type::RelativeObject:
                        return new dom::VariantLeaf(tag, decode<ber::ObjectIdentifier>());

                    case ber::Type::OctetString:
                        return new dom::VariantLeaf(tag, decode<ber::Octets>());

                    case ber::Type::Null:
                        return new dom::VariantLeaf(tag, decode<ber::Null>());

                    default:
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
    bool AsyncBerReader::readTagByte(value_type value)
    {
        if (value == 0 && m_bytesRead == 0)
        {
            reset(DecodeState::Terminator);
            return false;
        }

        if (m_bytesRead > 12)
            throw std::runtime_error("Number of tag octets out of bounds");

        if ((m_bytesRead == 0 && (value & 0x1F) != 0x1F)
        ||  (m_bytesRead > 0 && (value & 0x80) == 0))
        {
            if (m_appTag.number() == 0 && m_appTag.preamble() == 0)
            {
                ber::Tag const tag = libember::ber::decode<ber::Tag>(m_buffer);

                m_appTag = tag;
                m_appTag.setContainer(false);
            }
            else
            {
                ber::Tag const tag = libember::ber::decode<ber::Tag>(m_buffer);

                m_isContainer = tag.isContainer();
                m_typeTag = tag;
            }

            reset(DecodeState::Length);
            return false;
        }

        ++m_bytesRead;
        return false;
    }

    LIBEMBER_INLINE
    bool AsyncBerReader::readLengthByte(value_type value)
    {
        if (m_bytesExpected == 0)
        {
            if ((value & 0x80) != 0)
            {
                m_bytesExpected = (value & 0x7F) + 1;
            }
            else
            {
                m_bytesExpected = 1;
            }

            if (m_bytesExpected > 5)
                throw std::runtime_error("Number of length octets out of bounds");
        }

        ++m_bytesRead;

        if (m_bytesRead == m_bytesExpected)
        {
            ber::Type const type = ber::Type::fromTag(m_typeTag);
            if (type.value() == 0)
            {
                m_outerLength = ber::decode<length_type>(m_buffer).value;

                if (m_outerLength == 0)
                    throw std::runtime_error("Zero outer length encountered");

                reset(DecodeState::Tag);
            }
            else 
            {
                m_length = ber::decode<length_type>(m_buffer).value;

                bool const isEofOk = m_length == 0;
                if (m_isContainer)
                {
                    reset(DecodeState::Tag);
                    containerReady();
                    pushContainer();
                    disposeCurrentTLV();
                    return isEofOk;
                }

                if (m_length == 0)
                {
                    preloadValue();
                }
                else
                {
                    reset(DecodeState::Value);
                }

                return isEofOk;
            }
        }
        return false;
    }

    LIBEMBER_INLINE
    bool AsyncBerReader::readValueByte(value_type)
    {
        if (m_bytesRead == 0)
            m_bytesExpected = m_length;

        ++m_bytesRead;

        if (m_bytesRead == m_bytesExpected)
        {
            preloadValue();
            return true;
        }

        return false;
    }

    LIBEMBER_INLINE
    bool AsyncBerReader::readTerminatorByte(value_type value)
    {
        if (value != 0)
        {
            throw std::runtime_error("Non-zero byte in terminator");
        }

        if (m_stack.empty()) 
        {
            throw std::runtime_error("Unexpected terminator");
        }

        AsyncContainer& currentContainer = m_stack.back();
        if  (currentContainer.length() != length_type::INDEFINITE)
        {
            throw std::runtime_error("Unexpected terminator");
        }

        m_bytesRead += 1;
        if (m_bytesRead == 3)
        {
            // end of indefinite length container
            currentContainer.setLength(currentContainer.bytesRead());
            return true;
        }
        return false;
    }

    LIBEMBER_INLINE
    void AsyncBerReader::reset(DecodeState const& state)
    {
        m_buffer.clear();
        m_decodeState = state;
        m_bytesExpected = 0;
        m_bytesRead = 0;
    }

    LIBEMBER_INLINE
    void AsyncBerReader::preloadValue()
    {
        if (m_valueBuffer.empty() == false)
            m_valueBuffer.clear();

        m_valueBuffer.swap(m_buffer);
        m_valueLength = std::min(m_length, m_valueBuffer.size());

        reset(DecodeState::Tag);
        itemReady();
        disposeCurrentTLV();
    }

    LIBEMBER_INLINE
    void AsyncBerReader::pushContainer()
    {
        AsyncContainer newContainer(m_appTag, m_typeTag, m_length);
        m_stack.push_back(newContainer);
    }

    LIBEMBER_INLINE
    bool AsyncBerReader::popContainer()
    {
        reset(DecodeState::Tag);

        if (!m_stack.empty())
        {
            {
                AsyncContainer const& currentContainer = m_stack.back();
                m_appTag  = currentContainer.tag();
                m_typeTag = currentContainer.type();
                m_length  = currentContainer.length();
                m_isContainer = true;

                m_stack.pop_back();
            }

            itemReady();

            if (!m_stack.empty())
            {
                m_stack.back().incrementBytesRead(m_length);
            }
            disposeCurrentTLV();
            return true;
        }
        else
        {
            return false;
        }
    }

    LIBEMBER_INLINE
    void AsyncBerReader::disposeCurrentTLV()
    {
        m_appTag = ber::make_tag(0, 0);
        m_typeTag = ber::make_tag(0, 0);
        m_isContainer = false;
        m_length = 0;
        m_outerLength = 0;
        m_valueBuffer.clear();
        m_valueLength = 0;
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_ASYNCBERREADER_IPP

