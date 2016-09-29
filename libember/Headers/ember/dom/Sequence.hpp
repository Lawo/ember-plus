/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_SEQUENCE_HPP
#define __LIBEMBER_DOM_SEQUENCE_HPP

#include "detail/ListContainer.hpp"

namespace libember { namespace dom
{
    /**
     * A node type for SEQUENCE containers.
     */
    class LIBEMBER_API Sequence
        : public detail::ListContainer
    {
        public:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit Sequence(ber::Tag tag);

            /**
             * Covariant override of Node::clone()
             * @see Node::clone()
             */
            virtual Sequence* clone() const;

        protected:
            /** @see Node::typeTagImpl() */
            virtual ber::Tag typeTagImpl() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Sequence.ipp"
#endif

#endif  // __LIBEMBER_DOM_SEQUENCE_HPP

