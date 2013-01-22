/*
    libember -- C++ 03 implementation of the Ember+ Protocol
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

#ifndef __LIBEMBER_GLOW_GLOWCOMMAND_HPP
#define __LIBEMBER_GLOW_GLOWCOMMAND_HPP

#include "CommandType.hpp"
#include "DirFieldMask.hpp"
#include "GlowElement.hpp"

namespace libember { namespace glow
{ 
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowNode;
    class GlowParameter;
    class GlowQualifiedNode;
    class GlowQualifiedParameter;

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
            GlowCommand(CommandType const& number, DirFieldMask const& mask = DirFieldMask::None);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The parameter this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowParameter* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::None);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The parameter this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowQualifiedParameter* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::None);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The node this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowNode* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::None);

            /**
             * Initializes a command and attaches it to the provided parent.
             * @note The application tag will be set to ContextSpecific-0.
             * @param parent The node this command refers to.
             * @param number The command number to initialize this instance with.
             * @param mask Optional mask that indicates the properties a consumer is interested in.
             */
            explicit GlowCommand(GlowQualifiedNode* parent, CommandType const& number, DirFieldMask const& mask = DirFieldMask::None);

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
             * Sets the dirFieldFlags. This property is only considered when the command type is
             * set to 'GetDirectory'.
             * @param value The flags value to set.
             */
            void setDirFieldMask(DirFieldMask const& value);

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
