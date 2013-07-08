#ifndef __TINYEMBER_GLOW_UTIL_STREAMCONVERTER_H
#define __TINYEMBER_GLOW_UTIL_STREAMCONVERTER_H

#include "../../gadget/StreamFormat.h"
#include "../../gadget/ParameterTypeVisitor.h"

/** Forward declarations */
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
    /**
     * This class is used to create a stream entry from one or more parameters.
     */
    class StreamConverter
    {
        public:
            /**
             * Creates a new stream collection that contains all parameters that are registered to the specified stream manager.
             * @param root The collection to append the stream entries to.
             * @param manager The manager to get the stream values from.
             * @return The passed GlowStreamCollection.
             */
            static libember::glow::GlowStreamCollection* create(libember::glow::GlowStreamCollection* root, gadget::StreamManager const& manager);

        private:
            /**
             * Encodes an integral stream value and appends it to the specified output stream.
             * @param value The value to encode.
             * @param format The stream format to encode the value to.
             * @param first Reference to the first element of the output buffer, where the encoded data shall be written to.
             * @param last Reference to the first element one past the valid output buffer range.
             */
            template<typename OutputIterator>
            static void encode(long long value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last);

            /**
             * Encodes a real stream value and appends it to the specified output stream.
             * @param value The value to encode.
             * @param format The stream format to encode the value to.
             * @param first Reference to the first element of the output buffer, where the encoded data shall be written to.
             * @param last Reference to the first element one past the valid output buffer range.
             */
            template<typename OutputIterator>
            static void encode(double value, gadget::StreamFormat const& format, OutputIterator first, OutputIterator last);

            /**
             * Encodes the value of the passed parameter and appends it to the specified output stream.
             * @param parameter The parameter whos value shall be encoded.
             * @param format The stream format to encode the value to.
             * @param first Reference to the first element of the output buffer, where the encoded data shall be written to.
             * @param last Reference to the first element one past the valid output buffer range.
             */
            template<typename OutputIterator>
            static void encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last);

        private:
            /**
             * Converter for single stream entries
             */
            class SingleStreamEntryFactory : private gadget::ParameterTypeVisitor
            {
                public:
                    /**
                     * Creates a new StreamEntry from the passed parameter.
                     * @param parameter The parameter to create a stream entry for.
                     * @return A new stream entry which contains the streamidentifier and the current value
                     *      of the passed parameter.
                     */
                    static libember::glow::GlowStreamEntry* create(gadget::Parameter* parameter);

                public:
                    /**
                     * Creates a new stream entry for a boolean parameter.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    virtual void visit(gadget::BooleanParameter* parameter);

                    /**
                     * Creates a new stream entry for an enumeration parameter.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    virtual void visit(gadget::EnumParameter* parameter);

                    /**
                     * Creates a new stream entry for a string parameter.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    virtual void visit(gadget::StringParameter* parameter);

                    /**
                     * Creates a new stream entry for an integer parameter.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    virtual void visit(gadget::IntegerParameter* parameter);

                    /**
                     * Creates a new stream entry for a real parameter.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    virtual void visit(gadget::RealParameter* parameter);

                private:
                    /**
                     * Initializes a new SingleStreamFactory. This constructor is called
                     * by the static create method.
                     * @param parameter The parameter to create a stream entry for.
                     */
                    explicit SingleStreamEntryFactory(gadget::Parameter* parameter);

                private:
                    libember::glow::GlowStreamEntry* m_entry;
            };

        private:
            /**
             * Helper class which encodes and appends stream values that are transported
             * within a binary stream.
             */
            template<typename OutputIterator>
            class OctetStreamEntryEncoder : private gadget::ParameterTypeVisitor
            {
                public:
                    /**
                     * Encodes and appends the parameter's value to the provided output stream.
                     * @param parameter The parameter whos stream value shall be added to the output stream.
                     * @param first Reference to the first element of the output buffer, where the encoded data shall be written to.
                     * @param last Reference to the first element one past the valid output buffer range.
                     */
                    static void encode(gadget::Parameter* parameter, OutputIterator first, OutputIterator last);

                public:
                    /**
                     * Encodes and appends the currently selected index.
                     * @param parameter the parameter whos value shall be appended to the output stream.
                     */
                    virtual void visit(gadget::EnumParameter* parameter);

                    /**
                     * String parameters are not supported, so this method doesn't do anything.
                     * @param parameter the parameter whos value shall be appended to the output stream.
                     */
                    virtual void visit(gadget::StringParameter* parameter);

                    /**
                     * Encodes and appends the parameter's value.
                     * @param parameter the parameter whos value shall be appended to the output stream.
                     */
                    virtual void visit(gadget::IntegerParameter* parameter);

                    /**
                     * Encodes and appends the parameter's value
                     * @param parameter the parameter whos value shall be appended to the output stream.
                     */
                    virtual void visit(gadget::RealParameter* parameter);

                    /**
                     * Boolean parameters are not supported, so this method doesn't do anything.
                     * @param parameter the parameter whos value shall be appended to the output stream.
                     */
                    virtual void visit(gadget::BooleanParameter* parameter);

                private:
                    /**
                     * Initializes a new OctetStreamEncoder instance which appends the value of the passed
                     * parameter to the provided output stream.
                     * @param parameter The parameter whos value shall be added to the output stream.
                     * @param first Reference to the first element of the output buffer, where the encoded data shall be written to.
                     * @param last Reference to the first element one past the valid output buffer range.
                     */
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
    void StreamConverter::OctetStreamEntryEncoder<OutputIterator>::visit(gadget::BooleanParameter* parameter)
    {
        /* Booleans are not supported */
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
