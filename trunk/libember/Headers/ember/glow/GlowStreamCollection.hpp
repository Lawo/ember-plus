/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP

#include "../ber/Octets.hpp"
#include "GlowElement.hpp"
#include "GlowStreamEntry.hpp"

namespace libember { namespace glow
{
    /** Forward declarations */
    class GlowNodeFactory;

    /**
     * The StreamCollection contains a list of StreamEntries.
     */
    class LIBEMBER_API GlowStreamCollection : public GlowElement
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Creates a new GlowStreamCollection, which is used as root node. The returned instance must
             * be deleted manually when it is no longer needed.
             * @return A new instance of GlowStreamCollection.
             */
            static GlowStreamCollection* create();

            /**
             * Inserts a new StreamEntry into the collection.
             * @param streamIdentifier The stream identifier.
             * @param value The stream value.
             */
            void insert(int streamIdentifier, int value);

            /**
             * Inserts a new StreamEntry into the collection.
             * @param streamIdentifier The stream identifier.
             * @param value The stream value.
             */
            void insert(int streamIdentifier, double value);

            /**
             * Inserts a new StreamEntry into the collection.
             * @param streamIdentifier The stream identifier.
             * @param value The stream value.
             */
            void insert(int streamIdentifier, std::string const& value);

            /**
             * Inserts the provided StreamEntry to the collection.
             * @param entry The entry to insert.
             */
            void insert(GlowStreamEntry* entry);

            /**
             * Insert a new StreamEntry, which contains an array of bytes. The format of the byte array
             * must be provided by the Parameter's StreamDescriptor.
             * @param first The first element to copy to the stream value buffer.
             * @param last The first item beyond the stream buffer.
             */
            template<typename InputIterator>
            void insert(int streamIdentifier, InputIterator first, InputIterator last);

        private:
            /**
             * Initializes an empty stream collection with the provided application tag.
             * @param tag Application tag.
             */
            explicit GlowStreamCollection(ber::Tag const& tag);
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void GlowStreamCollection::insert(int streamIdentifier, InputIterator first, InputIterator last)
    {
        GlowElement::insert(end(), new GlowStreamEntry(streamIdentifier, first, last));
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStreamCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_HPP
