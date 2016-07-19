/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_CONTAINER_HPP
#define __LIBEMBER_GLOW_CONTAINER_HPP

#include "../dom/Sequence.hpp"
#include "util/Find.hpp"
#include "traits/ValueTypeToBerType.hpp"
#include "GlowTags.hpp"
#include "GlowType.hpp"

namespace libember { namespace glow
{
    /**
     * Base class for all glow object types.
     */
    class LIBEMBER_API GlowContainer : public dom::Sequence
    {
        protected:
            /**
             * Initializes a new container with a glow type and application tag.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @param tag The application tag.
             */
            GlowContainer(GlowType const& type, ber::Tag const& tag);

            /**
             * Initializes a new container only with a type.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @note The application tag will be set to ContextSpecific-0.
             */
            explicit GlowContainer(GlowType const& type);

            /**
             * Returns the universal tag of this container.
             * @return The universal tag of this container.
             */
            virtual ber::Tag typeTagImpl() const;

        protected:
            /**
             * Since the entries within a GlowContainer need to be ordered by tag
             * in the ASN.1 notation of Ember+, this override does exactly that.
             * The provided iterator will be ignored. Instead, the node's tag determines
             * the insert location of the node.
             * @param child The node to insert.
             */
            virtual iterator insertImpl(iterator const&, Node* child);

        private:
            /** Prohibit assignment */
            GlowContainer& operator=(GlowContainer const&);

        private:
            ber::Tag const m_universalTag;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowContainer.ipp"
#endif

#endif  // __LIBEMBER_GLOW_CONTAINER_HPP


