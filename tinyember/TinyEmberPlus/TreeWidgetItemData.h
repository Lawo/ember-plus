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
    /**
     * Base class which is used to store a gadget node or parameter.
     */
    class EntityStore
    {
        public:
            /** Destructor */
            virtual ~EntityStore()
            {}
    };

    /**
     * Templated implementation the EntityStore
     */
    template<typename EntityType>
    struct EntityStoreImpl : EntityStore
    {
        public:
            /**
             * Constructor
             * @param payload A pointer to the object to store.
             */
            explicit EntityStoreImpl(EntityType* payload)
                : m_payload(payload)
            {}

            /**
             * Returns the stored item.
             * @return The stored item.
             */
            EntityType* payload() const
            {
                return m_payload;
            }

        private:
            EntityType* m_payload;
    };
}

/**
 * Scoped enumeration which contains the symbolic names of the possible
 * object types that may be stored within a TreeItem.
 */
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

        /**
         * Initializes a new TreeWidgetItemDataType.
         * @param value The value to initialize this instance with.
         */
        TreeWidgetItemDataType(_Domain value)
            : m_value(value)
        {}

        /**
         * Returns the numeric representation of the current object type.
         * @return The numeric representation of the current object type.
         */
        value_type value() const
        {
            return m_value;
        }

    private:
        value_type m_value;
};


/**
 * This class stores a node or parameter and is attached to the items of the gadget
 * treeview.
 */
class TreeWidgetItemData
{
    public:
        /** Constructor */
        TreeWidgetItemData()
            : m_type(TreeWidgetItemDataType::Unknown)
            , m_payload(nullptr)
        {}

        /**
         * Initializes a new TreeWidgetItemData instance.
         * @param node The node to store.
         */
        explicit TreeWidgetItemData(gadget::Node* node)
            : m_type(TreeWidgetItemDataType::Node)
            , m_payload(new detail::EntityStoreImpl<gadget::Node>(node))
        {}

        /**
         * Initializes a new TreeWidgetItemData instance.
         * @param parameter The parameter to store.
         */
        explicit TreeWidgetItemData(gadget::Parameter *parameter)
            : m_type(TreeWidgetItemDataType::Parameter)
            , m_payload(new detail::EntityStoreImpl<gadget::Parameter>(parameter))
        {}

        /**
         * Returns the type of the stored object.
         * @return The type of the stored object.
         */
        TreeWidgetItemDataType const& type() const
        {
            return m_type;
        }

        /**
         * Casts the internal object to the specified type and returns it.
         * @return The internal object converted to the specified type.
         */
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
