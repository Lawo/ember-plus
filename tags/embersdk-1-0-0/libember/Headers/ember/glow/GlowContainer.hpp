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

#ifndef __LIBEMBER_GLOW_CONTAINER_HPP
#define __LIBEMBER_GLOW_CONTAINER_HPP

#include "../dom/Sequence.hpp"
#include "detail/NodeApplicationTagEquals.hpp"
#include "traits/ValueTypeToBerType.hpp"
#include "GlowTags.hpp"
#include "GlowType.hpp"

namespace libember { namespace glow
{
    /**
     * Base class for all glow object types.
     */
    class LIBEMBER_API GlowContainer : public dom::Sequence
    {
        protected:
            /**
             * Initializes a new container with a glow type and application tag.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @param tag The application tag.
             */
            GlowContainer(GlowType const& type, ber::Tag const& tag);

            /**
             * Initializes a new container only with a type.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @note The application tag will be set to ContextSpecific-0.
             */
            explicit GlowContainer(GlowType const& type);

            /**
             * Returns the universal tag of this container.
             * @return The universal tag of this container.
             */
            virtual ber::Tag typeTagImpl() const;

        private:
            /** Prohibit assignment */
            GlowContainer& operator=(GlowContainer const&);

        private:
            ber::Tag const m_universalTag;
    };

    /**
     * Searches for a node with the specified application tag and tries 
     * to cast it to the specified type.
     * @param first Start element of the container to traverse.
     * @param last End element of the container to traverse
     * @param tag Tag to look for
     * @return If the node exists, this method returns a pointer to the node. Otherwise, it 
     *      returns null.
     */
    template<typename GlowType>
    GlowType* find_node(dom::Container::iterator first, dom::Container::iterator last, ber::Tag const& tag);

    /**
     * Searches for a node with the specified application tag and tries 
     * to cast it to the specified type.
     * @param first Start element of the container to traverse.
     * @param last End element of the container to traverse
     * @param tag Tag to look for
     * @return If the node exists, this method returns a pointer to the node. Otherwise, it 
     *      returns null.
     */
    template<typename GlowType>
    GlowType const* find_node(dom::Container::const_iterator first, dom::Container::const_iterator last, ber::Tag const& tag);


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename GlowType>
    inline GlowType* find_node(dom::Container::iterator first, dom::Container::iterator last, ber::Tag const& tag)
    {
        dom::Container::iterator const it = std::find_if(first, last, detail::NodeApplicationTagEquals(tag));
        return it != last ? &dynamic_cast<GlowType&>(*it) : 0;
    }

    template<typename GlowType>
    inline GlowType const* find_node(dom::Container::const_iterator first, dom::Container::const_iterator last, ber::Tag const& tag) 
    {
        dom::Container::const_iterator const it = std::find_if(first, last, detail::NodeApplicationTagEquals(tag));
        return it != last ? &dynamic_cast<GlowType const&>(*it) : 0;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowContainer.ipp"
#endif

#endif  // __LIBEMBER_GLOW_CONTAINER_HPP


