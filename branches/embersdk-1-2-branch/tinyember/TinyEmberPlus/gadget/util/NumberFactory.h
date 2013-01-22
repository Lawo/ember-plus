#ifndef __TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H
#define __TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H

#include <algorithm>
#include "../Collection.h"
#include "../Node.h"
#include "../Parameter.h"

namespace gadget { namespace util
{
    /**
     * Simple helper class which computes a number for a newly created node or parameter.
     */
    class NumberFactory
    {
        public:
            /**
             * Generates a new number that is still not used within the child nodes or parameters 
             * of the passed node.
             * @param node The node that is used as parent for a new node or parameter.
             * @return A new number which can be used for a newly created child node or parameter.
             */
            static int create(Node const* node)
            {
                auto const& nodes = node->nodes();
                auto const& params = node->parameters();

                auto const max = std::max(NumberFactory::max(nodes), NumberFactory::max(params));
                return 1 + max;
            }

        private:
            /**
             * Searches for largest number that is already in use.
             * @param collection The collection to traverse. The specified ValueType must be either Parameter or Node.
             * @return The largest number within the collection.
             */
            template<typename ValueType>
            static int max(Collection<ValueType> const& collection)
            {
                auto const first = std::begin(collection);
                auto const last = std::end(collection);
                auto result = std::max_element(first, last, [](ValueType c, ValueType n) -> bool { return n->number() > c->number(); });

                return result != last ? (*result)->number() : 0;
            }
    };
}
}
#endif//__TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H
