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

#ifndef __LIBEMBER_GLOW_GLOWTEMPLATE_HPP
#define __LIBEMBER_GLOW_GLOWTEMPLATE_HPP

#include "../ber/ObjectIdentifier.hpp"
#include "GlowTemplateBase.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowRootElementCollection;

    /**
     * Class representing a qualified template.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     */
    class LIBEMBER_API GlowTemplate : public GlowTemplateBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a GlowTemplate with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param path The path this node has in the gadget tree.
             */
            explicit GlowTemplate(int number);

            /**
             * Initializes a new GlowTemplate and inserts it to the provided element collection.
             * @param parent The root element collection to insert this node to.
             * @param path The path this node has in the gadget tree.
             */
            GlowTemplate(GlowRootElementCollection* parent, int number);

            /**
             * Constructor, initializes a GlowTemplate with an application tag.
             * @param path The path of this node.
             * @param tag Application tag.
             */
            GlowTemplate(int number, ber::Tag const& tag);

            /**
             * Returns the number of this template.
             * @return The number of this template.
             */
            int number() const;

            /**
             * Returns the tag used for the description.
             * @return The tag used for the description.
             */
            virtual ber::Tag descriptionTag() const;

            /**
             * Returns the tag used for the template element.
             * @return The tag used for the template element.
             */
            virtual ber::Tag elementTag() const;

        private:
            /**
             * This constructor initializes a function without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowTemplate(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowTemplate.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTEMPLATE_HPP
