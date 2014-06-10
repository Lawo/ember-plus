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

#ifndef __LIBEMBER_DOM_NODEFACTORY_HPP
#define __LIBEMBER_DOM_NODEFACTORY_HPP

#include "../util/Api.hpp"
#include "../util/OctetStream.hpp"
#include "../ber/Tag.hpp"
#include "../ber/Type.hpp"

namespace libember { namespace dom
{
    class Container;
    class Node;

    /**
     * NodeFactory interface. This class is used by the DomReader in order to create
     * the decoded objects. 
     */
    class LIBEMBER_API NodeFactory
    {
        public:
            /** Destructor */
            virtual ~NodeFactory();

            /**
             * This method is called by the reader when in detected an application defined node type.
             * @param type The decoded object type.
             * @param tag The decoded object application tag.
             */
            virtual Node* createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const = 0;

        protected:
            /**
             * Creates a default set with the specified application tag.
             * @param tag Application tag
             * @return Returns a Set.
             */
            Node* createSet(ber::Tag const& tag) const;

            /**
             * Creates a default sequence with the specified application tag.
             * @param tag Application tag
             * @return Returns a Sequence.
             */
            Node* createSequence(ber::Tag const& tag) const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/NodeFactory.ipp"
#endif

#endif  // __LIBEMBER_DOM_NODEFACTORY_HPP

