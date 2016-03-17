#include <algorithm>
#include <qmenu.h>
#include "CreateNodeDialog.h"
#include "CreateParameterDialog.h"
#include "GadgetViewContextMenu.h"
#include "TreeWidgetItemData.h"
#include "gadget/Node.h"
#include "gadget/NodeFactory.h"
#include "gadget/Parameter.h"
#include "gadget/ParameterFactory.h"
#include "gadget/BooleanParameter.h"
#include "gadget/EnumParameter.h"
#include "gadget/IntegerParameter.h"
#include "gadget/RealParameter.h"
#include "gadget/StringParameter.h"
#include "glow/ConsumerProxy.h"
#include "util/StringConverter.h"

using namespace gadget;

GadgetViewContextMenu::GadgetViewContextMenu(::glow::ConsumerProxy* proxy, QTreeWidget* view, QPoint const& cursor)
    : m_view(view)
    , m_proxy(proxy)
    , m_item(view->currentItem())
    , m_position(view->mapToGlobal(cursor))
    , m_removeItem(false)
{
}

void GadgetViewContextMenu::exec()
{
    if (m_item != nullptr)
    {
        auto const data = m_item->data(1, 0).value<TreeWidgetItemData>();
        auto const type = data.type();
        if (type.value() == TreeWidgetItemDataType::Node)
        {
            auto node = data.payload<gadget::Node>();

            if (node != nullptr)
            {
                exec(node);
                if (m_removeItem)
                {
                    delete m_item;
                    delete node;
                }
            }
        }
        else if (type.value() == TreeWidgetItemDataType::Parameter)
        {
            auto parameter = data.payload<gadget::Parameter>();

            if (parameter != nullptr)
            {
                exec(parameter);
                if (m_removeItem)
                {
                    delete m_item;
                    delete parameter;
                }
            }
        }
    }
    else
    {
        QMenu menu;
        auto const newnode = menu.addAction(QIcon(":/image/resources/node-insert-child.png"), "Create Node...");
        auto const selection = menu.exec(m_position);
        if (selection == newnode)
        {
            CreateNodeDialog dialog(m_view, nullptr);
            if (dialog.exec() == QDialog::Accepted)
            {
                auto const identifier = dialog.identifier();
                auto const description = dialog.description();
                auto root = NodeFactory::createRoot(identifier);
                auto item = new QTreeWidgetItem(m_view);

                root->setDescription(description);
                item->setIcon(0, QIcon(":/image/resources/globe-medium.png"));
                item->setText(0, ::util::StringConverter::toUtf8QString(root->identifier()));
                item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(root)));

                m_view->setCurrentItem(item);
            }
        }
    }
}

void GadgetViewContextMenu::exec(gadget::Node* parent)
{
    QMenu menu;
    auto const newnode = menu.addAction(QIcon(":/image/resources/node-insert-child.png"), "Create Node...");
    auto const newparameter = menu.addAction(QIcon(":/image/resources/node-insert-next.png"), "Create Parameter...");
    auto const deleteentity = menu.addAction(QIcon(":/image/resources/node-delete-child.png"), "Delete Node");
    auto const sendProviderStateActive = parent != nullptr && parent->parent() == NULL
        ? menu.addAction(QIcon(":/image/resources/control.png"), "Send 'Provider active' message")
        : static_cast<QAction*>(nullptr);
    auto const sendProviderStateBackup = parent != nullptr && parent->parent() == NULL
        ? menu.addAction(QIcon(":/image/resources/control--yellow.png"), "Send 'Provider passive' message")
        : static_cast<QAction*>(nullptr);

    auto const selection = menu.exec(m_position);
    
    if(selection == sendProviderStateActive && sendProviderStateActive != nullptr)
    {
        if (m_proxy)
            m_proxy->writeProviderState(true);
    }
    else if(selection == sendProviderStateBackup && sendProviderStateBackup != nullptr)
    {
        if (m_proxy)
            m_proxy->writeProviderState(false);
    }
    else if (selection == deleteentity)
    {
        m_removeItem = true;
    }
    else if (selection == newnode)
    {
        CreateNodeDialog dialog(m_view, parent);
        if (dialog.exec() == QDialog::Accepted)
        {
            auto const identifier = dialog.identifier();
            auto const description = dialog.description();
            auto node = NodeFactory::createNode(parent, identifier);
            auto item = new QTreeWidgetItem(m_item);

            node->setDescription(description);
            item->setIcon(0, QIcon(":/image/resources/globe-small.png"));
            item->setText(0, ::util::StringConverter::toUtf8QString(node->identifier()));
            item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(node)));
        }
    }
    else if (selection == newparameter)
    {
        CreateParameterDialog dialog(m_view, parent);
        if (dialog.exec() == QDialog::Accepted)
        {
            auto const type = dialog.type();
            auto const identifier = dialog.identifier();
            auto const description = dialog.description();
            auto parameter = static_cast<Parameter*>(nullptr);

            switch(type.value())
            {
                case ParameterType::Enum:
                    parameter = ParameterFactory::create(parent, identifier);
                    break;

                case ParameterType::Integer:
                    parameter = ParameterFactory::create(parent, identifier, 0, 1000, 0);
                    break;

                case ParameterType::Real:
                    parameter = ParameterFactory::create(parent, identifier, 0.0, 1000.0, 0.0);
                    break;

                case ParameterType::String:
                    parameter = ParameterFactory::create(parent, identifier, std::string("text"));
                    break;
                
                case ParameterType::Boolean:
                    parameter = ParameterFactory::create(parent, identifier, false);
                    break;

                default:
                    break;
            }

            if (parameter != nullptr)
            {
                auto item = new QTreeWidgetItem(m_item);
                parameter->setDescription(description);
                item->setIcon(0, QIcon(":/image/resources/leaf.png"));
                item->setText(0, ::util::StringConverter::toUtf8QString(parameter->identifier()));
                item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(parameter)));
            }
        }
    }
}

void GadgetViewContextMenu::exec(gadget::Parameter* parameter)
{
    QMenu menu;
    auto const deleteentity = menu.addAction(
        QIcon(":/image/resources/node-delete-next.png"),
        QString("Delete ") + ::util::StringConverter::toUtf8QString(parameter->identifier()));

    if (menu.exec(m_position) == deleteentity)
        m_removeItem = true;
}
