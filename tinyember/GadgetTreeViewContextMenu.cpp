#include <QMenu>
#include <QTreeWidget>
#include "CreateNodeDialog.h"
#include "CreateParameterDialog.h"
#include "GadgetTreeViewContextMenu.h"
#include "TreeWidgetItemData.h"
#include "gadgets/Node.h"
#include "gadgets/Parameter.h"

using namespace ::gadgets;
using namespace ::gadgets::states;

GadgetTreeViewContextMenu::GadgetTreeViewContextMenu(QTreeWidget* view, QPoint const& localCursor)
    : m_view(view)
    , m_item(view->currentItem())
    , m_position(view->mapToGlobal(localCursor))
    , m_removeItem(false)
{
}


void GadgetTreeViewContextMenu::exec()
{
    if (m_item != 0)
    {
        TreeWidgetItemData const data = m_item->data(1, 0).value<TreeWidgetItemData>();
        gadgets::Entity* const entity = data.entity();

        if (entity != 0)
            entity->visit(*this);

        if (m_removeItem)
        {
            delete m_item;
            delete entity;
        }
    }
    else
    {
        QMenu menu;
        QAction *newNode = menu.addAction(QIcon(":/image/resources/node-insert-child.png"), "Create Node...");
        QAction *selection = menu.exec(m_position);

        if (selection == newNode)
        {
            CreateNodeDialog dialog(m_view);

            if(dialog.exec() == QDialog::Accepted)
            {
                Node *const root = dialog.createRoot();
                QTreeWidgetItem *const item = new QTreeWidgetItem(m_view);
                item->setIcon(0, QIcon(":/image/resources/globe-medium.png"));
                item->setText(0, QString::fromStdString(root->identifier()));
                item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(root)));
            }
        }
    }
}

//virtual
void GadgetTreeViewContextMenu::visit(Node& node)
{
    showNodeMenu(node);
}

//virtual
void GadgetTreeViewContextMenu::visit(Parameter& parameter, NumericState<int>&)
{
    showParameterMenu(parameter);
}

//virtual
void GadgetTreeViewContextMenu::visit(Parameter& parameter, NumericState<double>&)
{
    showParameterMenu(parameter);
}

//virtual
void GadgetTreeViewContextMenu::visit(Parameter& parameter, EnumState&)
{
    showParameterMenu(parameter);
}

//virtual
void GadgetTreeViewContextMenu::visit(Parameter& parameter, StringState&)
{
    showParameterMenu(parameter);
}

void GadgetTreeViewContextMenu::showParameterMenu(Parameter& parameter)
{
    QMenu menu;
    QAction *const deleteEntity = menu.addAction(
                QIcon(":/image/resources/node-delete-next.png"),
                QString("Delete ") + QString::fromStdString(parameter.identifier()));

    m_removeItem = (menu.exec(m_position) == deleteEntity);
}

void GadgetTreeViewContextMenu::showNodeMenu(Node& node)
{
    QMenu menu;
    QAction *const newNode = menu.addAction(QIcon(":/image/resources/node-insert-child.png"), "Create Node...");
    QAction *const newParameter = menu.addAction(QIcon(":/image/resources/node-insert-next.png"), "Create Parameter...");
    QAction *const deleteEntity = menu.addAction(QIcon(":/image/resources/node-delete-child.png"), "Delete Node");

    QAction *const selection = menu.exec(m_position);

    if(selection == deleteEntity)
    {
        m_removeItem = true;
    }
    else if(selection == newNode)
    {
        CreateNodeDialog dialog(m_view);

        if(dialog.exec() == QDialog::Accepted)
        {
            gadgets::Node* const childNode = dialog.createNode(node);
            QTreeWidgetItem* const item = new QTreeWidgetItem(m_item);
            item->setIcon(0, QIcon(":/image/resources/globe-small.png"));
            item->setText(0, QString::fromStdString(childNode->identifier()));
            item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(childNode)));
        }
    }
    else if(selection == newParameter)
    {
        CreateParameterDialog dialog(m_view);

        if(dialog.exec() == QDialog::Accepted)
        {
            Parameter *const parameter = dialog.createParameter(node);
            QTreeWidgetItem* const item = new QTreeWidgetItem(m_item);
            item->setIcon(0, QIcon(":/image/resources/leaf.png"));
            item->setText(0, QString::fromStdString(parameter->identifier()));
            item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(parameter)));
        }
    }
}
