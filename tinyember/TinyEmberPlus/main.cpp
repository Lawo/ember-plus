//#include "TinyEmberPlus.h"
//#include <QtGui/QApplication>
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    TinyEmberPlus w;
//    w.show();
//    return a.exec();
//}
#include "TinyEmberPlus.h"
#include <memory>
#include <QtGui/QApplication>

#include "gadget/Subscriber.h"
#include "glow/ConsumerProxy.h"
#include "glow/ProviderInterface.h"

/**
 * The LocalScheduler class marshalls asynchronous consumer request to the ui thread,
 * where the requests are being handled.
 */
class LocalScheduler : public glow::ProviderInterface
{
    public:
        /** Constructor */
        LocalScheduler()
            : m_provider(nullptr)
        {
        }

        /** Destructor */
        virtual ~LocalScheduler()
        {
        }

        /**
         * Forwards the request to the attached provider implementation.
         * @param node The gadget tree sent by a consumer.
         * @param subscriber The subscriber object representing the consumer.
         */
        virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber)
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->notifyAsync(node, subscriber);
        }

        /**
         * Forwards the register request to the attached provider implementation.
         * @param subscriber The subscriber object representing the consumer.
         */
        virtual void registerSubscriberAsync(gadget::Subscriber* subscriber) 
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->registerSubscriberAsync(subscriber);
        }

        /**
         * Forwards the unregister request to the attached provider implementation.
         * @param subscriber The subscriber object representing the consumer.
         */
        virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber) 
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->unregisterSubscriberAsync(subscriber);
        }

        /**
         * Updates the provider where consumer requests are forwarded to.
         * @param provider The provider to forward consumer requests to.
         */
        void setSynchronizationObject(glow::ProviderInterface* provider)
        {
            m_provider = provider;
        }

    private:
        glow::ProviderInterface* m_provider;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto first = argv;
    auto const last = argv + argc;
    auto port = short(9000);

    for (; first != last; first++)
    {
        auto const item = QString(*first);
        if (item.contains("port", Qt::CaseInsensitive) && (std::next(first) != last))
        {
            std::advance(first, 1);
            port = QString(*first).toShort();
            break;
        }
    }

    auto result = -1;
    {
        LocalScheduler scheduler;
        glow::ConsumerProxy proxy(&app, &scheduler, port);

        TinyEmberPlus window(&proxy);
        window.show();
        scheduler.setSynchronizationObject(&window);
        result = app.exec();
        proxy.close();
    }

    return result;
}

