#ifndef GLOWNODEVISITOR_H
#define GLOWNODEVISITOR_H

#include "GlowElementVisitor.h"

/** Forward declarations **/
namespace libember { namespace glow
{
    class GlowElementCollection;
    class GlowNode;
}
}

namespace gadgets
{
    class Node;
}

namespace glow
{
    /**
     * Visitor impleemtation for a node type.
     */
    class GlowNodeVisitor : public GlowElementVisitor
    {
    public:
        /**
         * Constructor
         * @param parent The element collection this parameter adds itself to
         * @param parameter The parameter being visited
         */
        explicit GlowNodeVisitor(libember::glow::GlowElementCollection* parent, gadgets::Node const& node);

        /**
         * Creates a new visitor which registers itself as child of this entity.
         */
        virtual GlowStateVisitor* createChildVisitor();

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
        libember::glow::GlowNode* m_node;
    };
}

#endif // GLOWNODEVISITOR_H
