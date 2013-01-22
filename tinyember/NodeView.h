#ifndef NODEVIEW_H
#define NODEVIEW_H

#include <QFrame>

/** Forward declarations **/
namespace gadgets {
    class Node;
}

namespace Ui {
    class NodeView;
}


/**
 * Property View for a single node
 */
class NodeView : public QFrame
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param node Node
     */
    explicit
    NodeView(QWidget *parent, gadgets::Node* node);

    /**
     * Destructro
     */
    ~NodeView();

private slots:
    /**
     * Updates the ui elements with the node's properties
     */
    void updateUi();

    /**
     * Updates the entity's description
     */
    void updateEntityDescription();

private:
    gadgets::Node* m_node;
    Ui::NodeView *ui;
};

#endif // NODEVIEW_H
