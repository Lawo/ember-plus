#include "nodeview.h"
#include "ui_nodeview.h"
#include "gadgets/Node.h"

NodeView::NodeView(QWidget *parent, gadgets::Node* node)
    : QFrame(parent)
    , m_node(node)
    , ui(new Ui::NodeView)
{
    ui->setupUi(this);

    QMetaObject::invokeMethod(this, "updateUi");
}

//slot
void NodeView::updateUi()
{
    QString const description = QString::fromStdString(m_node->description());
    QString const identifier = QString::fromStdString(m_node->identifier());
    QString const number = QVariant(m_node->number()).toString();

    if (ui->descriptionText->text() != description)
        ui->descriptionText->setText(description);

    if (ui->identifierText->text() != identifier)
        ui->identifierText->setText(identifier);

    if (ui->numberText->text() != number)
        ui->numberText->setText(number);
}

//slot
void NodeView::updateEntityDescription()
{
    m_node->setDescription(ui->descriptionText->text().toStdString());
}

//virtual
NodeView::~NodeView()
{

    delete ui;
}
