#include "gadget/Node.h"
#include "gadget/Parameter.h"
#include "gadget/EnumParameter.h"
#include "gadget/IntegerParameter.h"
#include "gadget/RealParameter.h"
#include "gadget/StreamManager.h"
#include "glow/ConsumerRequestProcessor.h"
#include "glow/ConsumerProxy.h"
#include "glow/util/StreamConverter.h"
#include "serialization/Archive.h"
#include "util/StringConverter.h"
#include "GadgetViewContextMenu.h"
#include "TinyEmberPlus.h"
#include "TreeWidgetItemData.h"
#include "ViewFactory.h"
#include <ember/Ember.hpp>
#include <sstream>
#include <qdialog.h>
#include <qfileinfo.h>
#include <qfiledialog.h>

using namespace ::glow;
using namespace ::libember;
using namespace ::serialization;

const QString TinyEmberPlus::NotificationBehavior = "NotificationBehavior";
const QString TinyEmberPlus::ResponseBehavior = "ResponseBehavior";
const QString TinyEmberPlus::AlwaysReportOnlineState = "AlwaysReportOnlineState";
const QString TinyEmberPlus::AutoLoadConfig = "AutoLoadConfig";
const QString TinyEmberPlus::ConfigurationName = "ConfigurationName";
const QString TinyEmberPlus::GenerateRandomValues = "GenerateRandomValues";
const QString TinyEmberPlus::StreamTimerInterval = "StreamTimerInterval";
const QString TinyEmberPlus::SendKeepAliveRequest = "SendKeepAliveRequest";
const QString TinyEmberPlus::UseEnumMap = "UseEnumMap";

TinyEmberPlus::TinyEmberPlus(::glow::ConsumerProxy* proxy, QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
    , m_proxy(proxy)
    , m_settingsSerializer("Settings.xml")
    , m_generateRandomValues(false)
    , m_sendKeepAlive(false)
    , m_lastKeepAliveTransmitTime(QDateTime::currentDateTimeUtc())
{
    m_dialog.setupUi(this);
    m_dialog.gadgetTreeView->setContextMenuPolicy( Qt::CustomContextMenu );
    m_dialog.portHintLabel->setText(QString("Listening on TCP/IP port ") + QVariant(proxy->port()).toString());

    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    m_timer->setSingleShot(false);
    m_timer->start(500);

    setWindowIcon(QIcon(":/image/resources/document-globe.png"));
    setWindowTitle(title());
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timer()));
    
    auto const notificationBehavior = m_settingsSerializer.getOption(NotificationBehavior);
    auto const notificationBehaviorIndex = m_dialog.boxNotificationBehavior->findText(notificationBehavior);
    if (notificationBehaviorIndex > -1)
    {
        m_dialog.boxNotificationBehavior->setCurrentIndex(notificationBehaviorIndex);
    }
    else
    {
        m_dialog.boxNotificationBehavior->setCurrentIndex(0);
    }

    auto const responseBehavior = m_settingsSerializer.getOption(ResponseBehavior);
    auto const responseBehaviorIndex = m_dialog.boxResponseBehavior->findText(responseBehavior);
    if (responseBehaviorIndex > -1)
    {
        m_dialog.boxResponseBehavior->setCurrentIndex(responseBehaviorIndex);
    }
    else
    {
        m_dialog.boxResponseBehavior->setCurrentIndex(0);
    }

    auto const alwaysReportOnlineState = m_settingsSerializer.getOption(AlwaysReportOnlineState).toLower() == "true";
    m_dialog.alwaysReportOnlineStateCheckBox->setChecked(alwaysReportOnlineState);


    auto const loadConfig = m_settingsSerializer.getOption(AutoLoadConfig).toLower() == "true";
    if (loadConfig)
    {
        auto configuration = m_settingsSerializer.getOption(ConfigurationName);
        if (configuration.isEmpty() == false)
        {
            loadFile(configuration);
        }
    }

    auto const useEnumMap = m_settingsSerializer.getOption(UseEnumMap).toLower() == "true";

    m_dialog.useEnumMapCheckBox->setChecked(useEnumMap);

    auto const generateRandomValues = m_settingsSerializer.getOption(GenerateRandomValues).toLower() == "true";
    if (generateRandomValues)
    {
        m_generateRandomValues = true;
        m_dialog.checkGenerateRandomValues->setChecked(true);
    }

    auto const streamTimer = m_settingsSerializer.getOption(StreamTimerInterval, -1);
    if (streamTimer > -1)
    {
        m_dialog.streamIntervalBox->setValue(streamTimer);
    }

    auto const sendKeepAlive = m_settingsSerializer.getOption(SendKeepAliveRequest).toLower() == "true";
    if (sendKeepAlive)
    {
        m_dialog.checkRequestKeepAlive->setChecked(sendKeepAlive);
    }

    m_dialog.checkAutoLoadConfig->setChecked(loadConfig);
}

TinyEmberPlus::~TinyEmberPlus()
{
    treeItemChanged(nullptr, nullptr);
}

void TinyEmberPlus::rebuildTree(gadget::Node* root)
{
    treeItemChanged(nullptr, nullptr);
    delete this->root();

    m_dialog.gadgetTreeView->clear();

    if (root != nullptr)
    {
        root->registerListener(m_proxy);

        auto item = new QTreeWidgetItem(m_dialog.gadgetTreeView);
        item->setIcon(0, QIcon(":/image/resources/globe-medium.png"));
        item->setText(0, ::util::StringConverter::toUtf8QString(root->identifier()));
        item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(root)));

        rebuildTree(root, item);
    }
}

void TinyEmberPlus::rebuildTree(gadget::Node* parent, QTreeWidgetItem* owner)
{
    auto& parameters = parent->parameters();
    auto& children = parent->nodes();

    for(auto parameter : parameters)
    {
        auto const icon = QIcon(":/image/resources/leaf.png");
        auto const item = new QTreeWidgetItem(owner);
        item->setIcon(0, icon);
        item->setText(0, ::util::StringConverter::toUtf8QString(parameter->identifier()));
        item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(parameter)));
    }

    for(auto node : children)
    {
        auto const icon = QIcon(":/image/resources/globe-small.png");
        auto const item = new QTreeWidgetItem(owner);
        item->setIcon(0, icon);
        item->setText(0, ::util::StringConverter::toUtf8QString(node->identifier()));
        item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(node)));
        rebuildTree(node, item);
    }
}

QString TinyEmberPlus::title() const
{
    std::stringstream stream;
    auto const version = libember::glow::GlowDtd::version();

    stream << "Tiny Ember+ [Dtd version " << ((version >> 8) & 0xFF) << "." << (version & 0xFF) << "]";
    return QString::fromStdString(stream.str());
}

gadget::Node* TinyEmberPlus::root()
{
    auto const view = m_dialog.gadgetTreeView;
    auto const& children = view->children();
    if (children.empty() == false)
    {
        auto const item = view->topLevelItem(0);
        if (item != nullptr)
        {
            auto const data = item->data(1, 0).value<TreeWidgetItemData>();
            if (data.type().value() == TreeWidgetItemDataType::Node)
            {
                auto node = data.payload<gadget::Node>();
                return node;
            }
        }
    }

    return nullptr;
}

void TinyEmberPlus::loadFile(QString const& filename)
{
    Archive archive;
    auto root = archive.deserialize(filename.toStdString());

    if (root != nullptr)
    {
        rebuildTree(root);

        QFileInfo info(filename);
        m_dialog.configurationName->setText(info.fileName());
        m_settingsSerializer.setOption(ConfigurationName, info.fileName());
        m_settingsSerializer.save();
    }
}

void TinyEmberPlus::loadFile()
{
    auto const filename = QFileDialog::getOpenFileName(
        this,
        "Load EmBER File...",
        QString(),
        "*.EmBER");

    if (filename.isEmpty() == false)
    {
        loadFile(filename);
    }
}

void TinyEmberPlus::saveFile()
{
    auto root = this->root();
    auto filename = m_dialog.configurationName->text();
    if (filename.toLower().endsWith(".ember") == false)
        filename += ".EmBER";

    Archive archive;
    archive.serialize(root, filename.toStdString());
}

void TinyEmberPlus::saveFileAs()
{
    auto root = this->root();
    auto filename = QFileDialog::getSaveFileName(
        this,
        "Save EmBER Configuration...",
        QString(),
        "*.EmBER");

    if (filename.isEmpty() == false && root != nullptr)
    {
        Archive archive;
        archive.serialize(root, filename.toStdString());

        QFileInfo const info(filename);
        m_dialog.configurationName->setText(info.fileName());
        m_settingsSerializer.setOption(ConfigurationName, info.fileName());
        m_settingsSerializer.save();
    }
}

void TinyEmberPlus::exit()
{
    close();
}

void TinyEmberPlus::treeItemChanged(QTreeWidgetItem* item, QTreeWidgetItem*)
{
    auto first = std::begin(m_dialog.entityView->children());
    auto const last = std::end(m_dialog.entityView->children());
    for (; first != last; ++first)
        delete *first;

    if (item != nullptr)
    {
        auto parent = m_dialog.entityView;
        auto view = static_cast<QFrame*>(nullptr);
        auto const data = item->data(1, 0).value<TreeWidgetItemData>();
        auto const type = data.type();
        if (type.value() == TreeWidgetItemDataType::Node)
        {
            auto node = data.payload<gadget::Node>();
            view = ViewFactory::createView(parent, node);

            if (node->parent() == nullptr)
            {
                node->registerListener(m_proxy);
            }
        }
        else if (type.value() == TreeWidgetItemDataType::Parameter)
        {
            auto parameter = data.payload<gadget::Parameter>();
            view = ViewFactory::createView(parent, parameter);
        }

        if (view != nullptr)
            view->setVisible(true);
    }
}

void TinyEmberPlus::showContextMenu(QPoint cursor)
{
    auto menu = GadgetViewContextMenu(m_proxy, m_dialog.gadgetTreeView, cursor);
    menu.exec();
}

void TinyEmberPlus::notificationBehaviorChanged(int index)
{
    if (index > -1)
    {
        auto& settings = ConsumerProxy::settings();
        if (index == 0)
            settings.setNotificationBehavior(NotificationBehavior::UseExpandedContainer);
        else if (index == 1)
            settings.setNotificationBehavior(NotificationBehavior::UseQualifiedContainer);

        m_settingsSerializer.setOption(NotificationBehavior, m_dialog.boxNotificationBehavior->currentText());
        m_settingsSerializer.save();
    }
}

void TinyEmberPlus::responseBehaviorChanged(int index)
{
    if (index > -1)
    {
        auto& settings = ConsumerProxy::settings();
        if (index == 0)
            settings.setResponseBehavior(ResponseBehavior::Default);
        else if (index == 1)
            settings.setResponseBehavior(ResponseBehavior::ForceExpandedContainer);
        else if (index == 2)
            settings.setResponseBehavior(ResponseBehavior::ForceQualifiedContainer);

        m_settingsSerializer.setOption(ResponseBehavior, m_dialog.boxResponseBehavior->currentText());
        m_settingsSerializer.save();
    }
}

void TinyEmberPlus::autoLoadConfigStateChanged(bool state)
{
    m_settingsSerializer.setOption(AutoLoadConfig, state ? "true" : "false");
    m_settingsSerializer.save();
}

void TinyEmberPlus::generateRandomValuesStateChanged(bool state)
{
    m_settingsSerializer.setOption(GenerateRandomValues, state ? "true" : "false");
    m_settingsSerializer.save();
    m_generateRandomValues = state;
}

void TinyEmberPlus::timer()
{
    auto& manager = gadget::StreamManager::instance();
    if (manager.size() > 0)
    {
        if (m_generateRandomValues)
            manager.updateValues();

        auto proxy = m_proxy;
        auto root = libember::glow::GlowStreamCollection::create();
        ::glow::util::StreamConverter::create(root, manager);

        if (proxy != nullptr && root->size() > 0)
            proxy->write(root);

        delete root;
    }

    auto const now = QDateTime::currentDateTimeUtc();
    if (m_sendKeepAlive)
    {
        if (m_lastKeepAliveTransmitTime.msecsTo(now) > 4000)
        {
            m_lastKeepAliveTransmitTime = now;
            m_proxy->writeRequestKeepAlive();
        }
    }
}

void TinyEmberPlus::updateStreamTimer()
{
    auto const interval = m_dialog.streamIntervalBox->value();

    m_timer->setInterval(interval);
    m_settingsSerializer.setOption(StreamTimerInterval, QVariant::fromValue(interval).toString());
    m_settingsSerializer.save();
}

void TinyEmberPlus::updateSendKeepAliveState(bool state)
{
    m_settingsSerializer.setOption(SendKeepAliveRequest, state ? "true" : "false");
    m_settingsSerializer.save();
    m_sendKeepAlive = state;
}

void TinyEmberPlus::updateAlwaysReportOnlineState(bool state)
{
    m_proxy->settings().setAlwaysReportOnlineState(state);
    m_settingsSerializer.setOption(AlwaysReportOnlineState, state ? "true" : "false");
    m_settingsSerializer.save();
}

void TinyEmberPlus::updateUseEnumMap(bool state)
{
    m_proxy->settings().setUseEnumMap(state);
    m_settingsSerializer.setOption(UseEnumMap, state ? "true" : "false");
    m_settingsSerializer.save();
}

void TinyEmberPlus::notifyAsync(libember::dom::Node* node, gadget::Subscriber* subscriber)
{
    if (subscriber != nullptr)
        subscriber->addRef();

    QMetaObject::invokeMethod(this, "synchronizedNotify", Q_ARG(void*, node), Q_ARG(void*, subscriber));
}

void TinyEmberPlus::registerSubscriberAsync(gadget::Subscriber* subscriber)
{
    if (subscriber != nullptr)
    {
        subscriber->addRef();
        QMetaObject::invokeMethod(this, "registerSubscriber", Q_ARG(gadget::Subscriber*, subscriber));
    }
}

void TinyEmberPlus::unregisterSubscriberAsync(gadget::Subscriber* subscriber)
{
    if (subscriber != nullptr)
    {
        subscriber->addRef();
        QMetaObject::invokeMethod(this, "unregisterSubscriber", Q_ARG(gadget::Subscriber*, subscriber));
    }
}

void TinyEmberPlus::registerSubscriber(gadget::Subscriber* subscriber)
{
    if (subscriber != nullptr)
        subscriber->releaseRef();
}

void TinyEmberPlus::unregisterSubscriber(gadget::Subscriber* subscriber)
{
    if (subscriber != nullptr)
    {
        subscriber->unsubscribe();
        subscriber->releaseRef();
    }
}

void TinyEmberPlus::synchronizedNotify(void* nodeptr, void* subscriberptr)
{
    auto subscriber = reinterpret_cast<gadget::Subscriber*>(subscriberptr);
    auto node = reinterpret_cast<dom::Node*>(nodeptr);
    if (node != nullptr)
    {
        auto const type = ber::Type::fromTag(node->typeTag());
        auto root = this->root();
        if (type.value() == libember::glow::GlowType::RootElementCollection
        &&  root != nullptr)
        {
            auto transmit = false;
            auto response = libember::glow::GlowRootElementCollection::create();
            auto collection = dynamic_cast<libember::glow::GlowRootElementCollection*>(node);
            auto proxy = m_proxy;
            ConsumerRequestProcessor::execute(collection, root, response, transmit, subscriber);

            if (transmit && proxy != nullptr)
                proxy->write(response);

            delete response;
        }

        delete node;
    }

    if (subscriber != nullptr)
        subscriber->releaseRef();
}
