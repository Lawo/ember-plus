#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include "functors/functors.h"
#include "Entity.h"
#include "EntityStateListener.h"
#include "EntityStateVisitor.h"
#include "EntityTypeVisitor.h"

namespace gadgets
{
    using namespace functors;

    Entity::Entity(Entity* parent, String const& identifier, String const& description)
        : m_number(parent ? 1 + parent->size() : 1)
        , m_identifier(identifier)
        , m_description(description)
        , m_parent(parent)
    {
        if (m_parent)
            m_parent->m_children.push_back(this);
    }

    Entity::~Entity()
    {
        if (m_parent)
        {
            Entity::iterator const first = m_parent->begin();
            Entity::iterator const last = m_parent->end();

            m_parent->m_children.erase(std::find(first, last, this));
        }

        // Create a copy of the child elements and delete all entities.
        EntityList children;

        std::copy(m_children.begin(), m_children.end(), std::back_inserter(children));
        std::for_each(children.begin(), children.end(), delete_object<Entity>());
    }

    void Entity::setDescription(String const& description)
    {
        if (m_description != description)
        {
            m_description = description;

            markDirty(Property::Description);
        }
    }

    String const& Entity::description() const
    {
        return m_description;
    }

    void Entity::addListener(EntityStateListener* listener)
    {
        EntityStateListenerList::const_iterator const first = m_stateListeners.begin();
        EntityStateListenerList::const_iterator const last = m_stateListeners.end();
        EntityStateListenerList::const_iterator const it = std::find(first, last, listener);

        if (it == last && listener != 0)
        {
            m_stateListeners.push_back(listener);
        }
    }

    void Entity::removeListener(EntityStateListener* listener)
    {
        m_stateListeners.remove(listener);
    }

    void Entity::clearDirtyState() const
    {
        m_state.clear();
    }

    void Entity::markDirty(Property const& property) const
    {
        m_state.set(property.value);

        // Only notify the listeners if a property of this entity
        // changed or if this is the root.
        if(property.value != Property::DirtyChildEntity || m_parent == 0)
        {
            EntityStateListenerList::const_iterator first = m_stateListeners.begin();
            EntityStateListenerList::const_iterator const last = m_stateListeners.end();
            PropertyState const state = m_state;

            for( /** Nothing **/ ; first != last; ++first)
            {
                (*first)->dirtyStateChanged(*this, state);
            }
        }

        if (m_parent != 0)
            m_parent->markDirty();
    }

    void Entity::visit(EntityStateVisitor& visitor, PropertyState const& properties) const
    {
        PropertyState const state = m_state.combine(properties);

        visitImpl(visitor, state);

        if (state.isSet(Property::Identifier))
            visitor.addProperty(Property::Identifier, m_identifier);

        if (state.isSet(Property::Description))
            visitor.addProperty(Property::Description, m_description);
    }

    void Entity::visit(EntityTypeVisitor& typeVisitor)
    {
        visitImpl(typeVisitor);
    }
}
