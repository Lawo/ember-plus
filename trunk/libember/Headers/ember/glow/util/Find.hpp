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

#ifndef __LIBEMBER_GLOW_UTIL_FIND_HPP
#define __LIBEMBER_GLOW_UTIL_FIND_HPP

#include <algorithm>
#include "../../dom/Container.hpp"

namespace libember { namespace glow { namespace util
{
    namespace traits
    {
        namespace detail
        {
            /** 
             * Struture that provides the () operator to compare a node's application tag with
             * another tag.
             */
            struct NodeApplicationTagEquals
            {
                /** 
                    * Constructor, initializes the structure with the tag to compare 
                    * a node's application tag with.
                    * @param tag Application tag.
                    */
                explicit NodeApplicationTagEquals(ber::Tag const& tag)
                    : tag(tag)
                {}

                /** 
                    * Compares the application tag of the node with the tag stored in this instance.
                    * @param node Node to get the application tag from.
                    * @return Returns true if the application tag equals the one that has been passed
                    *      to the constructor, false otherwise.
                    */
                bool operator()(dom::Node const& node) const
                {
                    return tag == node.applicationTag();
                }
        
                ber::Tag tag;
            }
        }

        /**
         * Default traits for a collection of node pointers.
         */
        template<typename Iterator>
        struct Find
        {
            /**
             * Searches for a node with the passed application tag.
             * @param first The reference to the first node pointer in the collection.
             * @param last A reference to the first element beyond the node collection.
             * @param tag The tag to look for.
             * @return Returns last if no node with the specified application tag has been found.
             *      Otherwise, this method returns an iterator that points to the node with the 
             *      requested tag.
             */
            static Iterator execute(Iterator const& first, Iterator const& last, ber::Tag const& tag)
            {
                for ( ; first != last; ++first)
                {
                    if ((*first)->applicationTag() == tag)
                    {
                        break;
                    }
                }
                return first;
            }
        }

        /**
         * Specialization for the default const_iterator of a dom::Container class.
         */
        template<>
        struct Find<dom::Container::const_iterator>
        {
            typedef dom::Container::const_iterator iterator;

            /**
             * Searches for a node with the passed application tag.
             * @param first The reference to the first node pointer in the collection.
             * @param last A reference to the first element beyond the node collection.
             * @param tag The tag to look for.
             * @return Returns last if no node with the specified application tag has been found.
             *      Otherwise, this method returns an iterator that points to the node with the 
             *      requested tag.
             */
            static iterator execute(iterator const& first, iterator const& last, ber::Tag const& tag)
            {
                iterator const result = std::find_if(first, last, detail::NodeApplicationTagEquals(tag));
                return result;
            }
        }

        /**
         * Specialization for the default iterator of a dom::Container class.
         */
        template<>
        struct Find<dom::Container::iterator>
        {
            typedef dom::Container::iterator iterator;

            /**
             * Searches for a node with the passed application tag.
             * @param first The reference to the first node pointer in the collection.
             * @param last A reference to the first element beyond the node collection.
             * @param tag The tag to look for.
             * @return Returns last if no node with the specified application tag has been found.
             *      Otherwise, this method returns an iterator that points to the node with the 
             *      requested tag.
             */
            static iterator execute(iterator const& first, iterator const& last, ber::Tag const& tag)
            {
                iterator const result = std::find_if(first, last, detail::NodeApplicationTagEquals(tag));
                return result;
            }
        }
    }

    /**
     * Searches for a node with the specified application tag.
     * @param first The reference to the first node in the collection.
     * @param last A reference to the first element beyond the node collection.
     * @param tag The tag to look for.
     * @return Returns last if no node with the specified application tag has been found.
     *      Otherwise, this method returns an iterator that points to the node with the 
     *      requested tag.
     */
    template<typename InputIterator>
    inline InputIterator find_tag(InputIterator const& first, InputIterator const& last, ber::Tag const& tag)
    {
        return traits::Find<InputIterator>::execute(first, last, tag);
    }
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_FIND_HPP
