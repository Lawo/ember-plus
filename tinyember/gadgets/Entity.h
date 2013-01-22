#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include "Property.h"
#include "Types.h"

namespace gadgets
{
    /** Forward declarations **/
    class Node;
    class Parameter;
    class EntityStateListener;
    class EntityStateVisitor;
    class EntityTypeVisitor;


    /**
     * Entity base class. An entity represents a node or a parameter.
     */
    class Entity
    {
        typedef std::list<Entity*> EntityList;
        typedef std::list<EntityStateListener*> EntityStateListenerList;

    public:
        typedef EntityList::const_iterator const_iterator;
        typedef EntityList::iterator iterator;
        typedef EntityList::size_type size_type;
        typedef std::size_t number_type;

    public:
        /**
         * Destructor
         */
        virtual ~Entity();

        /**
         * Creates a copy of this entity, including all children.
         * @param parent The parent of the copy
         * @param identifier The identifier of the copy
         */
        virtual Entity* clone(Node* parent, String const& identifier) const = 0;

        /**
         * Used to collect dirty properties of this entity. All properties marked dirty
         * or provided in the properties argument shall be reported to the visitor.
         * The dirty flags will not be reset until clearDirtyState is being called.
         * @param visitor State visitor.
         * @param properties The properties the visitor is interested in.
         */
        void visit(EntityStateVisitor &visitor, PropertyState const& properties) const;

        /**
         * Used to receive detailed type information about this entity
         * @param typeVisitor The visitor the will receive the resolved type information.
         */
        void visit(EntityTypeVisitor& typeVisitor);

        /**
         * Sets the entity's description
         * @param description Description
         */
        void setDescription(String const& description);

        /**
         * Returns true if the entity has no children
         * @return True if the entity has no children, otherwise false
         */
        bool empty() const;

        /**
         * Returns the number of child entities
         */
        size_type size() const;

        /**
         * Returns the start iterator which points to the first child
         */
        const_iterator begin() const;

        /**
         * Indicates the end of the child list
         */
        const_iterator end() const;

        /**
         * Returns the start iterator
         */
        iterator begin();

        /**
         * Indicates the end of the child list
         */
        iterator end();

        /**
         * Returns the current dirty flag
         * @return True if at least one property is marked dirty
         */
        bool isDirty() const;

        /**
         * Marks a property as dirty. When an entity is marked dirty, all parents are being
         * notifed as well. That way, the tree can be traversed from top in order to collect
         * all dirty entities.
         * @param property Property to mark dirty
         */
        void markDirty(Property const& property = Property::DirtyChildEntity) const;

        /**
         * Resets the dirty state of this entity.
         */
        void clearDirtyState() const;

        /**
         * Adds a new listener to the entity. If the listener is already inside the
         * current list, it will not be added a second time.
         * The state listeners are notified each time a property of the entity changes.
         * @param listener Listener to add.
         */
        void addListener(EntityStateListener* listener);

        /**
         * Removes a listener from the list
         * @param listener Listener to remove
         */
        void removeListener(EntityStateListener* listener);

        /**
         * Returns the entity identifier
         */
        String const& identifier() const;

        /**
         * Returns the entity description
         */
        String const& description() const;

        /**
         * Returns the number
         */
        number_type number() const;

    protected:
        /**
         * Constructor
         * @param parent The entities' parent
         * @param identifier Identifier string
         * @param description Description string
         */
        explicit
        Entity(Entity* parent, String const& identifier, String const& description);

        /**
         * Called by visit. the implementation is responsible of providing all requested properties
         * @param visitor Visitor
         * @param properties Requested properties
         */
        virtual void visitImpl(EntityStateVisitor &visitor, PropertyState const& properties) const = 0;

        /**
         * Called by visit. The implementation is responsible to provided the concrete entity type.
         * @param visitor Visitor
         */
        virtual void visitImpl(EntityTypeVisitor &visitor) = 0;

    private:
        number_type const m_number;
        String const m_identifier;
        String m_description;
        Entity *const m_parent;
        EntityList m_children;
        EntityStateListenerList m_stateListeners;
        mutable PropertyState m_state;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline Entity::number_type Entity::number() const
    {
        return m_number;
    }

    inline String const& Entity::identifier() const
    {
        return m_identifier;
    }

    inline bool Entity::isDirty() const
    {
        return m_state.isDirty();
    }

    inline bool Entity::empty() const
    {
        return m_children.empty();
    }

    inline Entity::size_type Entity::size() const
    {
        return m_children.size();
    }

    inline Entity::const_iterator Entity::begin() const
    {
        return m_children.begin();
    }

    inline Entity::const_iterator Entity::end() const
    {
        return m_children.end();
    }

    inline Entity::iterator Entity::begin()
    {
        return m_children.begin();
    }

    inline Entity::iterator Entity::end()
    {
        return m_children.end();
    }
}


#endif // ENTITY_H
