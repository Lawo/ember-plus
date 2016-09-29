/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_ASYNCDOMREADER_HPP
#define __LIBEMBER_DOM_ASYNCDOMREADER_HPP

#include "../util/Api.hpp"
#include "../ber/Encoding.hpp"
#include "../ber/Length.hpp"
#include "AsyncBerReader.hpp"

namespace libember { namespace dom
{
    class Container;
    class Node;
    class NodeFactory;

    /**
     * Implementation of an Async Dom reader which reconstructs
     * a tree from the provided data.
     */
    class LIBEMBER_API AsyncDomReader : public AsyncBerReader
    {
        public:
            /**
             * Initializes a new AsyncDomReader instance.
             * @param factory Reference to the node factory to use.
             */
            explicit AsyncDomReader(dom::NodeFactory const& factory);

            /**
             * Destructor, deletes the root node, if set.
             */
            virtual ~AsyncDomReader();

            /**
             * Returns true when the root node has been decoded.
             */
            bool isRootReady() const;

            /**
             * When a root has been decoded, this method returns the 
             * root node and resets its internal state. In that case, the returned node
             * muste be deleted manually when it is no longer being used.
             * @return The root node if already decoded, otherwise null.
             */
            dom::Node* detachRoot();

        protected:
            /**
             * This method is called when a new container node has been decoded. The
             * default implementation is empty.
             * @param node The decoded node.
             */
            virtual void containerReady(dom::Node* node);

            /**
             * This method is called when a new node has been decoded. The
             * default implementation is empty.
             * @param node The decoded node.
             */
            virtual void itemReady(dom::Node* node);

            /**
             * This method is called when the root node has been decoded. The
             * default implementation is empty.
             * @param node The decoded root node.
             */
            virtual void rootReady(dom::Node* node);

            /**
             * Deletes the root node and resets the current node and the isRootReady flag.
             */
            virtual void resetImpl();

            /**
             * Decodes a container node.
             */
            virtual void containerReady();

            /**
             * Updates the current node and attaches it to its parent node.
             */
            virtual void itemReady();

        private:
            /** Prohibit assignments */
            AsyncDomReader& operator=(AsyncDomReader const&);

        private:
            bool m_isRootReady;
            dom::Node* m_root;
            dom::Node* m_current;
            dom::NodeFactory const& m_factory;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/AsyncDomReader.ipp"
#endif

#endif  // __LIBEMBER_DOM_ASYNCDOMREADER_HPP

