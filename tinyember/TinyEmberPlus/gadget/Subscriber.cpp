#include <algorithm>
#include "Subscriber.h"

namespace gadget
{
    Subscriber::Subscriber()
        : m_refCount(1)
    {}

    Subscriber::~Subscriber()
    {}

    void Subscriber::addRef()
    {
        m_refCount = m_refCount + 1;
    }

    long Subscriber::releaseRef()
    {
        if (m_refCount == 1)
        {
            delete this;
            return 0;
        }
        else
        {
            m_refCount = m_refCount - 1;
            return m_refCount;
        }
    }

    long Subscriber::unsubscribe()
    {
        addRef();
        auto subscribables = m_subscribables;
        for(auto subscribable : subscribables)
        {
            subscribable->unsubscribe(this);
        }
        return releaseRef();
    }

    void Subscriber::subscribe(Subscribable* subscribable)
    {
        auto const first = std::begin(m_subscribables);
        auto const last = std::end(m_subscribables);
        auto const result = std::find_if(first, last, [&subscribable](decltype(*first) entry) -> bool
        {
            return entry == subscribable;
        });

        if (result == last)
        {
            m_subscribables.push_back(subscribable);
        }
    }

    void Subscriber::unsubscribe(Subscribable* subscribable)
    {
        auto const first = std::begin(m_subscribables);
        auto const last = std::end(m_subscribables);
        auto const where = std::find_if(first, last, [&subscribable](decltype(*first) entry) -> bool
        {
            return entry == subscribable;
        });

        m_subscribables.erase(where);
    }


    Subscribable::~Subscribable()
    {
        unsubscribe();
    }

    void Subscribable::subscribe(Subscriber* subscriber)
    {
        if (subscriber != nullptr)
        {
            auto const isSubscribed = this->isSubscribed();
            auto const first = std::begin(m_subscribers);
            auto const last = std::end(m_subscribers);
            auto const result = std::find_if(first, last, [&subscriber](decltype(*first) entry) -> bool
            {
                return entry == subscriber;
            });

            if (result == last)
            {
                m_subscribers.push_back(subscriber);

                subscriber->subscribe(this);
                subscriber->addRef();
                subscriptionCountChanged(subscribers());

                if (isSubscribed == false)
                    subscribed();
            }
        }
    }

    void Subscribable::unsubscribe(Subscriber* subscriber)
    {
        if (subscriber != nullptr)
        {
            auto const isSubscribed = this->isSubscribed();
            auto const first = std::begin(m_subscribers);
            auto const last = std::end(m_subscribers);
            auto const result = std::find_if(first, last, [&subscriber](decltype(*first) entry) -> bool
            {
                return entry == subscriber;
            });

            if (result != last)
            {
                m_subscribers.erase(result);

                subscriber->unsubscribe(this);
                subscriber->releaseRef();
                subscriptionCountChanged(subscribers());
            }

            if (isSubscribed == true && this->isSubscribed() == false)
                unsubscribed();
        }
    }

    void Subscribable::unsubscribe()
    {
        auto const isSubscribed = this->isSubscribed();
        auto subscribers = m_subscribers;
        for(auto subscriber : subscribers)
        {
            subscriber->unsubscribe(this);
            subscriber->releaseRef();
        }
    }

    bool Subscribable::isSubscribed() const
    {
        return m_subscribers.empty() == false;
    }

    void Subscribable::subscribed()
    {
    }

    void Subscribable::unsubscribed()
    {
    }

    void Subscribable::subscriptionCountChanged(size_type newCount)
    {
    }

    Subscribable::size_type Subscribable::subscribers() const
    {
        return m_subscribers.size();
    }
}
