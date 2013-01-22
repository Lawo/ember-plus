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
#include "glow\ConsumerProxy.h"
#include "glow\ProviderInterface.h"

class LocalScheduler : public glow::ProviderInterface
{
    public:
        LocalScheduler()
            : m_provider(nullptr)
        {
        }

        virtual ~LocalScheduler()
        {
        }

        virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber)
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->notifyAsync(node, subscriber);
        }

        virtual void registerSubscriberAsync(gadget::Subscriber* subscriber) 
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->registerSubscriberAsync(subscriber);
        }

        virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber) 
        {
            auto provider = m_provider;
            if (provider != nullptr)
                provider->unregisterSubscriberAsync(subscriber);
        }

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
    auto result = -1;
    {
        LocalScheduler scheduler;
        glow::ConsumerProxy proxy(&app, &scheduler);

        TinyEmberPlus window(&proxy);
        window.show();
        scheduler.setSynchronizationObject(&window);
        result = app.exec();
        proxy.close();
    }

    return result;
}

