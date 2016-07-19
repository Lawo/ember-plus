/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_COMMANDTYPE_HPP
#define __LIBS101_COMMANDTYPE_HPP

//SimianIgnore

namespace libs101
{
    /**
     * Struct enumerating the commands available for the S101 messages.
     */ 
    struct CommandType
    {
        enum _Domain
        {
            /**
             * This command indicates that the data of the message contains an encoded
             * ember packet. 
             */
            EmBER = 0x00,

            /**
             * A keep-alive request. This command can be used by both, consumer and provider.
             */
            KeepAliveRequest = 0x01,

            /**
             * The response to the keep-alive request.
             */
            KeepAliveResponse = 0x02,

            /**
             * Reports the server status.
             */
            ProviderState = 0x03
        };


        typedef unsigned char value_type;
        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            CommandType(_Domain value)
                : m_value(static_cast<value_type>(value))
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit CommandType(value_type value)
                : m_value(value)
            {}

            /**
             * Returns the value.
             * @return The value.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };

    inline bool operator==(CommandType const& left, CommandType const& right)
    {
        return (left.value() == right.value());
    }

    inline bool operator!=(CommandType const& left, CommandType const& right)
    {
        return !(left == right);
    }
}

//EndSimianIgnore

#endif  // __LIBS101_COMMANDTYPE_HPP

