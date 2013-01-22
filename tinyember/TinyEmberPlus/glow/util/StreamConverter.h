#ifndef __TINYEMBER_GLOW_UTIL_STREAMCONVERTER_H
#define __TINYEMBER_GLOW_UTIL_STREAMCONVERTER_H

#include "../../gadget/StreamFormat.h"
#include "../../gadget/ParameterTypeVisitor.h"

namespace libember { namespace glow
{
    class GlowStreamCollection;
    class GlowStreamEntry;
}
}

namespace gadget
{
    class StreamManager;
}

namespace glow { namespace util 
{
    class StreamConverter
    {
        public:
            static libember::glow::GlowStreamCollection* create(libember::glow::GlowStreamCollection* root, gadget::StreamManager const& manager);

        private:
            template<typename OutputIterator>
            static void encode(long long value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last);

            template<typename OutputIterator>
            static void encode(double value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last);

            template<typename OutputIterator>
            static void encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last);

            class SingleStreamEntryFactory : private gadget::ParameterTypeVisitor
            {
                public:
                    static libember::glow::GlowStreamEntry* create(gadget::Parameter* parameter);

                public:
                    virtual void visit(gadget::EnumParameter* parameter);

                    virtual void visit(gadget::StringParameter* parameter);

                    virtual void visit(gadget::IntegerParameter* parameter);

                    virtual void visit(gadget::RealParameter* parameter);

                private:
                    explicit SingleStreamEntryFactory(gadget::Parameter* parameter);

                private:
                    libember::glow::GlowStreamEntry* m_entry;
            };

            template<typename OutputIterator>
            class OctetStreamEntryEncoder : private gadget::ParameterTypeVisitor
            {
                public:
                    static void encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last);

                public:
                    virtual void visit(gadget::EnumParameter* parameter);

                    virtual void visit(gadget::StringParameter* parameter);

                    virtual void visit(gadget::IntegerParameter* parameter);

                    virtual void visit(gadget::RealParameter* parameter);

                private:
                    explicit OctetStreamEntryEncoder(gadget::Parameter* parameter, OutputIterator first, OutputIterator last);

                private:
                    OutputIterator m_first;
                    OutputIterator m_last;
            };

            template<typename OutputIterator>
            friend class OctetStreamEntryEncoder;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename OutputIterator>
    inline void StreamConverter::encode(long long value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last)
    {
        auto const type = format.value();
        switch(type)
        {
            case ::gadget::StreamFormat::UnsignedInt8:
                *first = (value & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt16BigEndian:
                *first++ = ((value >> 8) & 0xFF);
                *first++ = ((value >> 0) & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt16LittleEndian:
                *first++ = ((value >> 0) & 0xFF);
                *first++ = ((value >> 8) & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt32BigEndian:
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt32LittleEndian:
                *first++ = ((value >>  0) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt64BigEndian:
                *first++ = ((value >> 56) & 0xFF);
                *first++ = ((value >> 48) & 0xFF);
                *first++ = ((value >> 40) & 0xFF);
                *first++ = ((value >> 32) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::UnsignedInt64LittleEndian:
                *first++ = ((value >>  0) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 32) & 0xFF);
                *first++ = ((value >> 40) & 0xFF);
                *first++ = ((value >> 48) & 0xFF);
                *first++ = ((value >> 56) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt8:
                *first = (value & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt16BigEndian:
                *first++ = ((value >> 8) & 0xFF);
                *first++ = ((value >> 0) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt16LittleEndian:
                *first++ = ((value >> 0) & 0xFF);
                *first++ = ((value >> 8) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt32BigEndian:
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt32LittleEndian:
                *first++ = ((value >>  0) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt64BigEndian:
                *first++ = ((value >> 56) & 0xFF);
                *first++ = ((value >> 48) & 0xFF);
                *first++ = ((value >> 40) & 0xFF);
                *first++ = ((value >> 32) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::SignedInt64LittleEndian:
                *first++ = ((value >>  0) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 32) & 0xFF);
                *first++ = ((value >> 40) & 0xFF);
                *first++ = ((value >> 48) & 0xFF);
                *first++ = ((value >> 56) & 0xFF);
                break;

            case ::gadget::StreamFormat::IeeeFloat32BigEndian:
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::IeeeFloat32LittleEndian:
                *first++ = ((value >>  0) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                break;

            case ::gadget::StreamFormat::IeeeFloat64BigEndian:
                *first++ = ((value >> 56) & 0xFF);
                *first++ = ((value >> 48) & 0xFF);
                *first++ = ((value >> 40) & 0xFF);
                *first++ = ((value >> 32) & 0xFF);
                *first++ = ((value >> 24) & 0xFF);
                *first++ = ((value >> 16) & 0xFF);
                *first++ = ((value >>  8) & 0xFF);
                *first++ = ((value >>  0) & 0xFF);
                break;

            case ::gadget::StreamFormat::IeeeFloat64LittleEndian:
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
    inline void StreamConverter::encode(double value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last)
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
    inline void StreamConverter::encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
    {
        OctetStreamEntryEncoder<OutputIterator>::encode(parameter, first, last);
    }


    template<typename OutputIterator>
    inline void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
    {
        auto volatile encoder = OctetStreamEntryEncoder(parameter, first, last);
    }

    template<typename OutputIterator>
    StreamConverter::OctetStreamEntryEncoder<OutputIterator>::OctetStreamEntryEncoder(gadget::Parameter* parameter, OutputIterator first, OutputIterator last)
        : m_first(first)
        , m_last(last)
    {
        parameter->accept(*this);
    }

    template<typename OutputIterator>
    void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::EnumParameter* parameter)
    {
        if (parameter->hasStreamDescriptor())
        {
            long long const value = parameter->index();
            auto const format = parameter->streamDescriptor()->format();
            auto const offset = parameter->streamDescriptor()->offset();
            StreamConverter::encode(value, format, m_first + offset, m_last);
        }
    }

    template<typename OutputIterator>
    void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::StringParameter* parameter)
    {
        /* Strings are not supported for octet streams */
    }

    template<typename OutputIterator>
    void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::IntegerParameter* parameter)
    {
        if (parameter->hasStreamDescriptor())
        {
            long long const value = parameter->value();
            auto const format = parameter->streamDescriptor()->format();
            auto const offset = parameter->streamDescriptor()->offset();
            StreamConverter::encode(value, format, m_first + offset, m_last);
        }
    }

    template<typename OutputIterator>
    void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::RealParameter* parameter)
    {
        if (parameter->hasStreamDescriptor())
        {
            auto const value = parameter->value();
            auto const format = parameter->streamDescriptor()->format();
            auto const offset = parameter->streamDescriptor()->offset();
            StreamConverter::encode(value, format, m_first + offset, m_last);
        }
    }
}
}

#endif//__TINYEMBER_GLOW_UTIL_STREAMCONVERTER_H
