/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_HPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_HPP

#include "../ber/ObjectIdentifier.hpp"
#include "GlowMatrixBase.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowRootElementCollection;

    /**
     * Class representing a single matrix.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowQualifiedMatrix : public GlowMatrixBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a Glow matrix with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param path The path this matrix has in the gadget tree.
             */
            explicit GlowQualifiedMatrix(ber::ObjectIdentifier const& path);

            /**
             * Initializes a new matrix and inserts it to the provided element collection.
             * @param parent The root element collection to insert this matrix to.
             * @param path The path this matrix has in the gadget tree.
             */
            GlowQualifiedMatrix(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path);

            /**
             * Constructor, initializes a Glow matrix with an application tag.
             * @param path The path of this matrix.
             * @param tag Application tag.
             */
            GlowQualifiedMatrix(ber::ObjectIdentifier const& path, ber::Tag const& tag);

            /**
             * Returns the path of this matrix.
             * @return The path of this matrix.
             */
            ber::ObjectIdentifier path() const;

        private:
            /**
             * This constructor initializes a qualified matrix without the mandatory path.
             * It is only used by the NodeFactory since the path has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowQualifiedMatrix(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowQualifiedMatrix.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDMATRIX_HPP
