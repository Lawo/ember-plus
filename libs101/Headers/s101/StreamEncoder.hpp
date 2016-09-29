/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_STREAMENCODER_HPP
#define __LIBS101_STREAMENCODER_HPP

#include <vector>
#include "Byte.hpp"
#include "util/Crc16.hpp"

//SimianIgnore

namespace libs101
{
    /**
     * Base class which encodes a S101 message.
     */
    template<typename ValueType = unsigned char>
    class StreamEncoder
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
            explicit StreamEncoder(size_type capacity = 0);

            /**
             * Returns the current buffer size.
             * @return The current buffer size.
             */
            size_type size() const;

            /**
             * Returns the first buffer element.
             * @return The first buffer element.
             */
            const_iterator begin() const;

            /**
             * Returns the last item of the buffer.
             * @return The last item of the buffer.
             */
            const_iterator end() const;

            /**
             * Encodes a single byte.
             * @param input The byte to append to the buffer.
             */
            void encode(value_type input);

            /**
             * Encodes n bytes.
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
            /**
             * Appends a single byte to the encoding buffer. 
             * @input Byte to append to the buffer.
             */
            void append(value_type input);

        private:
            ByteVector m_bytes;
            util::Crc16::value_type m_crc;
            bool m_isFinished;
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline StreamEncoder<ValueType>::StreamEncoder(size_type capacity)
        : m_bytes(capacity)
        , m_crc(0xFFFF)
        , m_isFinished(false)
    {}

    template<typename ValueType>
    inline bool StreamEncoder<ValueType>::isFinished() const
    {
        return m_isFinished;
    }

    template<typename ValueType>
    inline void StreamEncoder<ValueType>::finish()
    {
        if (m_isFinished == false)
        {
            m_crc = ~m_crc;

            append((m_crc >> 0) & 0xFF);
            append((m_crc >> 8) & 0xFF);

            m_bytes.push_back(Byte::EoF);
            m_isFinished = true;
        }
    }

    template<typename ValueType>
    inline typename StreamEncoder<ValueType>::const_iterator StreamEncoder<ValueType>::begin() const
    {
        return m_bytes.begin();
    }

    template<typename ValueType>
    inline typename StreamEncoder<ValueType>::const_iterator StreamEncoder<ValueType>::end() const
    {
        return m_bytes.end();
    }

    template<typename ValueType>
    inline typename StreamEncoder<ValueType>::size_type StreamEncoder<ValueType>::size() const
    {
        return m_bytes.size();
    }

    template<typename ValueType>
    inline void StreamEncoder<ValueType>::reset()
    {
        m_bytes.clear();
        m_crc = 0xFFFF;
        m_isFinished = false;
    }

    template<typename ValueType>
    inline void StreamEncoder<ValueType>::encode(value_type input)
    {
        if (m_bytes.empty())
        {
            m_crc = 0xFFFF;
            m_bytes.push_back(Byte::BoF);
        }

        m_crc = util::Crc16::add(m_crc, input);

        append(input);
    }

    template<typename ValueType>
    template<typename InputIterator>
    inline void StreamEncoder<ValueType>::encode(InputIterator first, InputIterator last)
    {
        for(; first != last; ++first)
            encode(*first);
    }

    template<typename ValueType>
    inline void StreamEncoder<ValueType>::append(value_type input)
    {
        if (input >= Byte::Invalid)
        {
            m_bytes.push_back(Byte::CE);
            m_bytes.push_back(input ^ Byte::XOR);
        }
        else
        {
            m_bytes.push_back(input);
        }
    }
}

//EndSimianIgnore

#endif  // __LIBS101_STREAMENCODER_HPP

