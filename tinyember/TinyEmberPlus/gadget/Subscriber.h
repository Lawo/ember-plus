#ifndef __TINYEMBER_GADGET_SUBSCRIBER_H
#define __TINYEMBER_GADGET_SUBSCRIBER_H

#include <string>
#include <vector>
#include "util\EntityPath.h"

namespace gadget
{
    /** Forward declaration */
    class Subscribable;

    /**
     * Represents an object that subscribes to or unsubscribes from a Subscribable. This pattern
     * is currently implemented for parameter subscriptions.
     */
    class Subscriber
    {
        typedef std::vector<Subscribable*> SubscribableCollection;
        friend class Subscribable;
        public:
            /**
             * Returns a human readable name (like the ip address of a consumer) that
             * identifies the subscriber.
             * @return A string identifying the subscriber.
             */
            virtual std::string name() const = 0;

            /**
             * Atomically increments the reference counter for this instance.
             */
            void addRef();

            /**
             * Atomically decrements the reference count for this instance. When the counter
             * reaches 0, the Subscriber is being deleted.
             * @return The number of references left after the decrement operation.
             */
            long releaseRef();

            /**
             * Calls unsubscribe for all objects this subscriber is currently subscribed to.
             * @return The number of references left.
             */
            long unsubscribe();

        protected:
            /** Constructor */
            Subscriber();

            /** Destructor */
            virtual ~Subscriber();

        private:
            /**
             * Adds the passed Subscribable to the object's collection of subscribables.
             * @param subscribable The Subscribable to register.
             */
            void subscribe(Subscribable* subscribable);

            /**
             * Removes that passed Subscribable from the object's list.
             * @param subscribable The Subscribable to remove.
             */
            void unsubscribe(Subscribable* subscribable);

        private:
            SubscribableCollection m_subscribables;
            long m_refCount;
    };


    /**
     * Base class for an object that accepts subscriptions.
     */
    class Subscribable
    {
        typedef std::vector<Subscriber*> SubscriberCollection;
        public:
            typedef SubscriberCollection::size_type size_type;

            /** Destructor */
            virtual ~Subscribable();

            /**
             * Adds a new subscriber to this object.
             * @param subscriber The subscriber to add. The implementation calls the subscribe
             *      method of the passed subscriber.
             */
            void subscribe(Subscriber* subscriber);

            /**
             * Removes a subscriber.
             * @param subscriber The subscriber to remove.
             */
            void unsubscribe(Subscriber* subscriber);

            /**
             * Removes all registered subscribers from this Subscribable.
             */
            void unsubscribe();

            /**
             * Returns true when at least one object has subscribed to this Subscribable.
             * @return true when at least one object has subscribed to this Subscribable.
             */
            bool isSubscribed() const;

            size_type subscribers() const;

        protected:
            /**
             * This method is invoked when a new item has subscribed. The default implementation is
             * empty.
             */
            virtual void subscribed();

            /**
             * This method is invoked when a item has unsubscribed from this Subscribable. The default
             * implementation is empty.
             */
            virtual void unsubscribed();

            /**
             * This method is called when the number of subscribers has changed. The default implementation
             * is empty.
             * @param newCount The new number of subscribers.
             */
            virtual void subscriptionCountChanged(size_type newCount);

        private:
            SubscriberCollection m_subscribers;
    };
}

#endif//__TINYEMBER_GADGET_SUBSCRIBER_H

