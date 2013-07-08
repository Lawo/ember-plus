#ifndef __TINYEMBER_NODEVIEW_H
#define __TINYEMBER_NODEVIEW_H

#include <qframe.h>
#include "ui_NodeView.h"

/** Forward declaration */
namespace gadget
{
    class Node;
}

/**
 * A view which displays the node properties.
 */
class NodeView : public QFrame
{
    Q_OBJECT
    public:
        /**
         * Constructor.
         * @param parent The parent widget of this ivew.
         * @param node The node to display.
         */
        NodeView(QWidget* parent, gadget::Node* node);

        /** Destructor */
        virtual ~NodeView(void);

    private slots:
        /** Reloads all properties. */
        void updateUi();

        /** Updates the node's description string. */
        void updateDescription();

        /** Updates the online state of this node. */
        void updateOnlineState(bool state);

        /** Updates the report state of this node. */
        void updateUnmountState(bool state);

    private:
        Ui::NodeView* m_view;
        gadget::Node* m_node;
};

#endif//__TINYEMBER_NODEVIEW_H
