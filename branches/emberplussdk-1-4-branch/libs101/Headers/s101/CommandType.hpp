/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBS101_COMMANDTYPE_HPP
#define __LIBS101_COMMANDTYPE_HPP

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
             * Non-explicit constructor to allow implicit conversion.
             * @param value A value from _Domain to initialize this instance with.
             */
            CommandType(_Domain value)
                : m_value(static_cast<value_type>(value))
            {}

            /**
             * Return the raw value currently represented by this instance.
             * @return The raw value currently represented by this instance.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };
}

#endif  // __LIBS101_COMMANDTYPE_HPP

