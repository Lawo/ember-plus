#ifndef __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP
#define __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowStreamEntry represents a single audio level meter entry.
     * It contains a unique stream identifier and its current value in 1/32 dB steps.
     */
    class LIBEMBER_API GlowStreamEntry : public GlowContainer
    {
        public:
            /**
             * Constructor, initalizes a stream entry without an application tag.
             */
            GlowStreamEntry();

            /**
             * Constructor, initializes a stream entry with the tag specified.
             * @param tag Application tag.
             */
            explicit GlowStreamEntry(ber::Tag const& tag);

            /**
             * Constructor, initializes a stream entry with a stream identifier and its value..
             * @param streamIdentifier The stream identifier to set.
             * @param value The stream's current value.
             */
            GlowStreamEntry(int streamIdentifier, int value);

            /**
             * Sets the stream identifier.
             * @param streamIdentifier The identifier to set.
             */
            void setStreamIdentifier(int streamIdentifier);

            /**
             * Sets the stream value.
             * @param value Current stream value
             */
            void setValue(int value);

            /**
             * Returns the value of the stream identifier
             * @return The stream identifier or -1 if the container does not contain this value.
             */
            int streamIdentifier() const;

            /**
             * Returns the stream value
             * @return The stream value or -1 if the container does not contain this value.
             */
            int value() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowStreamEntry.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP
