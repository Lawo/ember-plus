/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP

#include "GlowContainer.hpp"
#include "util/TypeFilter.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowElement;

    /**
     * An Element Collection is used by content elements to store children.
     */
    class LIBEMBER_API GlowElementCollection : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes a new element collection with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             */
            GlowElementCollection();

            /**
             * Constructor, initializes a new element collection with an application tag.
             * @param tag Application tag to set.
             */
            explicit GlowElementCollection(ber::Tag const& tag);

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
    inline GlowContainer::size_type GlowElementCollection::elements(OutputIterator dest) const
    {
        return util::TypeFilter<GlowElement>::collect(begin(), end(), dest);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowElementCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
