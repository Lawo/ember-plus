#ifndef __TINYEMBER_GADGET_DIRTYSTATELISTENER_H
#define __TINYEMBER_GADGET_DIRTYSTATELISTENER_H

#include "DirtyState.h"

namespace gadget
{
    template<typename FlagType, typename ObjectType>
    class DirtyStateListener
    {
        public:
            typedef typename DirtyState<FlagType>::flag_type flag_type;
            typedef typename ObjectType object_type;

            virtual ~DirtyStateListener();

            virtual void notifyStateChanged(DirtyState<FlagType> const &state, object_type object) = 0;
    };

    template<typename FlagType, typename ObjectType>
    inline DirtyStateListener<FlagType, ObjectType>::~DirtyStateListener()
    {}
}

#endif//__TINYEMBER_GADGET_DIRTYSTATELISTENER_H
