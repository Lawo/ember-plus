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

#ifndef __LIBEMBER_DOM_DETAIL_LISTCONTAINER_HPP
#define __LIBEMBER_DOM_DETAIL_LISTCONTAINER_HPP

#include <list>
#include "../Container.hpp"

namespace libember { namespace dom { namespace detail
{
    class LIBEMBER_API ListContainer
        : public Container
    {
        public:
            /**
             * Destructor. Frees all child nodes below this container.
             */
            virtual ~ListContainer();

            /** @see Container::empty() */
            virtual bool empty() const;

            /** @see Container::size() */
            virtual size_type size() const;

            /** @see Container::begin() */
            virtual iterator begin();

            /** @see Container::begin() */
            virtual const_iterator begin() const;

            /** @see Container::end() */
            virtual iterator end();

            /** @see Container::end() */
            virtual const_iterator end() const;

        protected:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit ListContainer(ber::Tag tag);

            /**
             * Copy constructor. Creates a deep copy of the node passed 
             * in @p other and all of its children.
             * @param other a constant reference to the ListContainer node
             *      from whom the created node should be made a copy.
             */
            ListContainer(ListContainer const& other);

            /**
             * Helper function that calculates the sum of the encoded lengths
             * of all contained child nodes and returns it.
             * @return The sum of the encoded lengths of all currently added
             *      child nodes.
             */
            std::size_t encodedPayloadLength() const;

            /**
             * Helper function that sequentially encodes all child nodes to
             * the output stream provided in @p output.
             * @param output a reference to the stream buffer to which all
             *      currently added child nodes should be encoded.
             */
            void encodePayload(util::OctetStream& output) const;

        protected:
            /** @see Node::updateImpl() */
            virtual void updateImpl() const;

            /** @see Node::encodeImpl() */
            virtual void encodeImpl(util::OctetStream& output) const;

            /** @see Node::encodedLengthImpl() */
            virtual std::size_t encodedLengthImpl() const;

            /** @see Container::insertImpl() */
            virtual iterator insertImpl(iterator where, Node* child);

            /** @see Container::eraseImpl() */
            virtual void eraseImpl(iterator first, iterator last);

        private:
            typedef std::list<Node*> NodeList;

        private:
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif
            NodeList m_children;
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
            mutable std::size_t m_cachedLength;
    };

}
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/ListContainer.ipp"
#endif

#endif  // __LIBEMBER_DOM_DETAIL_LISTCONTAINER_HPP

