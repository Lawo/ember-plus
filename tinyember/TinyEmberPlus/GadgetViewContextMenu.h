#ifndef __TINYEMBER_GADGETVIEWCONTEXTMENU_H
#define __TINYEMBER_GADGETVIEWCONTEXTMENU_H

#include "gadget/Node.h"
#include "gadget/Parameter.h"
#include <qtreewidget.h>

namespace glow {
    class ConsumerProxy;
}

/**
 * Manages a context menu which can be used to create or remove nodes or parameters.
 */
class GadgetViewContextMenu
{
    public:
        /**
         * Initializes a new GadgetViewContext menu.
         * @param proxy The proxy which manages the client connections.
         * @param view A pointer to the gadget tree view.
         * @param cursor The top-left position of the context menu.
         */
        GadgetViewContextMenu(::glow::ConsumerProxy* proxy, QTreeWidget* view, QPoint const& cursor);

        /**
         * Runs the context menu.
         */
        void exec();

    private:
        /**
         * Runs the context menu for a selected node.
         * @param parent The selected node.
         */
        void exec(gadget::Node* parent);

        /**
         * Runs the context menu for a selected parameter.
         * @param parameter The selected parameter.
         */
        void exec(gadget::Parameter* parameter);

    private:
        QTreeWidget *const m_view;
        QTreeWidgetItem * const m_item;
        QPoint const m_position;
        ::glow::ConsumerProxy* m_proxy;
        bool m_removeItem;
};

#endif//__TINYEMBER_GADGETVIEWCONTEXTMENU_H
