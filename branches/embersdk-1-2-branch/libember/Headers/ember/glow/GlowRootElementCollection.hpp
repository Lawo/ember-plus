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

#ifndef __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP

#include "GlowContainer.hpp"
#include "util/TypeFilter.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowElement;
    class GlowNodeFactory;

    /**
     * An Element Collection is usually used as root element and also used
     * by nodes and parameters to store children and commands.
     */
    class LIBEMBER_API GlowRootElementCollection : public GlowContainer
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Creates a root element collection where the tag is set to GlowTags::Root() (Application - 30).
             * This collection is the root element of all glow messages. The returned instance must be deleted manually when it
             * is no longer needed.
             * @return Returns a new element collection which is tagged as root.
             */
            static GlowRootElementCollection* create();

        private:
            /**
             * Constructor, initializes a new element collection with the application tag set to C-0.
             * @note The application tag will be set to ContextSpecific-0.
             */
            GlowRootElementCollection();

            /**
             * Constructor, initializes a new element collection with an application tag.
             * @param tag Application tag to set.
             */
            explicit GlowRootElementCollection(ber::Tag const& tag);

            /**
             * Extracts all GlowElement objects from this sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type elements(OutputIterator dest) const;
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowRootElementCollection::elements(OutputIterator dest) const
    {
        return util::TypeFilter<GlowElement>::collect(begin(), end(), dest);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowRootElementCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP
