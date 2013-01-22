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
     *
     */
    class LIBEMBER_API NodeFactory
    {
        public:
            virtual ~NodeFactory();

            virtual Node* createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const = 0;

        protected:
            Node* createSet(ber::Tag const& tag) const;

            Node* createSequence(ber::Tag const& tag) const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/NodeFactory.ipp"
#endif

#endif  // __LIBEMBER_DOM_NODEFACTORY_HPP

