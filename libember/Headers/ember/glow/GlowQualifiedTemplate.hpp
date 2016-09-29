/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_HPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_HPP

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
    class LIBEMBER_API GlowQualifiedTemplate : public GlowTemplateBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a GlowQualifiedTemplate with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param path The path this node has in the gadget tree.
             */
            explicit GlowQualifiedTemplate(ber::ObjectIdentifier const& path);

            /**
             * Initializes a new GlowQualifiedTemplate and inserts it to the provided element collection.
             * @param parent The root element collection to insert this node to.
             * @param path The path this node has in the gadget tree.
             */
            GlowQualifiedTemplate(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path);

            /**
             * Constructor, initializes a GlowQualifiedTemplate with an application tag.
             * @param path The path of this node.
             * @param tag Application tag.
             */
            GlowQualifiedTemplate(ber::ObjectIdentifier const& path, ber::Tag const& tag);

            /**
             * Returns the path of this template.
             * @return The path of this template.
             */
            ber::ObjectIdentifier path() const;

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
            explicit GlowQualifiedTemplate(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowQualifiedTemplate.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDTEMPLATE_HPP
