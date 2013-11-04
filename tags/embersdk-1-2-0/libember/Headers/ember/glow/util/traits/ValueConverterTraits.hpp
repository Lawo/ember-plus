#ifndef __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP
#define __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP

#include "../../../ber/Value.hpp"

namespace libember { namespace glow { namespace util
{   
    /**
        * Default implementation for a value converter.
        */
    template<typename DestType>
    struct ValueConverterTraits
    {
        typedef DestType value_type;
            
        static value_type valueOf(ber::Value const& value, value_type const&)
        {
            return value.as<DestType>();
        }
    };

    /**
     * This specialization checks whether the leaf contains an int or long value and
     * safely converts it.
     */
    template<>
    struct ValueConverterTraits<int>
    {
        typedef int value_type;

        static value_type valueOf(ber::Value const& value, value_type const& default_)
        {
            if (value.typeId() == typeid(int))
            {
                return value.as<int>();
            }
            else if (value.typeId() == typeid(long))
            {
                return static_cast<value_type>(value.as<long>());
            }
            return default_;
        }
    };

    /**
     * This specialization checks whether the leaf contains an int or long value and
     * safely converts it.
     */
    template<>
    struct ValueConverterTraits<long>
    {
        typedef long value_type;

        static value_type valueOf(ber::Value const& value, value_type const& default_)
        {
            if (value.typeId() == typeid(int))
            {
                return value.as<int>();
            }
            else if (value.typeId() == typeid(long))
            {
                return value.as<long>();
            }
            return default_;
        }
    };

    /**
     * This specialization checks whether the leaf contains an float or double value and
     * safely converts it.
     */
    template<>
    struct ValueConverterTraits<float>
    {
        typedef float value_type;

        static value_type valueOf(ber::Value const& value, value_type const& default_)
        {
            if (value.typeId() == typeid(float))
            {
                return value.as<float>();
            }
            else if (value.typeId() == typeid(double))
            {
                return static_cast<value_type>(value.as<double>());
            }
            return default_;
        }
    };

    /**
     * This specialization checks whether the leaf contains an float or double value and
     * safely converts it.
     */
    template<>
    struct ValueConverterTraits<double>
    {
        typedef double value_type;

        static value_type valueOf(ber::Value const& value, value_type const& default_)
        {
            if (value.typeId() == typeid(float))
            {
                return value.as<float>();
            }
            else if (value.typeId() == typeid(double))
            {
                return value.as<double>();
            }
            return default_;
        }
    };
}
}
}

#endif //  __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP