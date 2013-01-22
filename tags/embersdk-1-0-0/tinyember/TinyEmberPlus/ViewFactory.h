#ifndef __TINYEMBER_VIEWFACTORY_H
#define __TINYEMBER_VIEWFACTORY_H

#include "gadget/ParameterTypeVisitor.h"

class QFrame;
class QWidget;

namespace gadget
{
    class Node;
    class Parameter;
}

class ViewFactory
{
    public:
        static QFrame* createView(QWidget* parent, gadget::Node* node);

        static QFrame* createView(QWidget* parent, gadget::Parameter* parameter);

    private:
        class ViewFactoryImpl : gadget::ParameterTypeVisitor
        {
            public:
                ViewFactoryImpl(QWidget* parent, gadget::Parameter* parameter);

                QFrame* view() const;

            private:
                virtual void visit(gadget::EnumParameter* parameter);

                virtual void visit(gadget::StringParameter* parameter);

                virtual void visit(gadget::IntegerParameter* parameter);

                virtual void visit(gadget::RealParameter* parameter);

            private:
                QWidget* m_parent;
                QFrame* m_view;
        };
};

#endif//__TINYEMBER_VIEWFACTORY_H

