/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
