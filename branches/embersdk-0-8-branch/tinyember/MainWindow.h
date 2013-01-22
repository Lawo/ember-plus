#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/** Forward declarations **/
class QModelIndex;
class QTreeWidgetItem;

namespace Ui {
    class MainWindow;
}

namespace gadgets {
    class EntityStateListener;
    class Entity;
    class Node;
}

namespace glow {
    class GlowStreamWriter;
}

namespace libember { namespace dom {
    class Node;
}
}


/**
 * Application's main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     */
    explicit
    MainWindow(gadgets::EntityStateListener* rootListener, glow::GlowStreamWriter* streamWriter, QWidget *parent = 0);

    /**
     * Destructor
     */
    ~MainWindow();

private:
    /**
     * Clears the treeview and populates it with the passed entity and its children.
     * @param root Root node.
     */
    void fillGadgetTreeView(gadgets::Node* root);

    /**
     * Recursively appends the child-entities to the treeview
     * @param parent Parent item.
     * @param entity Current entity
     */
    void fillGadgetTreeViewRecursive(QTreeWidgetItem* parent, gadgets::Entity* entity);

    /**
     * Returns the root entity of the treeview.
     * @return The current root entity or null if the treeview is empty.
     */
    gadgets::Entity* rootEntity();

public slots:
    /**
     * Marshalls a glow message to the ui thread.
     * @param nodeptr A pointer to a dom::Node which should be a GlowElementCollection.
     */
    void synchronizedGlowRequest(void* nodeptr);

private slots:
    /**
     * Opens a file dialog to select an existing configuration
     */
    void openFile();

    /**
     * Exits the application
     */
    void exit();

    /**
     * Saves the current configuration
     */
    void saveFile();

    /**
     * Saves the configuration with a new name
     */
    void saveFileAs();

    /**
     * Handles item selection in the gadget view
     */
    void gadgetViewSelectionChanged(QTreeWidgetItem* x, QTreeWidgetItem* y);

    /**
     * Handles the request for a context menu.
     * @param pos Cursor position.
     */
    void showContextMenu(QPoint pos);

private:
    /**
     * Loads a configuration from file and displays it in the treeview.
     * @param filename The name of the file to load.
     */
    void openConfiguration(QString const& filename);

private:
    Ui::MainWindow *ui;
    gadgets::EntityStateListener *const m_rootListener;
    glow::GlowStreamWriter *const m_streamWriter;
};

#endif // MAINWINDOW_H
