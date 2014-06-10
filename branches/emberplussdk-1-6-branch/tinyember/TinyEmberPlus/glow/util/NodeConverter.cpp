#include <ember\Ember.hpp>
#include "../../gadget/Node.h"
#include "../../gadget/util/EntityPath.h"
#include "glow/ConsumerProxy.h"
#include "NodeConverter.h"

using namespace gadget;
using namespace libember;
using namespace libember::glow;

namespace glow { namespace util
{
    libember::glow::GlowNodeBase* NodeConverter::create(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields)
    {
        auto const result = NodeConverter(parent, node, fields, false);
        return result.m_node;
    }

    libember::glow::GlowNodeBase* NodeConverter::createStructured(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields)
    {
        auto flags = fields;
        auto child = static_cast<GlowNodeBase*>(nullptr);
        auto first = static_cast<GlowNodeBase*>(nullptr);

        while(node != nullptr)
        {
            auto local = NodeConverter::create(nullptr, node, flags);
            if (first == nullptr)
            {
                first = local;
            }

            if (child != nullptr)
            {
                auto container = local->children();
                container->insert(container->end(), child);
            }

            flags = NodeField::None;
            child = local;
            node = node->parent();

            if (node == nullptr)
                root->insert(root->end(), local);
        }

        return first;
    }

    libember::glow::GlowNodeBase* NodeConverter::createQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields)
    {
        auto const result = NodeConverter(root, node, fields, true);
        return result.m_node;
    }

    NodeConverter::NodeConverter(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields, bool makeQualified)
    {
        if(makeQualified)
        {
            auto const path = gadget::util::make_path(node);
            m_node = new libember::glow::GlowQualifiedNode(libember::ber::ObjectIdentifier(path.begin(), path.end()));
        }
        else
        {
            m_node = new libember::glow::GlowNode(node->number());
        }

        if (parent)
            parent->insert(parent->end(), m_node);
        
        if (fields.isSet(NodeField::Identifier))
            m_node->setIdentifier(node->identifier());

        if (fields.isSet(NodeField::Description))
            m_node->setDescription(node->description());

        if (fields.isSet(NodeField::Schema))
        {
            String const& schema = node->schema();

            if (schema.empty() == false)
            {
                m_node->setSchemaIdentifiers(schema);
            }
        }

        if(ConsumerProxy::settings().alwaysReportOnlineState())
        {
            m_node->setIsOnline(node->isOnline());
        }
        else
        {
            if (node->isOnline() == false)
            {
                m_node->setIsOnline(false);
            }
        }
    }
}
}
