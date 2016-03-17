#ifndef TINYEMBERPLUS_H
#define TINYEMBERPLUS_H

#include <QtGui/QMainWindow>
#include <qdatetime.h>
#include <qtimer.h>
#include "glow/ProviderInterface.h"
#include "serialization/SettingsSerializer.h"
#include "ui_TinyEmberPlus.h"

/** Forward declarations */
class QModelIndex;
class QTreeWidgetItem;

namespace gadget
{
    class Node;
    class Parameter;
}

namespace glow
{
    class ConsumerProxy;
}

/**
 * The MainWindow of the TinyEmber+ application.
 */
class TinyEmberPlus : public QMainWindow, public glow::ProviderInterface
{
    Q_OBJECT
    public:
        /**
         * Constructor, initializes the main window.
         * @param proxy The consumer proxy which can be used to notify connected consumers.
         * @param parent The parent widget of this main window, usually null.
         * @param flags Optional window flags.
         */
        TinyEmberPlus(::glow::ConsumerProxy* proxy, QWidget *parent = 0, Qt::WFlags flags = 0);

        /** Destructor */
        ~TinyEmberPlus();

        /** @see glow::ProviderInterface::notifyAsync() */
        virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber);

        /** @see glow::ProviderInterface::registerSubscriberAsync() */
        virtual void registerSubscriberAsync(gadget::Subscriber* subscriber);

        /** @see glow::ProviderInterface::unregisterSubscriberAsync() */
        virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber);

    public slots:
        /**
         * All consumer requests are marhsalled to the ui thread and then executed.
         * @param nodeptr The pointer to the decoded node that has been sent by
         *      the consumer.
         * @param subsriberptr A pointer to the subscriber identifying the consumer.
         */
        void synchronizedNotify(void* nodeptr, void *subscriberptr);

        /**
         * Adds a subscriber.
         * @param subscriber The subscriber to register.
         */
        void registerSubscriber(gadget::Subscriber* subscriber);

        /**
         * Removes a subscriber.
         * @param subscriber The subscriber to unregister.
         */
        void unregisterSubscriber(gadget::Subscriber* subscriber);

    private slots:
        /**
         * Loads a file that contains an ember tree which represents the data of the provider.
         */
        void loadFile();

        /**
         * Saves the current provider configuration to a file.
         */
        void saveFile();

        /**
         * Saves the current provider configuration to a file, but offers the possibility
         * to select a filename first.
         */
        void saveFileAs();

        /**
         * Closes the application.
         */
        void exit();

        /**
         * This method is invoked when the selection of the gadget treeview changed.
         * In that case, a view for the new item will be created.
         * @param item The currently selected item.
         */
        void treeItemChanged(QTreeWidgetItem* item, QTreeWidgetItem*);

        /**
         * Display a context menu to create or remove a node or parameter.
         * @param cursor The local cursor where the menu shall be opened.
         */
        void showContextMenu(QPoint cursor);

        /**
         * This timer is used to send keep alive requests to connected consumers.
         */
        void timer();

        /**
         * Updates the notification behavior.
         * @param index The index of the new behavior.
         */
        void notificationBehaviorChanged(int index);

        /**
         * Updates the response behavior.
         * @param index The index of the new behavior.
         */
        void responseBehaviorChanged(int index);

        /**
         * This slot is called when the auto-load flag has changed.
         * @param state The new state.
         */
        void autoLoadConfigStateChanged(bool state);

        /**
         * This slot is called when the 'generate random values' setting has changed.
         * @param state The new state.
         */
        void generateRandomValuesStateChanged(bool state);

        /**
         * The timer slot is used to modify the values of parameters with stream identifier.
         * The values are only updated when the 'Generate random values' option is active.
         */
        void updateStreamTimer();

        /**
         * This slot is called when the 'send keep-alive' option has changed.
         * @param state The new state.
         */
        void updateSendKeepAliveState(bool state);

        /**
         * Updates the "Always Report Online State" option
         */
        void updateAlwaysReportOnlineState(bool state);

        /**
         * Updates the "Use Enum Map" option.
         */
        void updateUseEnumMap(bool state);

    private:
        /**
         * Gets the stored root node from the tree view.
         * @return The root node of the current configuration.
         */
        gadget::Node* root();

        /** 
         * Returns the window title.
         * @return The window title.
         */
        QString title() const;

        /**
         * Rebuilds the tree view.
         * @param root The root node of the current configuration.
         */
        void rebuildTree(gadget::Node* root);

        /**
         * Rebuilds the tree view.
         * @param parent The current gadget node.
         * @param owner The treeview item owning the current node.
         */
        void rebuildTree(gadget::Node* parent, QTreeWidgetItem* owner);

        /**
         * Loads a new configuration from the specified file.
         * @param filename The name of the file to load.
         */
        void loadFile(QString const& filename);

    private:
        Ui::TinyEmberPlusClass m_dialog;
        glow::ConsumerProxy *const m_proxy;
        serialization::SettingsSerializer m_settingsSerializer;
        QTimer* m_timer;
        QDateTime m_lastKeepAliveTransmitTime;
        bool m_generateRandomValues;
        bool m_sendKeepAlive;

        static const QString NotificationBehavior;
        static const QString ResponseBehavior;
        static const QString AutoLoadConfig;
        static const QString GenerateRandomValues;
        static const QString ConfigurationName;
        static const QString StreamTimerInterval;
        static const QString SendKeepAliveRequest;
        static const QString AlwaysReportOnlineState;
        static const QString UseEnumMap;
};

#endif // TINYEMBERPLUS_H
