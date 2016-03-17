#include "Consumer.h"
#include "ConsumerProxy.h"
#include "Encoder.h"
#include "util/NodeConverter.h"
#include "util/ParameterConverter.h"
#include "../gadget/Node.h"
#include "../gadget/StreamManager.h"
#include "../gadget/Parameter.h"

namespace glow
{
    Settings ConsumerProxy::s_settings;

    Settings& ConsumerProxy::settings()
    {
        return s_settings;
    }

    ConsumerProxy::ConsumerProxy(QApplication* app, ProviderInterface* provider, short port)
        : m_provider(provider)
    {
        m_server = new net::TcpServer(app, this, port);
    }

    ConsumerProxy::~ConsumerProxy()
    {
        close();
    }

    void ConsumerProxy::close()
    {
        auto server = m_server;
        if (server != nullptr)
        {
            server->close();
            delete server;
        }

        m_server = nullptr;
    }

    Consumer* ConsumerProxy::create(QTcpSocket* socket)
    {
        return new Consumer(m_provider, socket);
    }

    void ConsumerProxy::writeRequestKeepAlive()
    {
        auto const result = Encoder::createRequestKeepAliveMessage();
        auto server = m_server;
        if (server != nullptr)
        {
            auto const last = result.end();
            for(auto it = result.begin(); it != last; ++it)
            {
                server->write(it->begin(), it->end());
            }
        }
    }

    void ConsumerProxy::writeProviderState(bool state)
    {
        auto const result = Encoder::createProviderStateMessage(state);
        auto server = m_server;
        if (server != nullptr)
        {
            auto const last = result.end();
            for(auto it = result.begin(); it != last; ++it)
            {
                server->write(it->begin(), it->end());
            }
        }
    }

    void ConsumerProxy::write(libember::glow::GlowContainer const* container)
    {
        // The createEmberMessage returns an Encoder which may contain several packets
        // that need to be transmitted separately.
        auto const result = Encoder::createEmberMessage(container);
        auto server = m_server;
        if (server != nullptr)
        {
            auto const last = result.end();
            for(auto it = result.begin(); it != last; ++it)
            {
                server->write(it->begin(), it->end());
            }
        }
    }

    void ConsumerProxy::notifyStateChanged(gadget::NodeFieldState const& state, gadget::Node const* object)
    {
        using namespace libember;
        using namespace libember::glow;

        if (isNotificationRequired(object))
        {
            auto const root = GlowRootElementCollection::create();
            auto const behavior = settings().notificationBehavior().value();

            if (behavior == NotificationBehavior::UseExpandedContainer)
                transform(root, object);
            else
                transformQualified(root, object);

            if (root->size() > 0)
                write(root);
            delete root;

            object->clearDirtyState(true);
        }
    }

    bool ConsumerProxy::isNotificationRequired(gadget::Node const* node) const
    {
        if (node != nullptr)
        {
            auto const nodeState = node->dirtyState().mask(~gadget::NodeField::DirtyChildEntity);
            if (nodeState.isDirty())
            {
                return true;
            }
            else if(node->isDirty())
            {
                auto const& manager = gadget::StreamManager::instance();
                auto const& nodes = node->nodes();
                auto const& parameters = node->parameters();
                for(auto parameter : parameters)
                {
                    if (manager.isParameterTransmittedViaStream(parameter) && parameter->dirtyState().isSet(gadget::ParameterField::ForceUpdate) == false)
                    {
                        auto const state = parameter->dirtyState().mask(~gadget::ParameterField::Value);
                        if (state.isDirty())
                            return true;
                    }
                    else
                    {
                        auto const state = parameter->dirtyState();
                        if (state.isDirty())
                            return true;
                    }
                }

                for(auto child : nodes)
                {
                    auto const isDirty = isNotificationRequired(child);
                    if (isDirty)
                        return true;
                }
            }
        }

        return false;
    }

    void ConsumerProxy::transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node)
    {
        auto const state = node->dirtyState().mask(~gadget::NodeField::DirtyChildEntity);
        if (state.isDirty())
        {
            util::NodeConverter::createQualified(root, node, state);
        }

        auto const& nodes = node->nodes();
        for(auto child : nodes)
        {
            transformQualified(root, child);
        }

        auto const& parameters = node->parameters();
        for(auto parameter : parameters)
        {
            transformQualified(root, parameter);
        }
    }

    void ConsumerProxy::transform(libember::glow::GlowContainer* parent, gadget::Node const* node) const
    {
        if (isNotificationRequired(node))
        {
            parent = util::NodeConverter::create(parent, node, node->dirtyState())->children();

            auto const& nodes = node->nodes();
            for(auto child : nodes)
            {
                if (child->isDirty())
                    transform(parent, child);
            }

            auto const& parameters = node->parameters();
            for(auto parameter : parameters)
            {
                transform(parent, parameter);
            }
        }
    }
    
    void ConsumerProxy::transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter)
    {
        auto const& manager = gadget::StreamManager::instance();
        auto state = parameter->dirtyState();
        if (manager.isParameterTransmittedViaStream(parameter))
        {
            /**
             * Do not notify the value when it is already transmitted via a stream.
             */
            state = state.mask(~gadget::ParameterField::Value);
        }

        if (state.isDirty())
        {
            util::ParameterConverter::createQualified(root, parameter, state);
        }
    }

    void ConsumerProxy::transform(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter) const
    {
        auto const& manager = gadget::StreamManager::instance();
        auto state = parameter->dirtyState();
        if (manager.isParameterTransmittedViaStream(parameter) && state.isSet(gadget::ParameterField::ForceUpdate) == false)
        {
            /**
             * Do not notify the value when it is already transmitted via a stream.
             */
            state = state.mask(~gadget::ParameterField::Value);
        }

        if (state.isDirty())
        {
            util::ParameterConverter::create(parent, parameter, state);
        }
    }
}
