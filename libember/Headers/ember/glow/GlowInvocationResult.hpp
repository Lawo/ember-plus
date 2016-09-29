/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP
#define __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP

#include "util/TypeFilter.hpp"
#include "GlowTupleItemDescription.hpp"
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
             * Fills the result sequence with dom::Nodes representing the passed
             * glow::Values.
             * @param firstValue Iterator pointing to the first glow::Value.
             * @param lastValue Iterator pointing behind the last glow::Value.
             */
            template<typename InputIterator>
            void setTypedResult(InputIterator firstValue, InputIterator lastValue);

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
             * Copies the values of all leaves in the result sequence to the passed vector
             * @param dest The glow::Value iterator to copy the result values to.
             */
            template<typename OutputIterator>
            size_type typedResult(OutputIterator dest) const;
    };

    template<typename InputIterator>
    inline void GlowInvocationResult::setTypedResult(InputIterator firstValue, InputIterator lastValue)
    {
        dom::Sequence* container = result();

        //SimianIgnore
        // GlowInvocation
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
    inline GlowInvocationResult::size_type GlowInvocationResult::typedResult(OutputIterator dest) const
    {

        //SimianIgnore

        dom::Sequence const* const container = result();
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
#  include "impl/GlowInvocationResult.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWINVOCATIONRESULT_HPP
