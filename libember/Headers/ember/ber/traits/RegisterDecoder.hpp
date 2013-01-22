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

