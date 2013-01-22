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
             * Initializes an empty stream collection with the provided application tag.
             * @param tag Application tag.
             */
            explicit GlowStringIntegerCollection(ber::Tag const& tag);

            /**
             * Inserts a new StreamEntry into the collection.
             * @param name The enumeration entry name
             * @param value The enumeration entry value. This is the value that has to be sent 
             *      to the remote device.
             */
            void insert(std::string const& name, int value);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowStringIntegerCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP
