/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_STREAMDECODER_HPP
#define __LIBS101_STREAMDECODER_HPP

#include <vector>
#include "Byte.hpp"
#include "util/Crc16.hpp"

//SimianIgnore

namespace libs101
{
    /**
     * Base class which decodes a S101 message.
     */
    template<typename ValueType = unsigned char>
    class StreamDecoder
    {
        typedef std::vector<ValueType> ByteVector;

    public:
        typedef ValueType value_type;
        typedef typename ByteVector::iterator iterator;
        typedef typename ByteVector::const_iterator const_iterator;
        typedef typename ByteVector::pointer pointer;
        typedef typename ByteVector::const_pointer const_pointer;
        typedef typename ByteVector::reference reference;
        typedef typename ByteVector::const_reference const_reference;
        typedef typename ByteVector::size_type size_type;

        /** Constructor */
        StreamDecoder();

        /** Destructor */
        ~StreamDecoder();

        /**
         * Reads n bytes from the provided input buffer. Each time a 
         * valid message has been decoded, the provided callback function will
         * be invoked with the decoded result. The result contains the decoded
         * data buffer.
         * @param first First item of the buffer to decode the data from.
         * @param last Last item of the buffer to decode the data from.
         * @param callback Callback function that will be called when a valid
         *      message has been decoded. The function must have the following 
         *      signature: (const_iterator, const_iterator, StateType)
         * @param state A user state that can be used to transfer any 
         *      kind of data to the callback function.
         * @note Each time a message has been decoded this method calls reset.
         */
        template<typename InputIterator, typename CallbackType, typename StateType>
        void read(InputIterator first, InputIterator last, CallbackType callback, StateType state);

        /**
         * Reads n bytes from the provided input buffer. Each time a 
         * valid message has been decoded, the provided callback function will
         * be invoked with the decoded result. The result contains the decoded
         * data buffer.
         * @param first First item of the buffer to decode the data from.
         * @param last Last item of the buffer to decode the data from.
         * @param callback Callback function that will be called when a valid
         *      message has been decoded. The function must have the following 
         *      signature: (const_iterator, const_iterator)
         * @note Each time a message has been decoded this method calls reset.
         */
        template<typename InputIterator, typename CallbackType>
        void read(InputIterator first, InputIterator last, CallbackType callback);

        /**
         * Decodes a single byte. If this is the last byte of a S101 message
         * the provided callback function will be invoked.
         * @param input The byte to decode.
         * @param callback Callback function that will be called when a valid
         *      message has been decoded. The function must have the following 
         *      signature: (const_iterator, const_iterator, StateType)
         * @param state A user state that can be used to transfer any 
         *      kind of data to the callback function.
         * @note Each time a message has been decoded this method calls reset.
         */
        template<typename InputType, typename CallbackType, typename StateType>
        void readByte(InputType input, CallbackType callback, StateType state);

        /**
         * Decodes a single byte. If this is the last byte of a S101 message
         * the provided callback function will be invoked.
         * @param input The byte to decode.
         * @param callback Callback function that will be called when a valid
         *      message has been decoded. The function must have the following 
         *      signature: (const_iterator, const_iterator)
         * @note Each time a message has been decoded this method calls reset.
         */
        template<typename InputType, typename CallbackType>
        void readByte(InputType input, CallbackType callback);

        /** Resets the current decoding buffer. */
        void reset();

        /**
         * Gets a value indicating whether the decoder currently parses a
         * frame that uses the S101 variant without escaping. This
         * method shall be called while the callback is being invoked
         * to determine whether the current packet uses escaping or not.
         */
        bool isDecodingFrameWithoutEscaping() const;

    private:
        enum State
        {
            OutOfFrame,
            WithinFrameWithEscaping,
            WithinFrameWithoutEscaping
        };

    private:
        /** Resets the current decoding buffer.
         * @param state Specifies the current decoder state.
         */
        void reset(State state);

        /**
         * This static method is used to invoke a callback which doesn't have a state parameter.
         * @param first Start of the buffer containing a decoded S101 message.
         * @param last End of the buffer.
         * @param callback The stateless callback, which only expects the data buffer.
         */
        template<typename CallbackType>
        static void invokeStatelessCallback(const_iterator first, const_iterator last, CallbackType callback);

        ByteVector m_bytes;
        bool m_escape;
        State m_state;
        util::Crc16::value_type m_crc;
        size_type m_payloadLength;
        size_type m_payloadLengthLength;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename ValueType>
    inline StreamDecoder<ValueType>::StreamDecoder()
        : m_escape(false)
        , m_state(OutOfFrame)
        , m_crc(0xFFFF)
        , m_payloadLength(0)
        , m_payloadLengthLength(0)
    {}

    template<typename ValueType>
    inline StreamDecoder<ValueType>::~StreamDecoder()
    {}

    template<typename ValueType>
    inline bool StreamDecoder<ValueType>::isDecodingFrameWithoutEscaping() const
    {
        return m_state == WithinFrameWithoutEscaping;
    }

    template<typename ValueType>
    template<typename CallbackType>
    inline void StreamDecoder<ValueType>::invokeStatelessCallback(const_iterator first, const_iterator last, CallbackType callback)
    {
        callback(first, last);
    }

    template<typename ValueType>
    inline void StreamDecoder<ValueType>::reset()
    {
        reset(OutOfFrame);
    }

    template<typename ValueType>
    inline void StreamDecoder<ValueType>::reset(State state)
    {
        m_bytes.clear();
        m_escape = false;
        m_state = state;
        m_crc = 0xFFFF;
        m_payloadLength = 0;
        m_payloadLengthLength = 0;
    }

    template<typename ValueType>
    template<typename InputIterator, typename CallbackType, typename StateType>
    inline void StreamDecoder<ValueType>::read(InputIterator first, InputIterator last, CallbackType callback, StateType state)
    {
        for(; first != last; ++first)
            readByte(*first, callback, state);
    }

    template<typename ValueType>
    template<typename InputIterator, typename CallbackType>
    inline void StreamDecoder<ValueType>::read(InputIterator first, InputIterator last, CallbackType callback)
    {
        for(; first != last; ++first)
            readByte(*first, callback);
    }

    template<typename ValueType>
    template<typename InputType, typename CallbackType>
    inline void StreamDecoder<ValueType>::readByte(InputType input, CallbackType callback)
    {
        typedef void (*CallbackBindType)(const_iterator, const_iterator, CallbackType);

        readByte<InputType, CallbackBindType, CallbackType>(input, invokeStatelessCallback, callback);
    }

    template<typename ValueType>
    template<typename InputType, typename CallbackType, typename StateType>
    inline void StreamDecoder<ValueType>::readByte(InputType input, CallbackType callback, StateType state)
    {
        value_type byte = static_cast<value_type>(input);

        switch (m_state)
        {
        case OutOfFrame:
            if (byte == Byte::BoF)
            {
                reset(WithinFrameWithEscaping);
            }
            else if (byte == Byte::Invalid)
            {
                reset(WithinFrameWithoutEscaping);
            }

            break;

        case WithinFrameWithEscaping:
            switch (byte)
            {
            case Byte::BoF:
                reset(WithinFrameWithEscaping);
                break;

            case Byte::EoF:
                if (m_crc == 0xF0B8 && m_bytes.size() > 1)
                    callback(m_bytes.begin(), m_bytes.end() - 2, state);

                reset();
                break;

            case Byte::CE:
                m_escape = true;
                break;

            default:
                if (m_escape)
                {
                    m_escape = false;

                    byte = byte ^ Byte::XOR;
                }

                m_bytes.push_back(byte);
                m_crc = util::Crc16::add(m_crc, byte);
                break;
            }

            break;

        case WithinFrameWithoutEscaping:
            m_bytes.push_back(byte);

            size_type const length = m_bytes.size();

            if (length == 1)
            {
                m_payloadLengthLength = byte & 0x07;
            }
            else if (length == 1 + m_payloadLengthLength)
            {
                m_payloadLength = 0;

                for (size_type index = 0; index < m_payloadLengthLength; ++index)
                {
                    int const shift = (m_payloadLength - index - 1) * 8;

                    m_payloadLength |= (m_bytes[1 + index] << shift);
                }
            }

            if (length >= 1 && length - (1 + m_payloadLengthLength) == m_payloadLength)
            {
                callback(m_bytes.begin() + (1 + m_payloadLengthLength), m_bytes.end(), state);
                reset();
            }

            break;
        }
    }
}

//EndSimianIgnore

#endif  // __LIBS101_STREAMDECODER_HPP
