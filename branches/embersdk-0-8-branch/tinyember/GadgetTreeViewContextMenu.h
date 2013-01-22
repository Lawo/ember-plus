#ifndef GADGETTREEVIEWCONTEXTMENU_H
#define GADGETTREEVIEWCONTEXTMENU_H

#include "gadgets/EntityTypeVisitor.h"

/** Forward declarations **/
class TreeWidgetItemData;
class QTreeWidget;
class QTreeWidgetItem;

namespace gadgets
{
    class Entity;
}

/**
 * Generates a context menu where the entries depend on the type
 * of the provided entity.
 */
class GadgetTreeViewContextMenu : private gadgets::EntityTypeVisitor
{
public:
    /**
     * Constructor
     * @param view the gadget view container.
     * @param localCursor The current cursor position.
     */
    GadgetTreeViewContextMenu(QTreeWidget* view, QPoint const& localCursor);

    /**
     * Executes the context menu in a modal state.
     */
    void exec();

private:
    /**
     * @note See EntityTypeVisitor.
     */
    virtual void visit(gadgets::Node& node);

    /**
     * @note See EntityTypeVisitor.
     */
    virtual void visit(gadgets::Parameter& parameter, gadgets::states::NumericState<int>& state);

    /**
     * @note See EntityTypeVisitor.
     */
    virtual void visit(gadgets::Parameter& parameter, gadgets::states::NumericState<double>& state);

    /**
     * @note See EntityTypeVisitor.
     */
    virtual void visit(gadgets::Parameter& parameter, gadgets::states::EnumState& state);

    /**
     * @note See EntityTypeVisitor.
     */
    virtual void visit(gadgets::Parameter& parameter, gadgets::states::StringState& state);

    /**
     * Displays the context menu when a parameter has been selected.
     * @param parameter Selected parameter
     */
    void showParameterMenu(gadgets::Parameter& parameter);

    /**
     * Displays the context menu when a node has been selected.
     * @param node Selected node.
     */
    void showNodeMenu(gadgets::Node& node);

private:
    QTreeWidget* const m_view;
    QTreeWidgetItem* const m_item;
    QPoint const m_position;
    bool m_removeItem;
};

#endif // GADGETTREEVIEWCONTEXTMENU_H
