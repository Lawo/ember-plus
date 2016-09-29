/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_DECODERFACTORY_HPP
#define __LIBEMBER_BER_DECODERFACTORY_HPP

#include <map>
#include "../util/Api.hpp"
#include "traits/Decoder.hpp"

namespace libember { namespace ber
{
    /** Forward declarations. */
    class LIBEMBER_API DecoderFactory;
    template<typename ValueType> class RegisterDecoder;
    LIBEMBER_API DecoderFactory& decoderFactory();

    /**
     * A global factory class with which all decoder implementations are
     * automatically registered. And which may be used to dynamically map
     * universal tags to specific decoders for the corresponding C++ type.
     */
    class LIBEMBER_API DecoderFactory
    {
        template<typename ValueType> friend class RegisterDecoder;
        friend LIBEMBER_API DecoderFactory& decoderFactory();
        public:
            /**
             * Decode an explicitly tagged value from the stream referred to by
             * @p input and return it as a tagged, type-erased value.
             * @param input the stream buffer from which the explicitly tagged
             *      value should be decoded.
             * @return A tagged, type erased value wrapping the decoded value.
             * @throw std::runtime_error if the decoded tag does not match the universal
             *      tag of the specified type or if there is no decoder registered for the
             *      specified type.
             */
            Value decode(util::OctetStream& input) const;

        protected:
            /**
             * Default constructor. Creates a decoder factory without any
             * preregistered decoders.
             */
            DecoderFactory();

            /**
             * Register a decoder instance for the specified universal tag.
             * If the another decoder is already registered for the tag,
             * this request is silently ignored.
             * @param universalTag the universal tag for which a decoder
             *      shall be registered.
             * @param decoder a pointer to the decoder instance that allows
             *      decoding concrete, typed values corresponding to the
             *      universal type tag passed in @p universalTag.
             */
            void registerDecoder(Tag universalTag, Decoder* decoder);

        private:
            typedef std::map<Tag, Decoder*> DecoderMap;

        private:
            /**
             * @note Please note that the decoders contained in this map are not
             *      owned by this instance.
             */
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif
            DecoderMap m_decoderMap;
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
    };

    /**
     * Singleton accessor function. Returns a reference to the global decoder
     * factory.
     * @return A reference to the global decoder factory.
     */
    LIBEMBER_API
    DecoderFactory& decoderFactory();

    /**
     * Generic, dynamic decode function that decodes a tagged value from the
     * stream buffer referred to by @p input.
     * @return A type erased value containing the decoded value.
     * @todo Specify what exceptions will be thrown if decoding fails for
     *      various reasons
     */
    LIBEMBER_API
    Value decode(util::OctetStream& input);
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/DecoderFactory.ipp"
#endif

#endif  // __LIBEMBER_BER_DECODERFACTORY_HPP


