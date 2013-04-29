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

#ifndef __LIBEMBER_GLOW_GLOWINVOCATION_HPP
#define __LIBEMBER_GLOW_GLOWINVOCATION_HPP

#include "GlowContainer.hpp"
#include "Value.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowInvocation represents a request to execute a function.
     */
    class LIBEMBER_API GlowInvocation : public GlowContainer
    {
        public:
            /**
             * Initializes a new GlowInvocation.
             * @param tag The application tag to set.
             */
            explicit GlowInvocation(ber::Tag const& tag = GlowTags::Command::Invocation());

            /**
             * Sets the invocation identifier.
             * @param id The identifier to set.
             */
            void setInvocationId(int id);

            /**
             * Returns a modifiable collection of arguments. If no arguments are set yet, the
             * container will be created.
             * @return The collection of arguments.
             */
            dom::Sequence* arguments();

            /**
             * Returns the invocation identifier of this request. If no identifier is set,
             * this method returns -1.
             */
            int invocationId() const;

            /**
             * Returns the collection of arguments. If no arguments are set, this method
             * returns null.
             * @return The collection of arguments.
             */
            dom::Sequence const* arguments() const;

            /**
             * Copies all arguments that are of type GlowTupleItemDescription into the
             * passed output iterator.
             * @param dest The iterator to copy the valid arguments to.
             */
            template<typename OutputIterator>
            size_type typedArguments(OutputIterator dest) const;
    };

    template<typename OutputIterator>
    inline GlowInvocation::size_type GlowInvocation::typedArguments(OutputIterator dest) const
    {
        dom::Sequence const* container = arguments();

        return container != 0
            ? util::TypeFilter<GlowTupleItemDescription>::collect(container->begin(), container->end(), dest)
            : 0;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWINVOCATION_HPP
