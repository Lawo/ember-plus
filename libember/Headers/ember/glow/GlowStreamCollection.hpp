#ifndef __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP

#include "GlowElement.hpp"

namespace libember { namespace glow
{
    /**
     * The StreamCollection contains a list of StreamEntries.
     */
    class LIBEMBER_API GlowStreamCollection : public GlowElement
    {
        public:
            /**
             * Initializes an empty stream collection with the provided application tag.
             * @param tag Application tag.
             */
            explicit GlowStreamCollection(ber::Tag const& tag);

            /**
             * Inserts a new StreamEntry into the collection.
             * @param streamIdentifier The stream identifier.
             * @param value The stream value.
             */
            void insert(int streamIdentifier, int value);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowStreamCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP
