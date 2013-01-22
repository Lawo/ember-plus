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

