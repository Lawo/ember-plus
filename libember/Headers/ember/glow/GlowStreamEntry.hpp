/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP
#define __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP

#include "../ber/Octets.hpp"
#include "GlowContainer.hpp"
#include "Value.hpp"

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
             * Constructor, initializes a stream entry with the tag specified.
             * @param tag Application tag.
             */
            explicit GlowStreamEntry(ber::Tag const& tag);

            /**
             * Constructor, initializes a stream entry with a stream identifier and its value.
             * @param streamIdentifier The stream identifier to set.
             * @param value The stream's current value.
             */
            GlowStreamEntry(int streamIdentifier, int value);

            /**
             * Constructor, initializes a stream entry with a stream identifier and its value.
             * @param streamIdentifier The stream identifier to set.
             * @param value The stream's current value.
             */
            GlowStreamEntry(int streamIdentifier, double value);

            /**
             * Constructor, initializes a stream entry with a stream identifier and its string value.
             * @param streamIdentifier The stream identifier to set.
             * @param value The stream's current value.
             */
            GlowStreamEntry(int streamIdentifier, std::string const& value);

            /**
             * Constructor, initializes a stream entry with a stream identifier and its value..
             * @param streamIdentifier The stream identifier to set.
             * @param first Points to the first element in the buffer containing the stream data as bytes.
             * @param last Points to the first element beyond the source buffer.
             */
            template<typename InputIterator>
            GlowStreamEntry(int streamIdentifier, InputIterator first, InputIterator last);

            /**
             * Returns the value of the stream identifier
             * @return The stream identifier or -1 if the container does not contain this value.
             */
            int streamIdentifier() const;

            /**
             * Returns the stream value
             * @return The stream value.
             */
            Value value() const;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline GlowStreamEntry::GlowStreamEntry(int streamIdentifier, InputIterator first, InputIterator last)
        : GlowContainer(GlowType::StreamEntry)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamIdentifier(), streamIdentifier));
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamValue(), ber::Octets(first, last)));
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStreamEntry.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_HPP
