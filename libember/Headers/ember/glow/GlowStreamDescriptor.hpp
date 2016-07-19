/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_HPP
#define __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_HPP

#include "GlowContainer.hpp"
#include "StreamFormat.hpp"

namespace libember { namespace glow
{
    /** Forward declaration */
    class GlowParameterBase;

    /**
     * A GlowStreamDescriptor contains additional information about the format of a stream. 
     */
    class LIBEMBER_API GlowStreamDescriptor : public GlowContainer
    {
        friend class GlowParameterBase;
        public:
            /**
             * Initializes a new GlowStreamDescriptor.
             * @param tag The application tag to set
             */
            explicit GlowStreamDescriptor(ber::Tag const& tag);

            /**
             * Returns the stream format.
             * @return The stream format.
             */
            StreamFormat format() const;

            /**
             * Returns the offset within the stream entry containing the parameter's stream value.
             * @return The offset within the stream entry.
             */
            int offset() const;

        private:
            /**
             * Initializes a new GlowStreamDescriptor and inserts all properties into the sequence.
             * @param tag The application tag to use.
             * @param format The format type.
             * @param offset The offset of the stream value within its entry.
             */
            GlowStreamDescriptor(ber::Tag const& tag, StreamFormat format, int offset);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStreamDescriptor.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_HPP
