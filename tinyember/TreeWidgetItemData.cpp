#include "TreeWidgetItemData.h"
#include"EntityViewFactory.h"

TreeWidgetItemData::TreeWidgetItemData()
    : m_entity(0)
{
}


TreeWidgetItemData::TreeWidgetItemData(gadgets::Entity *entity)
    : m_entity(entity)
{
}

TreeWidgetItemData::TreeWidgetItemData(TreeWidgetItemData const& other)
    : m_entity(other.m_entity)
{
}

TreeWidgetItemData &TreeWidgetItemData::operator=(TreeWidgetItemData const& other)
{
    m_entity = other.m_entity;
    return *this;
}

QFrame* TreeWidgetItemData::createView(QWidget *parent) const
{
    return EntityViewFactory::createView(parent, m_entity);
}

