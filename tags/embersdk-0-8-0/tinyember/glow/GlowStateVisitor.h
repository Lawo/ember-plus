#ifndef GLOWSTATEVISITOR_H
#define GLOWSTATEVISITOR_H

#include <list>
#include "../gadgets/EntityStateVisitor.h"

/** Forward declarations **/
namespace libember { namespace glow
{
    class GlowElementCollection;
}
}

namespace glow
{
    /** Forward declaration **/
    class GlowElementVisitor;

    /**
     * Implementation of the EntityState visitor. This visitor internall constructs
     * a Glow representation of the dirty entities which can then be transmitted to connected
     * consumers.
     */
    class GlowStateVisitor : public gadgets::EntityStateVisitor
    {
    public:
        /**
         * Constructor
         * @param parent The parent of the entity being visited.
         */
        explicit GlowStateVisitor(libember::glow::GlowElementCollection* parent);

        /**
         * Destructor
         */
        virtual ~GlowStateVisitor();

        /**
         * Creates a new visitor which registers itself as child of this entity.
         */
        GlowStateVisitor* createChildVisitor();

        /**
         * Visit method. Called by the entity this visitor has been passed to.
         * @param node The node reporting its properties.
         */
        virtual void visit(gadgets::Node const& node);

        /**
         * Visit method. Called by the entity this visitor has been passed to.
         * @param parameter The parameter reporting its properties.
         */
        virtual void visit(gadgets::Parameter const& parameter);

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Integer value
         */
        virtual void addProperty(gadgets::Property property, int value);

        /**
         * Adds an integer property
         * @param property Property type
         * @param value Double value
         */
        virtual void addProperty(gadgets::Property property, double value);

        /**
         * Adds an integer property
         * @param property Property type
         * @param value String value
         */
        virtual void addProperty(gadgets::Property property, String const& value);

        /**
         * Adds a bool property
         * @param property Property type
         * @param value boolean value
         */
        virtual void addProperty(gadgets::Property property, bool value);

        /**
         * Adds an integer property
         * @param property Property type
         * @param enumeration String list
         */
        virtual void addProperty(gadgets::Property property, EnumList const& enumeration);

    private:
        typedef std::list<GlowStateVisitor*> VisitorList;

        GlowElementVisitor *m_proxy;
        libember::glow::GlowElementCollection* m_parent;
        VisitorList m_children;
    };
}

#endif // GLOWSTATEVISITOR_H
