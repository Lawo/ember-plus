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
