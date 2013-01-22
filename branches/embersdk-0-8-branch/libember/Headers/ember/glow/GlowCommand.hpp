#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_HPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_HPP

#include "CommandType.hpp"
#include "GlowElement.hpp"

namespace libember { namespace glow
{ 
    /** Forward declaration **/
    class GlowNodeFactory;

    class LIBEMBER_API GlowCommand : public GlowElement
    {
        friend class GlowNodeFactory;
        public:
            /** 
             * Initializes a command.
             * @note The application tag will be set to ContextSpecific-0.
             * @param number The command number to initialize this instance with.
             **/
            explicit GlowCommand(CommandType const& number);

            /**
             * Initializes a new command with an application tag.
             * @param number The command number to initialize this instance with
             * @param tag Application tag
             */
            GlowCommand(CommandType const& number, ber::Tag const& tag);

            /**
             * Returns the command number.
             * @return The command number.
             */
            int number() const;

        private:
            /**
             * This constructor initializes a command without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowCommand(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowCommand.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_HPP
