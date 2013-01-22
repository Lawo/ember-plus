#ifndef __TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H
#define __TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H

#include <algorithm>
#include "../Collection.h"
#include "../Node.h"
#include "../Parameter.h"

namespace gadget { namespace util
{
    class NumberFactory
    {
        private:
            template<typename ValueType>
            static int max(Collection<ValueType> const& collection)
            {
                auto const first = std::begin(collection);
                auto const last = std::end(collection);
                auto result = std::max_element(first, last, [](ValueType c, ValueType n) -> bool { return n->number() > c->number(); });

                return result != last ? (*result)->number() : 0;
            }


        public:
            static int create(Node const* node)
            {
                auto const& nodes = node->nodes();
                auto const& params = node->parameters();

                auto const max = std::max(NumberFactory::max(nodes), NumberFactory::max(params));
                return 1 + max;
            }
    };
}
}
#endif//__TINYEMBER_GADGET_UTIL_NUMBERFACTORY_H
