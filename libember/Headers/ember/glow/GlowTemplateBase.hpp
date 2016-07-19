/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTEMPLATEBASE_HPP
#define __LIBEMBER_GLOW_GLOWTEMPLATEBASE_HPP

#include "GlowElement.hpp"
#include "GlowElementCollection.hpp"

namespace libember { namespace glow
{
    /**
     * Class containing the common properties of a template. A template contains a description
     * and an element, which describes the structure of a node using that template.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowTemplateBase : public GlowElement
    {
        public:
            /**
             * Sets the description string.
             * @param description The description string to set.
             */
            void setDescription(std::string const& description);

            /**
             * Sets the element describing the structure of this template.
             * @param description The template element.
             */
            void setElement(GlowElement* element);

            /**
             * Returns the description string.
             * @return The description string.
             */
            std::string description() const;

            /**
             * Returns the template element.
             * @return The template element.
             */
            GlowElement const* element() const;

            /**
             * Returns the tag used for the description.
             * @return The tag used for the description.
             */
            virtual ber::Tag descriptionTag() const = 0;

            /**
             * Returns the tag used for the template element.
             * @return The tag used for the template element.
             */
            virtual ber::Tag elementTag() const = 0;

        protected:
            /**
             * This constructor initializes a node with the specified content- and children-tags.
             * @param type Object type.
             * @param tag Application tag.
             * @param contentsTag The tag to use for the contents set.
             * @param childrenTag The tag to use for the children.
             */
            GlowTemplateBase(GlowType const& type, ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowNodeBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTEMPLATEBASE_HPP

