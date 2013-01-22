#ifndef __TINYEMBER_PARAMETERFIELD_H
#define __TINYEMBER_PARAMETERFIELD_H

#include "DirtyState.h"

namespace gadget
{
    struct ParameterField
    {
        enum _Domain
        {
            None = 0U,
            Identifier = (1U << 0U),
            Description = (1U << 1U),
            IsOnline = (1U << 2U),
            IsStream = (1U << 3U),
            Access = (1U << 4U),
            Type = (1U << 5U),

            ValueMin = (1U << 10U),
            ValueMax = (1U << 11U),
            ValueEnumeration = (1U << 12U),
            Value = (1U << 13U),
            ValueFactor = (1U << 14U),
            ValueFormat = (1U << 15U),
            ValueStep = (1U << 16U),
            ValueFormula = (1U << 17),

            StreamIdentifier = (1U << 20U),
            StreamDescriptor = (1U << 21U),

            SubscriptionCount = (1U << 30U),
            All = (
                     Identifier | Description | IsOnline | Access | Type | IsStream | ValueMin |
                     ValueMax | ValueEnumeration | Value | ValueFactor | ValueFormat | ValueStep | ValueFormula | 
                     StreamIdentifier | StreamDescriptor
                  )
        };

        typedef unsigned int value_type;

        ParameterField(value_type value)
            : value(value)
        {
        }

        value_type const value;
    };

    typedef DirtyState<ParameterField::_Domain> ParameterFieldState;
}

#endif//__TINYEMBER_PARAMETERFIELD_H
