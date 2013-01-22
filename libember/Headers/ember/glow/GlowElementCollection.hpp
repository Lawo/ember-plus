#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowElement;

    /**
     * An Element Collection is usually used as root element and also used
     * by nodes and parameters to store children and commands.
     */
    class LIBEMBER_API GlowElementCollection : public GlowContainer
    {
        public:
            /**
             * Creates a root element collection where the tag is set to GlowTags::Root() (Application - 30).
             * This collection is the root element of all glow messages.
             * @return Returns a new element collection which is tagged as root.
             */
            static GlowElementCollection* createRoot();

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
             * Adds a new glow element into the collection.
             * @param element Element to append to the collection.
             */
            void insertElement(GlowElement* element);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowElementCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_HPP
