#ifndef __TINYEMBER_GADGETVIEWCONTEXTMENU_H
#define __TINYEMBER_GADGETVIEWCONTEXTMENU_H

#include "gadget\Node.h"
#include "gadget\Parameter.h"
#include <qtreewidget.h>

class GadgetViewContextMenu
{
    public:
        GadgetViewContextMenu(QTreeWidget* view, QPoint const& cursor);

        void exec();

    private:
        void exec(gadget::Node* parent);

        void exec(gadget::Parameter* parameter);

    private:
        QTreeWidget *const m_view;
        QTreeWidgetItem * const m_item;
        QPoint const m_position;
        bool m_removeItem;
};

#endif//__TINYEMBER_GADGETVIEWCONTEXTMENU_H
