#ifndef __TINYEMBER_GLOW_PROVIDERINTERFACE_H
#define __TINYEMBER_GLOW_PROVIDERINTERFACE_H

namespace libember { namespace dom 
{
    class Node;
}
}

namespace gadget
{
    class Subscriber;
}

namespace glow 
{
    class ProviderInterface
    {
        public:
            virtual ~ProviderInterface()
            {}

            virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber) = 0;

            virtual void registerSubscriberAsync(gadget::Subscriber* subscriber) = 0;

            virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber) = 0;
    };
}

#endif//__TINYEMBER_GLOW_PROVIDERINTERFACE_H

