#ifndef __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

#include <string>
#include "CodecTraits.hpp"
#include "../ObjectIdentifier.hpp"
#include "../detail/MultiByte.hpp"
#include "../../meta/FunctionTraits.hpp"

namespace libember { namespace ber
{
    /**
     * UniversalTagTraits specialization for object identifiers.
     */
    template<>
    struct UniversalTagTraits<ObjectIdentifier>
    {
        typedef std::string value_type;

        static Tag universalTag()
        {
            return make_tag(Class::Universal, Type::ObjectIdentifier);
        }
    };


    /** EncodingTraits specialization for object identifiers. */
    template<>
    struct EncodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;

        static std::size_t encodedLength(value_type value)
        {
            value_type::value_type const id = value.value();
            unsigned long const upper = static_cast<unsigned long>((id >> 32) & 0xffffffffU);
            unsigned long const lower = static_cast<unsigned long>((id)       & 0xffffffffU);
            return detail::getMultiByteEncodedLength(upper) + detail::getMultiByteEncodedLength(lower);
        }

        static void encode(util::OctetStream& output, value_type value)
        {
            value_type::value_type const id = value.value();
            unsigned long const upper = static_cast<unsigned long>((id >> 32) & 0xffffffffU);
            unsigned long const lower = static_cast<unsigned long>((id)       & 0xffffffffU);
            detail::encodeMultibyte(output, upper);
            detail::encodeMultibyte(output, lower);
        }
    };

    /** DecodingTraits specialization for object identifiers. */
    template<>
    struct DecodingTraits<ObjectIdentifier>
    {
        typedef ObjectIdentifier value_type;
        /**
         * Traits type providing various infos on the decode functions signature.
         * Unfortunately C++03 does not yet support a library independent
         * typeof/decltype operation, which is why we have to
         * 1. explicitly repeat the signature here and
         * 2. cannot defer the introspection work to the freestanding decode
         *    function.
         */
        typedef meta::FunctionTraits<value_type (*)(util::OctetStream&, std::size_t)> signature;

        static value_type decode(util::OctetStream& input, std::size_t /* size */)
        {
            return (static_cast<value_type::value_type>(detail::decodeMultibyte(input)) << 32) | static_cast<value_type::value_type>(detail::decodeMultibyte(input));
        }
    };
}
}

#endif // __LIBEMBER_BER_TRAITS_OBJECTIDENTIFIER_HPP

