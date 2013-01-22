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

#ifndef __LIBEMBER_BER_LENGTH_HPP
#define __LIBEMBER_BER_LENGTH_HPP

namespace libember { namespace ber
{
    /**
     * A simple template type that wraps an (unsigned) integral type, so that
     * type dispatching may be used to distinguish integral values that should
     * be encoded as LENGTHs instead of INTEGERs.
     * @note Specialization of this type is only sensible for unsigned integral
     *      types only.
     */
    template<typename IntegralType>
    struct Length
    {
        typedef IntegralType value_type;

        /** 
         * Indicates indefinite length
         */
        static value_type const INDEFINITE = static_cast<value_type>(-1);

        /**
         * Constructor that wraps the passed value.
         * @param value the value to wrap.
         * @note Please note that this constructor is intentionally non-explicit
         *      to allow for convenient implicit conversion, where it might be
         *      suitable.
         */
        Length(value_type value);

        /**
         * Checks if this length is indefinite 
         * @return True if the length value is indefinite, otherwise false
         */
        bool isIndefinite() const;

        value_type value;
    };

    /**
     * Generator function that makes wrapping values in multi-byte
     * wrappers more convenient.
     * @param value the value to wrap.
     * @return A multi-byte wrapper containing @p value.
     */
    template<typename IntegralType>
    Length<IntegralType> make_length(IntegralType value);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename IntegralType>
    inline Length<IntegralType>::Length(value_type value)
        : value(value)
    {}

    template<typename IntegralType>
    inline bool Length<IntegralType>::isIndefinite() const
    {
        return value == INDEFINITE;
    }

    template<typename IntegralType>
    inline Length<IntegralType> make_length(IntegralType value)
    {
        return Length<IntegralType>(value);
    }

    template<typename IntegralType>
    typename Length<IntegralType>::value_type const Length<IntegralType>::INDEFINITE;

}
}

#endif  // __LIBEMBER_BER_LENGTH_HPP

