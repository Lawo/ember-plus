#ifndef __TINYEMBER_GLOW_PROVIDERINTERFACE_H
#define __TINYEMBER_GLOW_PROVIDERINTERFACE_H

/** Forward declarations */
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
    /**
     * The ProviderInterface is used by a consumer to post its requests.
     */
    class ProviderInterface
    {
        public:
            /** Destructor */
            virtual ~ProviderInterface()
            {}

            /**
             * This method is used to forward a consumer request to the local provider. The request
             * may executed asynchronously, therefore the passed node must not be modifed by the 
             * caller after it invoked this method.
             * @param node The node containing the request. This node must be manually deleted by 
             *      the implementation when it is no longer needed.
             * @param subscriber The subscriber representing the connected consumer.
             */
            virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber) = 0;

            /**
             * Registers a new subscriber.
             * @param subscriber The subscriber to register.
             */
            virtual void registerSubscriberAsync(gadget::Subscriber* subscriber) = 0;

            /**
             * Unregisters a subscriber.
             * @param subscriber The subscriber to unregister.
             */
            virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber) = 0;
    };
}

#endif//__TINYEMBER_GLOW_PROVIDERINTERFACE_H

