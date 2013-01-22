#ifndef __TINYEMBER_TREEWIDGETITEMDATA_H
#define __TINYEMBER_TREEWIDGETITEMDATA_H

#include <memory>
#include <qmetatype.h>
#include "gadget/Node.h"
#include "gadget/Parameter.h"
#include "gadget/EnumParameter.h"
#include "gadget/IntegerParameter.h"
#include "gadget/RealParameter.h"
#include "gadget/StringParameter.h"

namespace detail
{
    class EntityStore
    {
        public:
            virtual ~EntityStore()
            {}
    };

    template<typename EntityType>
    struct EntityStoreImpl : EntityStore
    {
        public:
            explicit EntityStoreImpl(EntityType* payload)
                : m_payload(payload)
            {}

            EntityType* payload() const
            {
                return m_payload;
            }

        private:
            EntityType* m_payload;
    };
}

struct TreeWidgetItemDataType
{
    public:
        enum _Domain
        {
            Unknown,
            Node,
            Parameter,
        };

        typedef unsigned int value_type;

        TreeWidgetItemDataType(_Domain value)
            : m_value(value)
        {}

        value_type value() const
        {
            return m_value;
        }

    private:
        value_type m_value;
};

class TreeWidgetItemData
{
    public:
        TreeWidgetItemData()
            : m_type(TreeWidgetItemDataType::Unknown)
            , m_payload(nullptr)
        {}

        explicit TreeWidgetItemData(gadget::Node* node)
            : m_type(TreeWidgetItemDataType::Node)
            , m_payload(new detail::EntityStoreImpl<gadget::Node>(node))
        {}

        explicit TreeWidgetItemData(gadget::Parameter *parameter)
            : m_type(TreeWidgetItemDataType::Parameter)
            , m_payload(new detail::EntityStoreImpl<gadget::Parameter>(parameter))
        {}

        TreeWidgetItemDataType const& type() const
        {
            return m_type;
        }

        template<typename EntityType>
        EntityType* payload() const
        {
            auto store = m_payload.get();
            if (store != nullptr)
            {
                return static_cast<detail::EntityStoreImpl<EntityType>*>(store)->payload();
            }
            else
            {
                return nullptr;
            }
        }

    private:
        TreeWidgetItemDataType m_type;
        std::shared_ptr<detail::EntityStore> m_payload;
};

Q_DECLARE_METATYPE(TreeWidgetItemData)

#endif//__TINYEMBER_TREEWIDGETITEMDATA_H
