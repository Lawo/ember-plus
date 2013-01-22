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

#ifndef __LIBEMBER_GLOW_DETAIL_NODEAPPLICATIONTAGEQUALS_HPP
#define __LIBEMBER_GLOW_DETAIL_NODEAPPLICATIONTAGEQUALS_HPP

#include "../../dom/Node.hpp"

namespace libember { namespace glow { namespace detail
{
    /** 
     * Struture that provides the () operator to compare a node's application tag with
     * another tag.
     */
    struct NodeApplicationTagEquals
    {
        /** 
         * Constructor, initializes the structure with the tag to compare 
         * a node's application tag with.
         * @param tag Application tag.
         */
        explicit NodeApplicationTagEquals(ber::Tag const& tag)
            : tag(tag)
        {}

        /** 
         * Compares the application tag of the node with the tag stored in this instance.
         * @param node Node to get the application tag from.
         * @return Returns true if the application tag equals the one that has been passed
         *      to the constructor, false otherwise.
         */
        bool operator()(dom::Node const& node) const
        {
            return tag == node.applicationTag();
        }
        
        ber::Tag tag;
    };
}
}
}

#endif  // __LIBEMBER_GLOW_DETAIL_NODEAPPLICATIONTAGEQUALS_HPP
