/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "../../gadget/ParameterTypeVisitor.h"
#include "../../gadget/StreamFormat.h"
#include <map>
#include <memory>
#include <vector>
#include <ember/Ember.hpp>
#include "StreamConverter.h"
#include "../../gadget/BooleanParameter.h"
#include "../../gadget/EnumParameter.h"
#include "../../gadget/IntegerParameter.h"
#include "../../gadget/RealParameter.h"
#include "../../gadget/StringParameter.h"
#include "../../gadget/Parameter.h"
#include "../../gadget/StreamManager.h"

/**************************************************************************
 * Inline implementation                                                  *
 **************************************************************************/

template<typename OutputIterator>
inline void glow::util::StreamConverter::encode(long long value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last)
{
    auto const type = format.value();
    switch(type)
    {
        case gadget::StreamFormat::UnsignedInt8:
            *first = (value & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt16BigEndian:
            *first++ = ((value >> 8) & 0xFF);
            *first++ = ((value >> 0) & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt16LittleEndian:
            *first++ = ((value >> 0) & 0xFF);
            *first++ = ((value >> 8) & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt32BigEndian:
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt32LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt64BigEndian:
            *first++ = ((value >> 56) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::UnsignedInt64LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 56) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt8:
            *first = (value & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt16BigEndian:
            *first++ = ((value >> 8) & 0xFF);
            *first++ = ((value >> 0) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt16LittleEndian:
            *first++ = ((value >> 0) & 0xFF);
            *first++ = ((value >> 8) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt32BigEndian:
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt32LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt64BigEndian:
            *first++ = ((value >> 56) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::SignedInt64LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 56) & 0xFF);
            break;

        case gadget::StreamFormat::IeeeFloat32BigEndian:
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::IeeeFloat32LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            break;

        case gadget::StreamFormat::IeeeFloat64BigEndian:
            *first++ = ((value >> 56) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >>  0) & 0xFF);
            break;

        case gadget::StreamFormat::IeeeFloat64LittleEndian:
            *first++ = ((value >>  0) & 0xFF);
            *first++ = ((value >>  8) & 0xFF);
            *first++ = ((value >> 16) & 0xFF);
            *first++ = ((value >> 24) & 0xFF);
            *first++ = ((value >> 32) & 0xFF);
            *first++ = ((value >> 40) & 0xFF);
            *first++ = ((value >> 48) & 0xFF);
            *first++ = ((value >> 56) & 0xFF);
            break;
    }
}

template<typename OutputIterator>
inline void glow::util::StreamConverter::encode(double value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last)
{
    if (format.value() == gadget::StreamFormat::IeeeFloat64BigEndian
    ||  format.value() == gadget::StreamFormat::IeeeFloat64LittleEndian)
    {
        long long transformed = *(reinterpret_cast<long long*>(&value));
        encode(transformed, format, first, last);
    }
    else
    {
        auto valueAsSingle = static_cast<float>(value);
        long long transformed = *(reinterpret_cast<long*>(&valueAsSingle));
        encode(transformed, format, first, last);
    }
}

template<typename OutputIterator>
inline void glow::util::StreamConverter::encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
{
    glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::encode(parameter, first, last);
}

template<typename OutputIterator>
inline void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
{
    auto volatile encoder = OctetStreamEntryEncoder(parameter, first, last);
}

template<typename OutputIterator>
glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::OctetStreamEntryEncoder(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
    : m_first(first)
    , m_last(last)
{
    parameter->accept(*this);
}

template<typename OutputIterator>
void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::BooleanParameter* parameter)
{
    /* Booleans are not supported */
}

template<typename OutputIterator>
void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::EnumParameter* parameter)
{
    if (parameter->hasStreamDescriptor())
    {
        long long const value = parameter->index();
        gadget::StreamFormat const format = parameter->streamDescriptor()->format();
        auto const offset = parameter->streamDescriptor()->offset();
        glow::util::StreamConverter::encode(value, format, m_first + offset, m_last);
    }
}

template<typename OutputIterator>
void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::StringParameter* parameter)
{
    /* Strings are not supported for octet streams */
}

template<typename OutputIterator>
void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::IntegerParameter* parameter)
{
    if (parameter->hasStreamDescriptor())
    {
        long long const value = parameter->value();
        auto const format = parameter->streamDescriptor()->format();
        auto const offset = parameter->streamDescriptor()->offset();
        glow::util::StreamConverter::encode(value, format, m_first + offset, m_last);
    }
}

template<typename OutputIterator>
void glow::util::StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::RealParameter* parameter)
{
    if (parameter->hasStreamDescriptor())
    {
        auto const value = parameter->value();
        auto const format = parameter->streamDescriptor()->format();
        auto const offset = parameter->streamDescriptor()->offset();
        glow::util::StreamConverter::encode(value, format, m_first + offset, m_last);
    }
}

using namespace ::libember::glow;

namespace glow { namespace util 
{
    libember::glow::GlowStreamEntry* StreamConverter::SingleStreamEntryFactory::create(gadget::Parameter* parameter)
    {
        auto converter = SingleStreamEntryFactory(parameter);
        return converter.m_entry;
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::BooleanParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), !!parameter->value());
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::EnumParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), static_cast<int>(parameter->index()));
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::StringParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), parameter->value());
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::IntegerParameter* parameter)
    {
        m_entry = new GlowStreamEntry((int)parameter->streamIdentifier(), static_cast<int>(parameter->value()));
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::RealParameter* parameter)
    {
        m_entry = new GlowStreamEntry((int)parameter->streamIdentifier(), (gadget::RealParameter::value_type)parameter->value());
    }

    StreamConverter::SingleStreamEntryFactory::SingleStreamEntryFactory(gadget::Parameter* parameter)
        : m_entry(nullptr)
    {
        parameter->accept(*this);
    }




    libember::glow::GlowStreamCollection* StreamConverter::create(libember::glow::GlowStreamCollection* root, gadget::StreamManager const& manager)
    {
        typedef std::vector<gadget::Parameter*> ParameterCollection;
        std::map<int, std::unique_ptr<ParameterCollection> > dic;

        {
            auto it = std::begin(manager);
            auto const last= std::end(manager);
            for (; it != last; ++it)
            {
                auto& parameter = *it;
                auto const identifier = parameter->streamIdentifier();

                if (dic.find(identifier) == std::end(dic))
                {
                    dic[identifier] = std::unique_ptr<ParameterCollection>(new ParameterCollection());
                }

                dic[identifier]->push_back(parameter);
            }
        }

        {
            for(auto& pair : dic)
            {
                auto& streams = *pair.second;
                auto const identifier = pair.first;
                auto const size = streams.size();

                if (size == 1 && streams.front()->hasStreamDescriptor() == false)
                {
                    auto parameter = streams.front();
                    if (parameter->isSubscribed() && parameter->isDirty())
                    {
                        auto entry = SingleStreamEntryFactory::create(parameter);
                        root->insert(entry);
                    }
                }
                else if (size >= 1)
                {
                    auto const first = std::begin(streams);
                    auto const last = std::end(streams);
                    auto const result = std::max_element(first, last, [](decltype(*first) max, decltype(*first) cur) -> bool
                    {
                        if (max->hasStreamDescriptor() && cur->hasStreamDescriptor())
                        {
                            return cur->streamDescriptor()->offset() > max->streamDescriptor()->offset();
                        }
                        else if (max->hasStreamDescriptor())
                        {
                            return false;
                        }
                        else if (cur->hasStreamDescriptor())
                        {
                            return true;
                        }

                        return false;
                    });

                    auto const isSubscribed = std::any_of(first, last, [](decltype(*first) stream) -> bool
                    {
                        return stream->isSubscribed() && stream->isDirty();
                    });

                    if (result != last && isSubscribed)
                    {
                        auto descriptor = (*result)->streamDescriptor();
                        auto const format = descriptor->format();
                        auto const offset = descriptor->offset();
                        auto const size = offset + format.size();

                        auto buffer = std::vector<unsigned char>(size, 0x00);
                        for(auto parameter : streams)
                        {
                            encode(parameter, std::begin(buffer), std::end(buffer));
                            parameter->clearDirtyState();
                        }

                        root->insert(identifier, std::begin(buffer), std::end(buffer));
                    }
                }
            }
        }

        return root;
    }
}
}
