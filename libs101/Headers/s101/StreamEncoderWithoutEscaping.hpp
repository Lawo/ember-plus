/*
libs101 -- C++ 03 implementation of the S101 encoding and decoding

Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_STREAMENCODERWITHOUTESCAPING_HPP
#define __LIBS101_STREAMENCODERWITHOUTESCAPING_HPP

#include <vector>
#include "Byte.hpp"
#include "util/Crc16.hpp"

//SimianIgnore

namespace libs101
{
    /**
    * Base class which encodes a S101 message without the use of the default escaping mechanism. A S101 message
    * without escaping starts with 0xF8, followed by 4 length bytes in MSB order.
    */
    template<typename ValueType = unsigned char>
    class StreamEncoderWithoutEscaping
    {
    public:
        typedef typename std::vector<ValueType> ByteVector;
        typedef ValueType value_type;
        typedef typename ByteVector::iterator iterator;
        typedef typename ByteVector::const_iterator const_iterator;
        typedef typename ByteVector::pointer pointer;
        typedef typename ByteVector::const_pointer const_pointer;
        typedef typename ByteVector::reference reference;
        typedef typename ByteVector::const_reference const_reference;
        typedef typename ByteVector::size_type size_type;

        /**
        * Constructor, initializes the stream encoder.
        * @param capacity Initial buffer capacity.
        */
        explicit StreamEncoderWithoutEscaping(size_type capacity = 0);

        /**
        * Returns the current buffer size. @see finish must be called before calling this method
        * to assure that the packet is complete and the resulting size can be returned.
        * @return The current buffer size.
        */
        size_type size() const;

        /**
        * Returns the first buffer element. @see finish must be called before calling this method
        * to assure that the packet is complete.
        * @return The first buffer element.
        */
        const_iterator begin() const;

        /**
        * Returns the last item of the buffer. @see finish must be called before calling this method
        * to assure that the packet is complete.
        * @return The last item of the buffer.
        */
        const_iterator end() const;

        /**
        * Encodes a single byte.
        * This method has to check whether the packet has already started. If not, the header will
        * be written. Prefer using @see encode(first, last) to avoid a per-byte check.
        * @param input The byte to append to the buffer.
        */
        void encode(value_type input);

        /**
        * Encodes n bytes. If no byte has been written yet, the header will be
        * written once.
        * @param first First item to encode.
        * @param last Last item to encode.
        */
        template<typename InputIterator>
        void encode(InputIterator first, InputIterator last);

        /**
        * Appends the crc and the EoF byte to the buffer. After calling
        * this method, the packet may be transmitted.
        */
        void finish();

        /**
        * Resets the encoding buffer.
        */
        void reset();

        /**
        * Returns true if the encoded package is complete and ready to be transmitted.
        * @return true if the packet is valid, false if finish still has to be called.
        */
        bool isFinished() const;

    private:
        ByteVector m_bytes;
        bool m_isFinished;
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline StreamEncoderWithoutEscaping<ValueType>::StreamEncoderWithoutEscaping(size_type capacity)
        : m_bytes(capacity)
        , m_isFinished(false)
    {
    }

    template<typename ValueType>
    inline bool StreamEncoderWithoutEscaping<ValueType>::isFinished() const
    {
        return m_isFinished;
    }

    template<typename ValueType>
    inline void StreamEncoderWithoutEscaping<ValueType>::finish()
    {
        std::size_t const totalLength = m_bytes.size();

        if (totalLength < 6)
        {
            return;
        }

        std::size_t payloadLength = totalLength - 6;

        m_bytes[2] = static_cast<value_type>((payloadLength >> 24) & 0xFF);
        m_bytes[3] = static_cast<value_type>((payloadLength >> 16) & 0xFF);
        m_bytes[4] = static_cast<value_type>((payloadLength >> 8) & 0xFF);
        m_bytes[5] = static_cast<value_type>((payloadLength >> 0) & 0xFF);
        m_isFinished = true;
    }

    template<typename ValueType>
    inline typename StreamEncoderWithoutEscaping<ValueType>::const_iterator StreamEncoderWithoutEscaping<ValueType>::begin() const
    {
        return m_bytes.begin();
    }

    template<typename ValueType>
    inline typename StreamEncoderWithoutEscaping<ValueType>::const_iterator StreamEncoderWithoutEscaping<ValueType>::end() const
    {
        return m_bytes.end();
    }

    template<typename ValueType>
    inline typename StreamEncoderWithoutEscaping<ValueType>::size_type StreamEncoderWithoutEscaping<ValueType>::size() const
    {
        return m_bytes.size();
    }

    template<typename ValueType>
    inline void StreamEncoderWithoutEscaping<ValueType>::reset()
    {
        m_bytes.clear();
        m_isFinished = false;
    }

    template<typename ValueType>
    inline void StreamEncoderWithoutEscaping<ValueType>::encode(value_type input)
    {
        if (m_bytes.empty())
        {
            m_bytes.push_back(Byte::Invalid);
            m_bytes.push_back(0x04);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
        }

        m_bytes.push_back(input);
    }

    template<typename ValueType>
    template<typename InputIterator>
    inline void StreamEncoderWithoutEscaping<ValueType>::encode(InputIterator first, InputIterator last)
    {
        if (m_bytes.empty())
        {
            m_bytes.push_back(Byte::Invalid);
            m_bytes.push_back(0x04);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
            m_bytes.push_back(0x00);
        }

        m_bytes.insert(m_bytes.end(), first, last);
    }
}

//EndSimianIgnore

#endif  // __LIBS101_STREAMENCODERWITHOUTESCAPING_HPP

