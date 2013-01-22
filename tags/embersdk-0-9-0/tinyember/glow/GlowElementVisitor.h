#ifndef GLOWELEMENTVISITOR_H
#define GLOWELEMENTVISITOR_H

#include "../Types.h"
#include "../gadgets/EntityStateVisitor.h"
#include "../gadgets/Property.h"

namespace glow
{
    /** Forward declaration **/
    class GlowStateVisitor;

    /**
     * Base class used to collect the properties of a node or parameter.
     * Each entity type requires its own implementation, which means that there
     * are currently two implementations, one for the node and one for the parameter.
     */
    class GlowElementVisitor
    {
    public:
        typedef gadgets::EntityStateVisitor::EnumList EnumList;

        /** Destructor **/
        virtual ~GlowElementVisitor();

        /**
         * Creates a new visitor which registers itself as child of this entity.
         */
        virtual GlowStateVisitor* createChildVisitor() = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Integer value
         */
        virtual void addProperty(gadgets::Property property, int value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Double value
         */
        virtual void addProperty(gadgets::Property property, double value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param value String value
         */
        virtual void addProperty(gadgets::Property property, String const& value) = 0;

        /**
         * Adds a bool property
         * @param property Property type
         * @param value boolean value
         */
        virtual void addProperty(gadgets::Property property, bool value) = 0;

        /**
         * Adds an integer property
         * @param property Property type
         * @param enumeration String list
         */
        virtual void addProperty(gadgets::Property property, EnumList const& enumeration) = 0;
    };
}

#endif // GLOWELEMENTVISITOR_H
