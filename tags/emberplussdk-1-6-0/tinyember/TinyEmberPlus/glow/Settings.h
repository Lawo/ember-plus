#ifndef __TINYEMBER_GLOW_SETTINGS_H
#define __TINYEMBER_GLOW_SETTINGS_H

namespace glow
{
    /**
     * Scoped enumeration which contains the symbolic names of the available
     * response behaviors.
     */
    struct ResponseBehavior
    {
        enum _Domain
        {
            Default,
            ForceQualifiedContainer,
            ForceExpandedContainer,
        };

        typedef unsigned int value_type;

        /**
         * Constructor.
         * @param value The behavior to initialize this instance with.
         */
        ResponseBehavior(_Domain value)
            : m_value(value)
        {}

        /**
         * Returns the numeric value of the current behavior.
         * @return The numeric value of the current behavior.
         */
        value_type value() const
        {
            return m_value;
        }

        private:
            value_type m_value;
    };

    /**
     * Scoped enumeration which contains the symbolic names of the available
     * notification behaviors.
     */
    struct NotificationBehavior
    {
        enum _Domain
        {
            UseQualifiedContainer,
            UseExpandedContainer,
        };

        typedef unsigned int value_type;

        /**
         * Constructor.
         * @param value The value to initialize this instance with.
         */
        NotificationBehavior(_Domain value)
            : m_value(value)
        {}

        /**
         * Returns the numeric value of the current behavior.
         * @return The numeric value of the current behavior.
         */
        value_type value() const
        {
            return m_value;
        }

        private:
            value_type m_value;
    };

    /** Forward declaration */
    class ConsumerProxy;

    /**
     * This class stores the basic settings that define the response and notification behavior
     * of the local provider.
     */
    class Settings
    {
        friend class ConsumerProxy;
        public:
            /**
             * Returns the current response behavior.
             * @return The current response behavior.
             */
            ResponseBehavior responseBehavior() const;

            /**
             * Returns the current notification behavior.
             * @return The current notification behavior.
             */
            NotificationBehavior notificationBehavior() const;

            /**
             * Returns true if the online state should always be reported;
             * false if it will be only be reported when the node is online.
             * @return true if the online state should always be reported.
             */ 
            bool alwaysReportOnlineState() const;

            /**
             * Returns true if the parameter enumerations shall be
             * transmitted as enum map.
             * @return true if the parameter enumerations are transmitted
             *      as enum map.
             */
            bool useEnumMap() const;

            /**
             * Updates the response behavior.
             * @param value The new response behavior.
             */
            void setResponseBehavior(ResponseBehavior const& value);

            /**
             * Updates the notification behavior.
             * @param value The new notification behavior.
             */
            void setNotificationBehavior(NotificationBehavior const& value);

            /**
             * Updates the "Always Report Online State" property. If set to
             * true, a node always reports its online state. Otherwise,
             * it is only reported when the node is offline.
             * @param value The new value for this option.
             */
            void setAlwaysReportOnlineState(bool value);

            /**
             * Updates the "Use Enum Map" property. If set, the parameter
             * enumerations are transmitted as map instead of a single
             * string.
             * @param value The value for this option.
             */
            void setUseEnumMap(bool value);

        private:
            /** Constructor */
            Settings();

            bool m_useEnumMap;
            bool m_alwaysReportOnlineState;
            ResponseBehavior m_responseBehavior;
            NotificationBehavior m_notificationBehavior;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline bool Settings::alwaysReportOnlineState() const
    {
        return m_alwaysReportOnlineState;
    }

    inline ResponseBehavior Settings::responseBehavior() const
    {
        return m_responseBehavior;
    }

    inline NotificationBehavior Settings::notificationBehavior() const
    {
        return m_notificationBehavior;
    }

    inline bool Settings::useEnumMap() const
    {
        return m_useEnumMap;
    }

    inline void Settings::setResponseBehavior(ResponseBehavior const& value)
    {
        m_responseBehavior = value;
    }

    inline void Settings::setNotificationBehavior(NotificationBehavior const& value)
    {
        m_notificationBehavior = value;
    }

    inline void Settings::setAlwaysReportOnlineState(bool value)
    {
        m_alwaysReportOnlineState = value;
    }

    inline void Settings::setUseEnumMap(bool value)
    {
        m_useEnumMap = value;
    }

    inline Settings::Settings()
        : m_responseBehavior(ResponseBehavior::Default)
        , m_notificationBehavior(NotificationBehavior::UseExpandedContainer)
        , m_useEnumMap(false)
    {}
}

#endif//__TINYEMBER_GLOW_SETTINGS_H
