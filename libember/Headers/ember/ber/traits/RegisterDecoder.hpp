/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_REGISTERDECODER_HPP
#define __LIBEMBER_BER_TRAITS_REGISTERDECODER_HPP

#include "../DecoderFactory.hpp"

namespace libember { namespace ber
{
    template<typename ValueType>
    class RegisterDecoder
    {
        public:
            /**
             * Constructor that creates and registers a decoder for the
             * specified type and its universal tag with the
             * DecoderFactory.
             */
            RegisterDecoder();

        private:
            /**
             * Private, unimplemented copy constructor to make instances
             * non-copyable.
             */
            RegisterDecoder(RegisterDecoder const&);

            /**
             * Private, unimplemented assignment operator to make
             * instances non-copyable.
             */
            RegisterDecoder& operator=(RegisterDecoder const&);
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline RegisterDecoder<ValueType>::RegisterDecoder()
    {
        static DecoderImpl<ValueType> theDecoder;
        decoderFactory().registerDecoder(universalTag<ValueType>(), &theDecoder);
    }

}
}

#endif  // __LIBEMBER_BER_TRAITS_REGISTERDECODER_HPP

