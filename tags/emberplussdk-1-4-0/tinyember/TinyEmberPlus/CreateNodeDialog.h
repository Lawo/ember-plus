#ifndef __TINYEMBER_CREATENODEDIALOG_H
#define __TINYEMBER_CREATENODEDIALOG_H

#include <qdialog.h>
#include "ui_CreateNodeDialog.h"

/** Forward declaration */
namespace gadget
{
    class Node;
}

/**
 * A simple modal dialog which is used to create a new gadget node.
 */
class CreateNodeDialog : public QDialog
{
    Q_OBJECT
    public:
        /**
         * Initializes a new dialog.
         * @param parent The parent of this dialog.
         * @param owner The parent node of the node to create.
         */
        explicit CreateNodeDialog(QWidget* parent, gadget::Node const* owner);

        /** Destructor */
        virtual ~CreateNodeDialog(void);

        /**
         * Returns the identifier of the new node.
         * @return The identifier of the new node.
         */
        std::string identifier() const;

        /**
         * Returns the description of the new node.
         * @return The description of the new node.
         */
        std::string description() const;

    private slots:
        /**
         * This slot is invoked when the text of the identifier box changed.
         * @param text The current text of the identifier box.
         */
        void identifierChanged(QString text);

    private:
        Ui::CreateNodeDialog* m_dialog;
        gadget::Node const* m_owner;
};

#endif//__TINYEMBER_CREATENODEDIALOG_H
