/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP
#define __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP

#include "../dom/NodeFactory.hpp"

namespace libember { namespace glow
{
    /**
     * Implementation of the NodeFactory. This class creates the types
     * defined in Glow when decoding a message.
     */
    class LIBEMBER_API GlowNodeFactory : public dom::NodeFactory
    {
        public:
            /**
             * Returns the singleton instance of this factory.
             * @return The singleton instance of this factory.
             */
            static dom::NodeFactory& getFactory();

            /**
             * Creates a Glow specific type.
             * @param type The application defined type decoded by the reader.
             * @param tag The application tag.
             * @return A Glow type or null if the provided type is unknown.
             */
            dom::Node* createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const;

        private:
            /** Private constructor. **/
            GlowNodeFactory();
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowNodeFactory.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWNODEFACTORY_HPP


