#ifndef __LIBEMBER_GLOW_GLOWPROPERTY_HPP
#define __LIBEMBER_GLOW_GLOWPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow node or parameter can have
     */
    struct GlowProperty
    {
        enum _Domain
        {
            Number = 0, 
            Identifier,
            Description,
            Value,
            Minimum,
            Maximum,
            IsWriteable,
            Format,
            Enumeration,
            Factor,
            IsOnline,
            Formula,
            Step,
            Default,
            IsCommand,
            StreamIdentifier,
            Children
        };

        typedef unsigned int value_type;

        /**
         * Constructor
         * @param value The property type
         */
        GlowProperty(_Domain value)
            : value(value)
        {}

        /**
         * Contains the property value passed to the constructor. 
         */
        value_type value;
    };
}
}

#endif  // __LIBEMBER_GLOW_GLOWPROPERTY_HPP
