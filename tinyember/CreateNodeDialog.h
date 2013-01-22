#ifndef CREATENODEDIALOG_H
#define CREATENODEDIALOG_H

#include <QDialog>

/** Forward declarations **/
namespace Ui {
    class CreateNodeDialog;
}

namespace gadgets {
    class Node;
}

/**
 * A dialog class that provides a user interface which allows to
 * configure and create a new node.
 */
class CreateNodeDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent window
     */
    explicit CreateNodeDialog(QWidget *parent);

    /** Destructor **/
    virtual ~CreateNodeDialog();

    /**
     * Creates a new child node with the current identifier and description string.
     * @param parent The owner of the node to create.
     * @return Returns the created child node.
     */
    gadgets::Node* createNode(gadgets::Node& parent) const;

    /**
     * Creates a new root node with the current identifier and description string.
     * @return Returns the created root node
     */
    gadgets::Node* createRoot() const;

private slots:
    /**
     * Slot handling changes in the identifier text box.
     * @param text The current text of the identifier text box.
     */
    void identifierChanged(QString text);

private:
    Ui::CreateNodeDialog *ui;
};

#endif // CREATENODEDIALOG_H
