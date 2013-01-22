#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QTreeView>
#include "Libember.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "gadgets/Node.h"
#include "gadgets/Parameter.h"
#include "gadgets/serialization/EntityArchive.h"
#include "glow/GlowClientMessageHandler.h"
#include "glow/GlowStreamWriter.h"
#include "GadgetTreeViewContextMenu.h"
#include "TreeWidgetItemData.h"

using namespace ::gadgets;
using namespace ::gadgets::serialization;
using namespace ::glow;
using namespace ::libember;
using namespace ::libember::glow;

namespace
{
    /**
     * Overload for std::string
     */
    QDataStream& operator<<(QDataStream& s, String const& str)
    {
        s << QString(str.c_str());
        return s;
    }

    /**
     * Overload for std::string
     */
    QDataStream& operator>>(QDataStream& s, String& str)
    {
        QString intermediate;
        s >> intermediate;

        str = intermediate.toStdString();
        return s;
    }
}


MainWindow::MainWindow(EntityStateListener* rootListener, GlowStreamWriter *streamWriter, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_rootListener(rootListener)
    , m_streamWriter(streamWriter)
{
    ui->setupUi(this);
    ui->gadgetTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    setWindowIcon(QIcon(":/image/resources/document-globe.png"));

    //openConfiguration("default.glow");
}

MainWindow::~MainWindow()
{
    gadgetViewSelectionChanged(0, 0);
    fillGadgetTreeView(0);

    delete ui;
}

Entity* MainWindow::rootEntity()
{
    QTreeWidget *const view =  ui->gadgetTreeView;
    QObjectList const& children = view->children();

    if (!children.empty())
    {
        QTreeWidgetItem const* item = view->topLevelItem(0);

        if (item != 0)
        {
            TreeWidgetItemData const data = item->data(1, 0).value<TreeWidgetItemData>();
            return data.entity();
        }
    }

    return 0;
}

void MainWindow::fillGadgetTreeView(Node* root)
{
    gadgetViewSelectionChanged(0, 0);

    // Delete the current root node and clear the view
    delete rootEntity();

    ui->gadgetTreeView->clear();

    if (root != 0)
    {
        root->addListener(m_rootListener);

        QTreeWidgetItem *const item = new QTreeWidgetItem(ui->gadgetTreeView);
        item->setIcon(0, QIcon(":/image/resources/globe-medium.png"));
        item->setText(0, QString::fromStdString(root->identifier()));
        item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(root)));

        Entity::iterator first = root->begin();
        Entity::iterator const last = root->end();

        for(/* Nothing */; first != last; ++first)
        {
            fillGadgetTreeViewRecursive(item, *first);
        }
    }
}

void MainWindow::fillGadgetTreeViewRecursive(QTreeWidgetItem* parent, Entity* entity)
{
    bool const isParameter = dynamic_cast<Parameter*>(entity) != 0;
    QString const iconName = isParameter
            ? QString(":/image/resources/leaf.png")
            : QString(":/image/resources/globe-small.png");
    QTreeWidgetItem *const item = new QTreeWidgetItem(parent);
    item->setIcon(0, QIcon(iconName));
    item->setText(0, QString::fromStdString(entity->identifier()));
    item->setData(1, 0, QVariant::fromValue(TreeWidgetItemData(entity)));

    Entity::iterator first = entity->begin();
    Entity::iterator const last = entity->end();

    for(/* Nothing */; first != last; ++first)
    {
        fillGadgetTreeViewRecursive(item, *first);
    }
}

void MainWindow::gadgetViewSelectionChanged(QTreeWidgetItem* x, QTreeWidgetItem*)
{
    QTreeWidgetItem *const item = x;

    // Delete the current view(s)
    QObjectList::ConstIterator first = ui->entityView->children().begin();
    QObjectList::ConstIterator const last = ui->entityView->children().end();

    for( /* Nothing */ ; first != last; ++first)
        delete *first;

    if (item != 0)
    {
        TreeWidgetItemData const data = item->data(1, 0).value<TreeWidgetItemData>();
        QFrame *const frame = data.createView(ui->entityView);

        if (frame != 0)
            frame->setVisible(true);
    }
}

void MainWindow::exit()
{
    close();
}

void MainWindow::openConfiguration(QString const& filename)
{
    QFile file(filename);

    if (QFile::exists(filename) && file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        QFileInfo fileInfo(filename);
        EntityArchive<QDataStream> archive;
        Node *const root = archive.load(stream);

        fillGadgetTreeView(root);
        file.close();

        ui->configurationName->setText(fileInfo.fileName());
    }
}

void MainWindow::openFile()
{
    QString const filename = QFileDialog::getOpenFileName(this, "Load Gadgets...", QString(), "*.glow");

    openConfiguration(filename);
}

void MainWindow::saveFile()
{
    QString filename = ui->configurationName->text();

    if (filename.endsWith(".glow") == false)
        filename += ".glow";

    QFile file(filename);
    Entity *const root = rootEntity();

    if (file.open(QIODevice::WriteOnly) && root != 0)
    {
        QDataStream stream(&file);
        EntityArchive<QDataStream> const archive;

        archive.save(stream, root);
        file.flush();
        file.close();
    }
}

void MainWindow::saveFileAs()
{
    QString const filename = QFileDialog::getSaveFileName(this, "Save file as...", QString(), "*.glow");
    Entity *const root = rootEntity();

    if (filename.isEmpty() == false && root != 0)
    {
        QFile file(filename);

        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            QFileInfo const fileInfo(filename);
            EntityArchive<QDataStream> const archive;

            archive.save(stream, root);
            file.flush();
            file.close();

            ui->configurationName->setText(fileInfo.fileName());
        }
    }
}


void MainWindow::showContextMenu(QPoint pos)
{
    GadgetTreeViewContextMenu menu(ui->gadgetTreeView, pos);
    menu.exec();
}


void MainWindow::synchronizedGlowRequest(void* nodeptr)
{
    dom::Node *const node = reinterpret_cast<dom::Node*>(nodeptr);

    if (node != 0)
    {
        ber::Type const type = ber::Type::fromTag(node->typeTag());
        Entity *const root = rootEntity();

        if (type.value() == GlowType::ElementCollection && root != 0)
        {
            GlowElementCollection response(GlowTags::Root());
            GlowClientMessageHandler messageHandler;

            messageHandler.process(dynamic_cast<GlowElementCollection*>(node), root, &response);

            if (m_streamWriter != 0)
                m_streamWriter->write(&response);
        }
    }
}

