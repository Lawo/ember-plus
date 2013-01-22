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

#ifndef __LIBEMBER_DOM_SEQUENCE_HPP
#define __LIBEMBER_DOM_SEQUENCE_HPP

#include "detail/ListContainer.hpp"

namespace libember { namespace dom
{
    /**
     * A node type for SEQUENCE containers.
     */
    class LIBEMBER_API Sequence
        : public detail::ListContainer
    {
        public:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit Sequence(ber::Tag tag);

            /**
             * Covariant override of Node::clone()
             * @see Node::clone()
             */
            virtual Sequence* clone() const;

        protected:
            /** @see Node::typeTagImpl() */
            virtual ber::Tag typeTagImpl() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Sequence.ipp"
#endif

#endif  // __LIBEMBER_DOM_SEQUENCE_HPP

