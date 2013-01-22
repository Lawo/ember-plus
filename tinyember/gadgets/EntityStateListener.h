#ifndef ENTITYSTATELISTENER_H
#define ENTITYSTATELISTENER_H

#include "Property.h"

namespace gadgets
{
    /** Forward listener **/
    class Entity;

    /**
     * This listener is used to receive a notification when the state of an entity changes, like the description.
     */
    class EntityStateListener
    {
    public:
        /** Destructor **/
        virtual ~EntityStateListener()
        {}

        /**
         * Abstract callback.
         * @param entity The entity that has changed state.
         * @param state The flags that indicate the properties that have changed.
         */
        virtual void dirtyStateChanged(Entity const& entity, PropertyState const& state) = 0;
    };
}

#endif // ENTITYSTATELISTENER_H
