#include "CreateParameterDialog.h"
#include "ui_CreateParameterDialog.h"
#include "gadgets/Parameter.h"
#include "gadgets/Node.h"

using namespace ::gadgets;

CreateParameterDialog::CreateParameterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateParameterDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/image/resources/node-insert-next.png"));
}

CreateParameterDialog::~CreateParameterDialog()
{
    delete ui;
}

void CreateParameterDialog::identifierChanged(QString text)
{
    ui->descriptionText->setText(text);
}

Parameter* CreateParameterDialog::createParameter(Node& parent) const
{
    StateType const type = ui->parameterTypeBox->currentIndex() + 1;
    String const identifier = ui->identifierText->text().toStdString();
    String const description = ui->descriptionText->text().toStdString();

    return parent.insert(type, identifier, description);
}
