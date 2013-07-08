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

#ifndef __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP
#define __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP

#include "../dom/NodeFactory.hpp"

namespace libember { namespace glow
{
    /**
     * Implementation of the NodeFactory. This class creates the types
     * defined in Glow when decoding a message.
     */
    class LIBEMBER_API GlowNodeFactory : public dom::NodeFactory
    {
        public:
            /**
             * Returns the singleton instance of this factory.
             * @return The singleton instance of this factory.
             */
            static dom::NodeFactory& getFactory();

            /**
             * Creates a Glow specific type.
             * @param type The application defined type decoded by the reader.
             * @param tag The application tag.
             * @return A Glow type or null if the provided type is unknown.
             */
            dom::Node* createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const;

        private:
            /** Private constructor. **/
            GlowNodeFactory();
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowNodeFactory.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP


