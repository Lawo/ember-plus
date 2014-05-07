#ifndef __TINYEMBER_GADGET_DIRTYSTATELISTENER_H
#define __TINYEMBER_GADGET_DIRTYSTATELISTENER_H

#include "DirtyState.h"

namespace gadget
{
    /**
     * Interface that can be used to receive updated dirty states from any object.
     */
    template<typename FlagType, typename ObjectType>
    class DirtyStateListener
    {
        public:
            typedef typename DirtyState<FlagType>::flag_type flag_type;
            typedef typename ObjectType object_type;

            /** Destructor */
            virtual ~DirtyStateListener();

            /** 
             * This method is invoked when the dirty state of an object changes
             * @param state The new dirty state.
             * @param object The object that has changed its state.
             */
            virtual void notifyStateChanged(DirtyState<FlagType> const &state, object_type object) = 0;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename FlagType, typename ObjectType>
    inline DirtyStateListener<FlagType, ObjectType>::~DirtyStateListener()
    {}
}

#endif//__TINYEMBER_GADGET_DIRTYSTATELISTENER_H
