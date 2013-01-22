#ifndef __LIBEMBER_GLOW_NODEPROPERTY_HPP
#define __LIBEMBER_GLOW_NODEPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow node can have
     */
    struct NodeProperty
    {
        /**
         * Enumeration containing hte properties a node may have.
         */
        enum _Domain
        {
            Identifier = 0, 
            Description = 1,
            Children = 2,
            IsRoot = 3,
        };

        typedef unsigned int value_type;

        /**
         * Constructor
         * @param value The property type
         */
        NodeProperty(_Domain value)
            : value(value)
        {}

        /**
         * Contains the property value passed to the constructor. 
         */
        value_type value;
    };
}
}

#endif  // __LIBEMBER_GLOW_NODEPROPERTY_HPP
