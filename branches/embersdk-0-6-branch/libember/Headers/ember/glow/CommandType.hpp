#ifndef __LIBEMBER_GLOW_COMMANDTYPE_HPP
#define __LIBEMBER_GLOW_COMMANDTYPE_HPP

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
                 * The command number for a GetDirectory command. This command
                 * is used to query the children of a node.
                 */
                GetDirectory = 32,

                /**
                 * The command number for a subscription request. The main
                 * purpose of this command is to subscribe to a stream parameter
                 * which contains audio level data.
                 */
                Subscribe = 1001,

                /**
                 * The command number for a unsubscribe request. The unsubscribe
                 * operation has to be performed for all child nodes and
                 * parameters.
                 */
                Unsubscribe = 1002
            };

            typedef int value_type;

        public:
            /**
             * Initializes a command. 
             * @param command Command type to initialize this instance with.
             */
            CommandType(_Domain command)
                : m_value(command)
            {}

            /**
             * Returns the command value.
             * @return The command value.
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

#endif  // __LIBEMBER_GLOW_COMMANDTYPE_HPP

