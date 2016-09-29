/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWINVOCATION_HPP
#define __LIBEMBER_GLOW_GLOWINVOCATION_HPP

#include "util/TypeFilter.hpp"
#include "GlowTupleItemDescription.hpp"
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
             * Fills the arguments sequence with dom::Nodes representing the passed
             * glow::Values.
             * @param firstValue Iterator pointing to the first glow::Value.
             * @param lastValue Iterator pointing behind the last glow::Value.
             */
            template<typename InputIterator>
            void setTypedArguments(InputIterator firstValue, InputIterator lastValue);

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
             * Copies the values of all leaves in the arguments sequence to the passed vector
             * @param dest The glow::Value iterator to copy the argument values to.
             */
            template<typename OutputIterator>
            size_type typedArguments(OutputIterator dest) const;
    };

    template<typename InputIterator>
    inline void GlowInvocation::setTypedArguments(InputIterator firstValue, InputIterator lastValue)
    {
        dom::Sequence* container = arguments();

        //SimianIgnore
        container->clear();

        for( ; firstValue != lastValue; ++firstValue)
        {
            ber::Value berValue = (*firstValue).toBerValue();
            dom::Node* node = new dom::VariantLeaf(GlowTags::ElementDefault(), berValue);

            container->insert(container->end(), node);
        }
        //EndSimianIgnore
    }

    template<typename OutputIterator>
    inline GlowInvocation::size_type GlowInvocation::typedArguments(OutputIterator dest) const
    {

        //SimianIgnore

        dom::Sequence const* const container = arguments();
        size_type size = 0;

        if(container != 0)
        {
            dom::Container::const_iterator first = container->begin();
            dom::Container::const_iterator last = container->end();

            for( ; first != last; ++first)
            {
                dom::VariantLeaf const* const node = dynamic_cast<libember::dom::VariantLeaf const*>(&*first);
                if (node != 0)
                {
                    *dest++ = Value(node->value());
                    ++size;
                }
            }
        }

        return size;

        //EndSimianIgnore
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowInvocation.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWINVOCATION_HPP
