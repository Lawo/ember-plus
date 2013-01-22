#ifndef __TINYEMBER_CREATEPARAMETERDIALOG_H
#define __TINYEMBER_CREATEPARAMETERDIALOG_H

#include <QDialog>
#include "gadget\ParameterType.h"
#include "ui_CreateParameterDialog.h"

class CreateParameterDialog : public QDialog
{
    Q_OBJECT
    public:
        explicit CreateParameterDialog(QWidget* parent);

        virtual ~CreateParameterDialog();

        gadget::ParameterType const type() const;

        std::string identifier() const;

        std::string description() const;

    private slots:
        void identifierChanged(QString text);

    private:
        Ui::CreateParameterDialog* m_dialog;
};

#endif//__TINYEMBER_CREATEPARAMETERDIALOG_H
