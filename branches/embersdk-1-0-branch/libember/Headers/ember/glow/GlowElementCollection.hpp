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

#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowElement;

    /**
     * An Element Collection is usually used as root element and also used
     * by nodes and parameters to store children and commands.
     */
    class LIBEMBER_API GlowElementCollection : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes a new element collection with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             */
            GlowElementCollection();

            /**
             * Constructor, initializes a new element collection with an application tag.
             * @param tag Application tag to set.
             */
            explicit GlowElementCollection(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowElementCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
