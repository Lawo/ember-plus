/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TRAITS_CODECTRAITS_HPP
#define __LIBEMBER_BER_TRAITS_CODECTRAITS_HPP

#include <cstddef>
#include "../Type.hpp"
#include "../../util/OctetStream.hpp"
#include "../../meta/EnableIf.hpp"
#include "../../meta/IntToType.hpp"
#include "../../meta/IsSame.hpp"

//SimianIgnore

namespace libember { namespace ber
{
    /**
     * A traits class encapsulating universal BER Tag of a type.
     */
    template<typename ValueType>
    struct UniversalTagTraits;

    /**
     * Generic function that returns the universal tag of types that have an
     * EncodingTraits specialization.
     * @return The universal tag corresponding to the type of @p dummy.
     */
    template<typename ValueType>
    Tag universalTag();


    /**
     * A traits class encapsulating information relevant to encode
     * values of a type to BER form.
     */
    template<typename ValueType>
    struct EncodingTraits;

    /**
     * Generic function that returns the encoded binary length of @p value.
     * This generic overload requires an EncodingTraits specialization for
     * the type of @p value.
     * @param value the value whose encoded binary length should be calculated.
     * @return The number of bytes required to represent @p value in binary.
     */
    template<typename ValueType>
    std::size_t encodedLength(ValueType value);

    /**
     * Generic encode function for types that have an EncodingTraits
     * specialization.
     * @param output a reference to a stream buffer to which the value should
     *      be encoded.
     * @param value the value that should be encoded.
     */
    template<typename ValueType>
    void encode(util::OctetStream& output, ValueType value);

    /**
     * A traits class encapsulating information relevant to decode 
     * values of a type from BER form.
     */
    template<typename ValueType>
    struct DecodingTraits;

    /**
     * Generic decode function for types that have an EncodingTraits
     * specialization.
     * @param input a reference to a stream buffer from which the value
     *      should be decoded.
     * @param encodedLength The size of the encoded value, in bytes.
     * @return The value decoded from @p input.
     */
    template<typename ValueType>
    typename meta::EnableIf<
                meta::IsSame<
                    typename DecodingTraits<ValueType>::signature::arity,
                    meta::IntToType<2>
                >,
                ValueType
    >::type decode(util::OctetStream& input, std::size_t encodedLength);

    /**
     * Generic decode function for types that have an EncodingTraits
     * specialization.
     * @param input a reference to a stream buffer from which the value
     *      should be decoded.
     * @return The value decoded from @p input.
     */
    template<typename ValueType>
    typename meta::EnableIf<
                meta::IsSame<
                    typename DecodingTraits<ValueType>::signature::arity,
                    meta::IntToType<1>
                >,
                ValueType
    >::type decode(util::OctetStream& input);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline Tag universalTag()
    {
        return UniversalTagTraits<ValueType>::universalTag();
    }
    
    template<typename ValueType>
    inline std::size_t encodedLength(ValueType value)
    {
        return EncodingTraits<ValueType>::encodedLength(value);
    }

    template<typename ValueType>
    inline void encode(util::OctetStream& output, ValueType value)
    {
        EncodingTraits<ValueType>::encode(output, value);
    }
    
    template<typename ValueType>
    inline typename meta::EnableIf<
                meta::IsSame<
                    typename DecodingTraits<ValueType>::signature::arity,
                    meta::IntToType<2>
                >,
                ValueType
    >::type decode(util::OctetStream& input, std::size_t encodedLength)
    {
        return DecodingTraits<ValueType>::decode(input, encodedLength);
    }

    template<typename ValueType>
    inline typename meta::EnableIf<
                meta::IsSame<
                    typename DecodingTraits<ValueType>::signature::arity,
                    meta::IntToType<1>
                >,
                ValueType
    >::type decode(util::OctetStream& input)
    {
        return DecodingTraits<ValueType>::decode(input);
    }
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_BER_TRAITS_CODECTRAITS_HPP

