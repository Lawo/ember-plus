/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP

#include "GlowContainer.hpp"
#include "util/TypeFilter.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowElement;
    class GlowNodeFactory;

    /**
     * An Element Collection is usually used as root element and also used
     * by nodes and parameters to store children and commands.
     */
    class LIBEMBER_API GlowRootElementCollection : public GlowContainer
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Creates a root element collection where the tag is set to GlowTags::Root() (Application - 30).
             * This collection is the root element of all glow messages. The returned instance must be deleted manually when it
             * is no longer needed.
             * @deprecated Use the default constructor instead.
             * @return Returns a new element collection which is tagged as root.
             */
            static GlowRootElementCollection* create();

            /**
             * Constructor, initializes a new element collection with the application tag set to GlowTags::Root() (Application - 30).
             * @note This collection is the root element of all glow messages.
             */
            GlowRootElementCollection();

        private:
            /**
             * Constructor, initializes a new element collection with an application tag.
             * @param tag Application tag to set.
             */
            explicit GlowRootElementCollection(ber::Tag const& tag);

            /**
             * Extracts all GlowElement objects from this sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type elements(OutputIterator dest) const;
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowRootElementCollection::elements(OutputIterator dest) const
    {
        return util::TypeFilter<GlowElement>::collect(begin(), end(), dest);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowRootElementCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWROOTELEMENTCOLLECTION_HPP
