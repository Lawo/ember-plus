#ifndef __TINYEMBER_GADGET_SUBSCRIBER_H
#define __TINYEMBER_GADGET_SUBSCRIBER_H

#include <string>
#include <vector>
#include "util\EntityPath.h"

namespace gadget
{
    class Subscribable;

    class Subscriber
    {
        typedef std::vector<Subscribable*> SubscribableCollection;
        friend class Subscribable;
        public:
            virtual std::string name() const = 0;

            void addRef();

            long releaseRef();

            long unsubscribe();

        protected:
            Subscriber();

            virtual ~Subscriber();

        private:
            void subscribe(Subscribable* subscribable);

            void unsubscribe(Subscribable* subscribable);

        private:
            SubscribableCollection m_subscribables;
            long m_refCount;
    };


    class Subscribable
    {
        typedef std::vector<Subscriber*> SubscriberCollection;
        public:
            typedef SubscriberCollection::size_type size_type;

            virtual ~Subscribable();

            void subscribe(Subscriber* subscriber);

            void unsubscribe(Subscriber* subscriber);

            void unsubscribe();

            bool isSubscribed() const;

            size_type subscribers() const;

        protected:
            virtual void subscribed();

            virtual void unsubscribed();

            virtual void subscriptionCountChanged(size_type newCount);

        private:
            SubscriberCollection m_subscribers;
    };
}

#endif//__TINYEMBER_GADGET_SUBSCRIBER_H

