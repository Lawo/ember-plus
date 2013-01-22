#ifndef __TINYEMBER_GLOW_SETTINGS_H
#define __TINYEMBER_GLOW_SETTINGS_H

namespace glow
{
    struct ResponseBehavior
    {
        enum _Domain
        {
            Default,
            ForceQualifiedContainer,
            ForceExpandedContainer,
        };

        typedef unsigned int value_type;
        ResponseBehavior(_Domain value)
            : m_value(value)
        {}

        value_type value() const
        {
            return m_value;
        }

        private:
            value_type m_value;
    };


    struct NotificationBehavior
    {
        enum _Domain
        {
            UseQualifiedContainer,
            UseExpandedContainer,
        };

        typedef unsigned int value_type;
        NotificationBehavior(_Domain value)
            : m_value(value)
        {}

        value_type value() const
        {
            return m_value;
        }

        private:
            value_type m_value;
    };


    class ConsumerProxy;


    class Settings
    {
        friend class ConsumerProxy;
        public:
            ResponseBehavior responseBehavior() const;

            NotificationBehavior notificationBehavior() const;

            void setResponseBehavior(ResponseBehavior const& value);

            void setNotificationBehavior(NotificationBehavior const& value);
            
        private:
            Settings();

            ResponseBehavior m_responseBehavior;
            NotificationBehavior m_notificationBehavior;
    };

    inline ResponseBehavior Settings::responseBehavior() const
    {
        return m_responseBehavior;
    }

    inline NotificationBehavior Settings::notificationBehavior() const
    {
        return m_notificationBehavior;
    }

    inline void Settings::setResponseBehavior(ResponseBehavior const& value)
    {
        m_responseBehavior = value;
    }

    inline void Settings::setNotificationBehavior(NotificationBehavior const& value)
    {
        m_notificationBehavior = value;
    }
            
    inline Settings::Settings()
        : m_responseBehavior(ResponseBehavior::Default)
        , m_notificationBehavior(NotificationBehavior::UseExpandedContainer)
    {
    }

}

#endif//__TINYEMBER_GLOW_SETTINGS_H
