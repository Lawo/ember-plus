#ifndef __TINYEMBER_NODEVIEW_H
#define __TINYEMBER_NODEVIEW_H

#include <qframe.h>
#include "ui_NodeView.h"

namespace gadget
{
    class Node;
}

class NodeView : public QFrame
{
    Q_OBJECT
    public:
        NodeView(QWidget* parent, gadget::Node* node);

        virtual ~NodeView(void);

    private slots:
        void updateUi();
        void updateDescription();

    private:
        Ui::NodeView* m_view;
        gadget::Node* m_node;
};

#endif//__TINYEMBER_NODEVIEW_H
