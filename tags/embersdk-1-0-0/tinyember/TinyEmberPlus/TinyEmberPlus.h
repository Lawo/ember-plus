#ifndef TINYEMBERPLUS_H
#define TINYEMBERPLUS_H

#include <QtGui/QMainWindow>
#include <qdatetime.h>
#include <qtimer.h>
#include "glow\ProviderInterface.h"
#include "serialization\SettingsSerializer.h"
#include "ui_TinyEmberPlus.h"

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


class TinyEmberPlus : public QMainWindow, public glow::ProviderInterface
{
    Q_OBJECT
    public:
        TinyEmberPlus(::glow::ConsumerProxy* proxy, QWidget *parent = 0, Qt::WFlags flags = 0);

        ~TinyEmberPlus();

        virtual void notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber);

        virtual void registerSubscriberAsync(gadget::Subscriber* subscriber);

        virtual void unregisterSubscriberAsync(gadget::Subscriber* subscriber);

    public slots:
        void synchronizedNotify(void* nodeptr, void *subscriberptr);
        void registerSubscriber(gadget::Subscriber* subscriber);
        void unregisterSubscriber(gadget::Subscriber* subscriber);

    private slots:
        void loadFile();
        void saveFile();
        void saveFileAs();
        void exit();
        void treeItemChanged(QTreeWidgetItem* item, QTreeWidgetItem*);
        void showContextMenu(QPoint cursor);
        void timer();
        void notificationBehaviorChanged(int index);
        void responseBehaviorChanged(int index);
        void autoLoadConfigStateChanged(bool state);
        void generateRandomValuesStateChanged(bool state);
        void updateStreamTimer();
        void updateSendKeepAliveState(bool state);

    private:
        gadget::Node* root();

        QString title() const;

        void rebuildTree(gadget::Node* root);

        void rebuildTree(gadget::Node* parent, QTreeWidgetItem* owner);

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
};

#endif // TINYEMBERPLUS_H
