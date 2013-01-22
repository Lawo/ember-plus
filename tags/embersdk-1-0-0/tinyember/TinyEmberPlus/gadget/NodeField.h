#ifndef __TINYEMBER_NODEFIELD_H
#define __TINYEMBER_NODEFIELD_H

#include "DirtyState.h"

namespace gadget
{
    struct NodeField
    {
        enum _Domain
        {
            None = 0U,
            Identifier = (1U << 0U),
            Description = (1U << 1U),
            IsRoot = (1U << 2U),

            DirtyChildEntity = (1U << 31U),

            All = (
                     Identifier | Description | IsRoot
                  )
        };

        typedef unsigned int value_type;

        NodeField(value_type value)
            : value(value)
        {
        }

        value_type const value;
    };

    typedef DirtyState<NodeField::_Domain> NodeFieldState;
}

#endif//__TINYEMBER_NODEFIELD_H
