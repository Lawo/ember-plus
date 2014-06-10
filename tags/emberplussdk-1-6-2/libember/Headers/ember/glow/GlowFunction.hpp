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

#ifndef __LIBEMBER_GLOW_GLOWFUNCTION_HPP
#define __LIBEMBER_GLOW_GLOWFUNCTION_HPP

#include "GlowFunctionBase.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;
    class GlowElementCollection;
    class GlowRootElementCollection;
    class GlowNodeBase;

    /**
     * Class representing a single function.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowFunction : public GlowFunctionBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a GlowFunction with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param number The internal function number.
             */
            explicit GlowFunction(int number);

            /**
             * Initializes a new GlowFunction with the specified number and adds it as child node
             * to the provided parent.
             * @param parent The root collection owning this node.
             * @param number The internal node number.
             */
            GlowFunction(GlowRootElementCollection* parent, int number);

            /**
             * Initializes a new GlowFunction with the specified number and adds it as child node
             * to the provided parent.
             * @param parent The node owning this node.
             * @param number The internal node number.
             */
            GlowFunction(GlowNodeBase* parent, int number);

            /**
             * Constructor, initializes a GlowFunction with an application tag.
             * @param number The internal node number.
             * @param tag Application tag.
             */
            GlowFunction(int number, ber::Tag const& tag);

            /**
             * Returns the number of this node.
             * @return The node number.
             */
            int number() const;

        private:
            /**
             * This constructor initializes a function without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowFunction(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowFunction.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWFUNCTION_HPP

