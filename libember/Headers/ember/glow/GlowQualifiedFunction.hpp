/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_HPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_HPP

#include "../ber/ObjectIdentifier.hpp"
#include "GlowFunctionBase.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowRootElementCollection;

    /**
     * Class representing a qualified function.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowQualifiedFunction : public GlowFunctionBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a GlowQualifiedFunction with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param path The path this node has in the gadget tree.
             */
            explicit GlowQualifiedFunction(ber::ObjectIdentifier const& path);

            /**
             * Initializes a new GlowQualifiedFunction and inserts it to the provided element collection.
             * @param parent The root element collection to insert this node to.
             * @param path The path this node has in the gadget tree.
             */
            GlowQualifiedFunction(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path);

            /**
             * Constructor, initializes a GlowQualifiedFunction with an application tag.
             * @param path The path of this node.
             * @param tag Application tag.
             */
            GlowQualifiedFunction(ber::ObjectIdentifier const& path, ber::Tag const& tag);

            /**
             * Returns the path of this function.
             * @return The path of this function.
             */
            ber::ObjectIdentifier path() const;

        private:
            /**
             * This constructor initializes a function without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowQualifiedFunction(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowQualifiedFunction.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDFUNCTION_HPP
