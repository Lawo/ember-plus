#ifndef __TINYEMBER_CREATEPARAMETERDIALOG_H
#define __TINYEMBER_CREATEPARAMETERDIALOG_H

#include <QDialog>
#include "gadget/ParameterType.h"
#include "ui_CreateParameterDialog.h"

/** Forward declaration */
namespace gadget
{
    class Node;
}

/**
 * A simple dialog which is used to create a new parameter.
 */
class CreateParameterDialog : public QDialog
{
    Q_OBJECT
    public:
        /**
         * Initializes a new CreateParameterDialog instance.
         * @param parent The parent of this dialog.
         * @param owner The parent node of the parameter to create.
         */
        explicit CreateParameterDialog(QWidget* parent, gadget::Node const* owner);

        /** Destructor */
        virtual ~CreateParameterDialog();

        /**
         * Returns the selected type of the parameter to create.
         * @return The selected type of the parameter to create.
         */
        gadget::ParameterType const type() const;

        /**
         * Returns the parameter identifier.
         * @return The parameter identifier.
         */
        std::string identifier() const;

        /**
         * Returns the parameter description string.
         * @return The parameter description string.
         */
        std::string description() const;

    private slots:
        /**
         * This slot is invoked when the text of the identifier box has changed.
         * @param text The new 
         */
        void identifierChanged(QString text);

    private:
        Ui::CreateParameterDialog* m_dialog;
        gadget::Node const* m_owner;
};

#endif//__TINYEMBER_CREATEPARAMETERDIALOG_H
