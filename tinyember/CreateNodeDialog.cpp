#include "CreateNodeDialog.h"
#include "ui_CreateNodeDialog.h"
#include "gadgets/Node.h"
#include "Types.h"

using namespace ::gadgets;

CreateNodeDialog::CreateNodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateNodeDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/image/resources/node-insert-child.png"));
}

CreateNodeDialog::~CreateNodeDialog()
{
    delete ui;
}

Node* CreateNodeDialog::createNode(Node& parent) const
{
    String const identifier = ui->identifierText->text().toStdString();
    String const description = ui->descriptionText->text().toStdString();

    return parent.insert(identifier, description);
}

Node* CreateNodeDialog::createRoot() const
{
    String const identifier = ui->identifierText->text().toStdString();
    String const description = ui->descriptionText->text().toStdString();

    return Node::create(identifier, description);
}

void CreateNodeDialog::identifierChanged(QString text)
{
    ui->descriptionText->setText(text);
}

