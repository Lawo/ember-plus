#ifndef TREEWIDGETITEMDATA_H
#define TREEWIDGETITEMDATA_H

#include <QMetaType>
#include "gadgets/Entity.h"

class QFrame;

/**
 * Stores a single entity and provides a method to create a property for
 * the entity.
 */
class TreeWidgetItemData
{
public:
    /**
     * Constructor
     */
    TreeWidgetItemData();

    /**
     * Copy constructor
     */
    TreeWidgetItemData(TreeWidgetItemData const& other);

    /**
     * Constructor
     * @param entity The entity to watch
     */
    explicit
    TreeWidgetItemData(gadgets::Entity *entity);

    /**
     * Creates a view for the entity this item stores.
     * @return Returns a view widget for the entity
     */
    QFrame* createView(QWidget *parent) const;

    /**
     * Assignment operator
     */
    TreeWidgetItemData &operator=(TreeWidgetItemData const& other);

    /**
     * Returns the stored entity
     */
    gadgets::Entity* entity() const;

private:
    gadgets::Entity* m_entity;
};

/******************************************************************************
 * Inline implementation                                                      *
 ******************************************************************************/

inline gadgets::Entity* TreeWidgetItemData::entity() const
{
    return m_entity;
}

Q_DECLARE_METATYPE(TreeWidgetItemData)


#endif // TREEWIDGETITEMDATA_H
