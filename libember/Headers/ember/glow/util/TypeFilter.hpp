/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_UTIL_TYPEFILTER_HPP
#define __LIBEMBER_GLOW_UTIL_TYPEFILTER_HPP

#include <memory>
#include "../../dom/Node.hpp"

namespace libember { namespace glow { namespace util
{
    /**
     * Helper class to extract typed collections. ValueType needs to
     * be derived from dom::Node.
     */
    template<typename ValueType>
    struct TypeFilter
    {
        public:
            /**
             * Collects all instances of type ValueType const* from the passed
             * collection. ValueType must be derived from dom::Node.
             * @param first iterator returned by a begin() method.
             * @param last iterator returned by a end() method.
             * @param dest iterator of the output collection.
             */
            template<typename InputIterator, typename OutputIterator>
            static std::size_t collect(InputIterator first, InputIterator last, OutputIterator dest)
            {
                std::size_t size = 0;
                for (; first != last; ++first)
                {
                    dom::Node const* node = &(*first);
                    ValueType const* type = dynamic_cast<ValueType const*>(node);

                    if (type != 0)
                    {
                        *dest++ = type;
                        size++;
                    }
                }

                return size;
            }
    };
}
}
}

#endif //__LIBEMBER_GLOW_UTIL_TYPEFILTER_HPP
