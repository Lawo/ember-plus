/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP
#define __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP


#include "../../../ber/Value.hpp"

//SimianIgnore

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
            else if (value.typeId() == typeid(long long))
            {
                return static_cast<value_type>(value.as<long long>());
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
            else if (value.typeId() == typeid(long long))
            {
                return static_cast<value_type>(value.as<long long>());
            }
            return default_;
        }
    };

    /**
     * This specialization checks whether the leaf contains an int or long value and
     * safely converts it.
     */
    template<>
    struct ValueConverterTraits<long long>
    {
        typedef long long value_type;

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
            else if (value.typeId() == typeid(long long))
            {
                return value.as<long long>();
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

//EndSimianIgnore

#endif //  __LIBEMBER_GLOW_UTIL_TRAITS_VALUECONVERTERTRAITS_HPP
