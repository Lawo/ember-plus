#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "DirtyState.h"

namespace gadgets
{
    /**
     * Contains a list of all properties available
     */
    struct Property
    {
        enum _Domain
        {
            None = 0U,
            Identifier = (1U << 0U),
            Description = (1U << 1U),
            IsOnline = (1U << 2U),
            IsWriteable = (1U << 3U),
            IsCommand = (1U << 4U),
            IsStream = (1U << 5U),

            ValueMin = (1U << 10U),
            ValueMax = (1U << 11U),
            ValueEnumeration = (1U << 12U),
            Value = (1U << 13U),
            ValueFactor = (1U << 14U),
            ValueFormat = (1U << 15U),
            ValueStep = (1U << 16U),

            DirtyChildEntity = (1U << 31U),

            All = (
                     Identifier | Description | IsOnline | IsWriteable | IsCommand | IsStream | ValueMin |
                     ValueMax | ValueEnumeration | Value | ValueFactor | ValueFormat | ValueStep
                  )
        };

        typedef std::size_t value_type;

        /**
         * Constructor
         * @param value Property
         */
        Property(value_type const value)
            : value(value)
        {
        }

        value_type const value;
    };


    /**
     * Flag collection for entity properties
     */
    typedef DirtyState<Property::_Domain> PropertyState;
}


#endif // PROPERTIES_H
