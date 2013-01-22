#ifndef __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
#define __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow parameter can have
     */
    struct ParameterProperty
    {
        /**
         * Enumeration containing the properties a parameter may have.
         */
        enum _Domain
        {
            Identifier = 0, 
            Description = 1,
            Value = 2,
            Minimum = 3,
            Maximum = 4,
            IsWriteable = 5,
            Format = 6,
            Enumeration = 7,
            Factor = 8,
            IsOnline = 9,
            Formula = 10,
            Step = 11,
            Default = 12,
            IsCommand = 13,
            StreamIdentifier = 14,
            Children = 15,            
            EnumMap = 16,
        };

        typedef unsigned int value_type;

        /**
         * Constructor
         * @param value The property type
         */
        ParameterProperty(_Domain value)
            : value(value)
        {}

        /**
         * Contains the property value passed to the constructor. 
         */
        value_type value;
    };
}
}

#endif  // __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
