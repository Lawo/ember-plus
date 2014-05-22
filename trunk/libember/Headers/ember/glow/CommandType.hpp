/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_COMMANDTYPE_HPP
#define __LIBEMBER_GLOW_COMMANDTYPE_HPP

//SimianIgnore

namespace libember { namespace glow
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the command types supported by Glow.
     */
    class CommandType
    {
        public:
            enum _Domain
            {
                /**
                 * This value is returned when a GlowCommand instance does not contain
                 * a command.
                 */
                None = 0,

                /**
                 * The command number for a subscription request. The main
                 * purpose of this command is to subscribe to a stream parameter
                 * which contains audio level data.
                 */
                Subscribe = 30,

                /**
                 * The command number for a unsubscribe request. The unsubscribe
                 * operation has to be performed for all child nodes and
                 * parameters.
                 */
                Unsubscribe = 31,

                /**
                 * The command number for a GetDirectory command. This command
                 * is used to query the children of a node.
                 */
                GetDirectory = 32,

                /**
                 * The command number for an invocation request. This command
                 * is used let the provider execute a function.
                 */
                Invoke = 33
            };

            typedef int value_type;

        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            CommandType(_Domain value)
                : m_value(value)
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
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_GLOW_COMMANDTYPE_HPP

