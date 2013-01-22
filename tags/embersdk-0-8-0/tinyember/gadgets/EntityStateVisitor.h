#ifndef ENTITYVISITOR_H
#define ENTITYVISITOR_H

#include <list>
#include "Entity.h"

namespace gadgets
{
    /** Forward declarations **/
    class Node;
    class Parameter;

    /**
     * Entity Visitor. This interface is used to collect "dirty" properties of an entity. The entity
     * shall call "addProperty" for each property that is provided or marked dirty as long as it
     * contains that property.
     */
    class EntityStateVisitor
    {
    public:
        typedef std::list<String> EnumList;

    public:
        /** Destructor */
        virtual ~EntityStateVisitor();

        /**
         * Visit method. Called by the entity this visitor has been passed to.
         * @param node The node reporting its properties.
         */
        virtual void visit(Node const& node) = 0;

        /**
         * Visit method. Called by the entity this visitor has been passed to.
         * @param parameter The parameter reporting its properties.
         */
        virtual void visit(Parameter const& parameter) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Integer value
         */
        virtual void addProperty(Property property, int value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Double value
         */
        virtual void addProperty(Property property, double value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value String value
         */
        virtual void addProperty(Property property, String const& value) = 0;

        /**
         * Adds a bool property
         * @param property Property type
         * @param value boolean value
         */
        virtual void addProperty(Property property, bool value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param enumeration String list
         */
        virtual void addProperty(Property property, EnumList const& enumeration) = 0;

        /**
         * Creates an enumeration, the iterator must contain a string value
         * @param property Property type
         * @param first First item to create an enumeration from
         * @param last Indicates the end of the enumeration source buffer
         */
        template<typename InputIterator>
        void addProperty(Property property, InputIterator first, InputIterator last);

    protected:
        /**
         * Constructor
         */
        EntityStateVisitor();
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void EntityStateVisitor::addProperty(Property property, InputIterator first, InputIterator last)
    {
        addProperty(property, EnumList(first, last));
    }

}


#endif // ENTITYVISITOR_H
