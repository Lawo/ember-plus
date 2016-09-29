/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_NODEFACTORY_HPP
#define __LIBEMBER_DOM_NODEFACTORY_HPP

#include "../util/Api.hpp"
#include "../util/OctetStream.hpp"
#include "../ber/Tag.hpp"
#include "../ber/Type.hpp"

namespace libember { namespace dom
{
    class Container;
    class Node;

    /**
     * NodeFactory interface. This class is used by the DomReader in order to create
     * the decoded objects. 
     */
    class LIBEMBER_API NodeFactory
    {
        public:
            /** Destructor */
            virtual ~NodeFactory();

            /**
             * This method is called by the reader when in detected an application defined node type.
             * @param type The decoded object type.
             * @param tag The decoded object application tag.
             */
            virtual Node* createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const = 0;

        protected:
            /**
             * Creates a default set with the specified application tag.
             * @param tag Application tag
             * @return Returns a Set.
             */
            Node* createSet(ber::Tag const& tag) const;

            /**
             * Creates a default sequence with the specified application tag.
             * @param tag Application tag
             * @return Returns a Sequence.
             */
            Node* createSequence(ber::Tag const& tag) const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/NodeFactory.ipp"
#endif

#endif  // __LIBEMBER_DOM_NODEFACTORY_HPP

