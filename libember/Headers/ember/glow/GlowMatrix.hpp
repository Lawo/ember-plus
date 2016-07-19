/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWMATRIX_HPP
#define __LIBEMBER_GLOW_GLOWMATRIX_HPP

#include "GlowMatrixBase.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowElementCollection;
    class GlowRootElementCollection;
    class GlowNodeBase;

    /**
     * Class representing a single node.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowMatrix : public GlowMatrixBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a Glow matrix with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param number The internal node number.
             */
            explicit GlowMatrix(int number);

            /**
             * Initializes a new Glow matrix with the specified number and adds it as child node
             * to the provided parent.
             * @param parent The root collection owning this node.
             * @param number The internal node number.
             */
            GlowMatrix(GlowRootElementCollection* parent, int number);

            /**
             * Initializes a new Glow matrix with the specified number and adds it as child node
             * to the provided parent.
             * @param parent The node owning this node.
             * @param number The internal node number.
             */
            GlowMatrix(GlowNodeBase* parent, int number);

            /**
             * Constructor, initializes a Glow matrix with an application tag.
             * @param number The internal node number.
             * @param tag Application tag.
             */
            GlowMatrix(int number, ber::Tag const& tag);

            /**
             * Returns the number of this node.
             * @return The node number.
             */
            int number() const;

        private:
            /**
             * This constructor initializes a matrix without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowMatrix(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowMatrix.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWMATRIX_HPP

