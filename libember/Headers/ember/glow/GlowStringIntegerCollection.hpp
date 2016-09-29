/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP

#include "GlowElement.hpp"

namespace libember { namespace glow
{
    /**
     * The StringIntegerCollection contains a list of StringIntegerPairs.
     */
    class LIBEMBER_API GlowStringIntegerCollection : public GlowElement
    {
        public:
            /**
             * Initializes an empty GlowStringIntegerCollection collection with the provided application tag.
             * @param tag Application tag.
             */
            explicit GlowStringIntegerCollection(ber::Tag const& tag);

            /**
             * Initializes a new GlowStringIntegerCollection with the passed tag and the provided enumeration.
             * The iterator's value type must be a string-integer pair.
             * @param tag The application tag of this collection.
             * @param first A reference to the first enumeration entry in a collection.
             * @param last Points to the first item beyond the enumeration entry collection.
             */
            template<typename InputIterator>
            GlowStringIntegerCollection(ber::Tag const& tag, InputIterator first, InputIterator last);

            /**
             * Inserts a new entry into the collection.
             * @param name The enumeration entry name
             * @param value The enumeration entry value. This is the value that has to be sent 
             *      to the remote device.
             */
            void insert(std::string const& name, int value);
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline GlowStringIntegerCollection::GlowStringIntegerCollection(ber::Tag const& tag, InputIterator first, InputIterator last)
        : GlowElement(GlowType::StringIntegerCollection, tag)
    {
        for ( ; first != last; ++first)
        {
            insert(first->first, first->second);
        }
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStringIntegerCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP
