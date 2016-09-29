/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_HPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_HPP

#include "CommandType.hpp"
#include "DirFieldMask.hpp"
#include "GlowElement.hpp"

namespace libember { namespace glow
{ 
    /** Forward declaration **/
    class GlowInvocation;
    class GlowNodeFactory;
    class GlowNodeBase;
    class GlowParameterBase;
    class GlowRootElementCollection;

    /**
     * The GlowCommand contains a command code (called number) and it is usually attached to the 
     * RootElementCollection, a Node or a Parameter.
     */ 
    class LIBEMBER_API GlowCommand : public GlowElement
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a command with a specified number and query mask.
             * @note The application tag will be set to ContextSpecific-0.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            GlowCommand(CommandType const& number, DirFieldMask const& mask = DirFieldMask::Default);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The parent this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowRootElementCollection* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::Default);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The parent this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowParameterBase* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::Default);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The parent this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowNodeBase* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::Default);

            /**
             * Returns the command number.
             * @return The command number.
             */
            CommandType number() const;

            /**
             * Returns the optional flags that can be set when the command type
             * is set to 'GetDirectory'. 
             * @return Returns the flags. If the value is not set, DirFieldFlags::All will be returned.
             */
            DirFieldMask dirFieldMask() const;

            /**
             * Returns the invocation that contains a function to execute. If no
             * invocation is present, this method returns null.
             * @return An invocation associated with this command.
             */
            GlowInvocation const* invocation() const;

            /**
             * Sets the dirFieldFlags. This property is only considered when the command type is
             * set to 'GetDirectory'.
             * @param value The flags value to set.
             */
            void setDirFieldMask(DirFieldMask const& value);

            /**
             * Sets an invocation request.
             * @param value The invocation to attach to this command.
             */
            void setInvocation(GlowInvocation* value);

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
#  include "impl/GlowCommand.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWCOMMAND_HPP
