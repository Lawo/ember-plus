#ifndef __TINYEMBER_CREATENODEDIALOG_H
#define __TINYEMBER_CREATENODEDIALOG_H

#include <qdialog.h>
#include "ui_CreateNodeDialog.h"

class CreateNodeDialog : public QDialog
{
    Q_OBJECT
    public:
        explicit CreateNodeDialog(QWidget* parent);

        virtual ~CreateNodeDialog(void);

        std::string identifier() const;

        std::string description() const;

    private slots:
        void identifierChanged(QString text);

    private:
        Ui::CreateNodeDialog* m_dialog;
};

#endif//__TINYEMBER_CREATENODEDIALOG_H
