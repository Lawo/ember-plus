/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP
#define __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP

#include "GlowContainer.hpp"
#include "Value.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowInvocationResult represents the result of a function invocation.
     */
    class LIBEMBER_API GlowInvocationResult : public GlowContainer
    {
        public:
            /**
             * Initializes a new GlowInvocationResult.
             * @param tag The application tag to set.
             */
            explicit GlowInvocationResult(ber::Tag const& tag = GlowTags::Root());

            /**
             * Sets the invocation identifier.
             * @param id The identifier to set.
             */
            void setInvocationId(int id);

            /**
             * Sets the success property of this invocation result.
             * @param success The result state of the invocation.
             */
            void setSuccess(bool success);

            /**
             * Returns the collection of result values. If the container does not
             * yet exist, it will be created.
             * @return The collection of result values.
             */
            dom::Sequence* result();

            /**
             * Returns the invocation identifier of this request. If no identifier is set,
             * this method returns -1.
             */
            int invocationId() const;

            /**
             * Returns the success state of the invocation result.
             * @return The success state of the invocation result.
             */
            bool success() const;

            /**
             * Returns the collection of arguments. If no arguments are set, this method
             * returns null.
             * @return The collection of arguments.
             */
            dom::Sequence const* result() const;

            /**
             * Copies all result types that are of type GlowTupleItemDescription into the
             * passed output iterator.
             * @param dest The iterator to copy the valid result types to.
             */
            template<typename OutputIterator>
            size_type typedResult(OutputIterator dest) const;
    };

    template<typename OutputIterator>
    inline GlowInvocationResult::size_type GlowInvocationResult::typedResult(OutputIterator dest) const
    {
        dom::Sequence const* container = result();

        return container != 0
            ? util::TypeFilter<GlowTupleItemDescription>::collect(container->begin(), container->end(), dest)
            : 0;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP
