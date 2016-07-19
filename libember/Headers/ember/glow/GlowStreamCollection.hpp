/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
             * @deprecated Use the default constructor instead.
             * @return A new instance of GlowStreamCollection.
             */
            static GlowStreamCollection* create();

            /**
             * Constructor, initializes a new stream collection with the application tag set to GlowTags::Root() (Application - 30).
             */
            GlowStreamCollection();

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
             * @param streamIdentifier The stream identifier.
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
