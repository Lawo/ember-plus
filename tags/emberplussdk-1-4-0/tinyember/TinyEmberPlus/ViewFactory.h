#ifndef __TINYEMBER_VIEWFACTORY_H
#define __TINYEMBER_VIEWFACTORY_H

#include "gadget/ParameterTypeVisitor.h"

/** Forward declarations */
class QFrame;
class QWidget;

namespace gadget
{
    class Node;
    class Parameter;
}

/**
 * The ViewFactory creates a ui-view for each entity type (node, int-param, real-param,
 * bool-param, string-param, enum-param).
 */
class ViewFactory
{
    public:
        /**
         * Creates a new View for the passed node.
         * @param parent The parent of the view to create.
         * @param node The node to create a view for.
         * @return The created view.
         */
        static QFrame* createView(QWidget* parent, gadget::Node* node);

        /**
         * Creates a new View for the specified parameter.
         * @param parent The parent of the view to create.
         * @param parameter The parameter to create a view for.
         * @return The created view.
         */
        static QFrame* createView(QWidget* parent, gadget::Parameter* parameter);

    private:
        /**
         * Helper class which resolves the parameter type and creates the appropriate view.
         */
        class ViewFactoryImpl : gadget::ParameterTypeVisitor
        {
            public:
                /**
                 * Initializes the view factory.
                 * @param parent The parent of the view to create.
                 * @param parameter The parameter to create a view for.
                 */
                ViewFactoryImpl(QWidget* parent, gadget::Parameter* parameter);

                /**
                 * Returns the created view.
                 * @return The created view.
                 */
                QFrame* view() const;

            private:
                /** 
                 * Creates a view for a boolean parameter.
                 * @param parameter The parameter to create a view for.
                 */
                virtual void visit(gadget::BooleanParameter* parameter);

                /** 
                 * Creates a view for an enumeration parameter.
                 * @param parameter The parameter to create a view for.
                 */
                virtual void visit(gadget::EnumParameter* parameter);

                /** 
                 * Creates a view for a string parameter.
                 * @param parameter The parameter to create a view for.
                 */
                virtual void visit(gadget::StringParameter* parameter);

                /** 
                 * Creates a view for an integer parameter.
                 * @param parameter The parameter to create a view for.
                 */
                virtual void visit(gadget::IntegerParameter* parameter);

                /** 
                 * Creates a view for a real parameter.
                 * @param parameter The parameter to create a view for.
                 */
                virtual void visit(gadget::RealParameter* parameter);

            private:
                QWidget* m_parent;
                QFrame* m_view;
        };
};

#endif//__TINYEMBER_VIEWFACTORY_H

