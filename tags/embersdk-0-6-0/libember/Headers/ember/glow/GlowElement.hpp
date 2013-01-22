#ifndef __LIBEMBER_GLOW_GLOWELEMENT_HPP
#define __LIBEMBER_GLOW_GLOWELEMENT_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * The Glow Element is the base class for Nodes, Parameters and Commands.
     */
    class LIBEMBER_API GlowElement : public GlowContainer
    {
        protected:
            /**
             * Initializes an new element with the provide type.
             * @note The application tag will be set to ContextSpecific-0.
             * @param type The glow type to set.
             */
            explicit GlowElement(GlowType const& type);

            /**
             * Constructor, initializes a new element with the provided type
             * and application tag.
             * @param type The glow type to set.
             * @param tag The application tag to initialize the container with.
             */
            GlowElement(GlowType const& type, ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowElement.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWELEMENT_HPP
