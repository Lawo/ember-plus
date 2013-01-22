#include <algorithm>
#include <iostream>
#include "EditEnumerationDialog.h"

namespace
{
    /**
     * Struct providing the unary () operator to extract the text of a QListWidgetItem
     * and convert it into a std::string
     */
    struct TransformListItemToString
    {
        /**
         * Extracts the object's text and returns it as std::string
         * @param object QListWidgetItem, needs to be casted.
         * @return Returns the item's text or an empty string if the conversion failed.
         */
        std::string operator()(QListWidgetItem const* item) const
        {
            std::string const result = item->text().toStdString();
            return result;
        }
    };
}

EditEnumerationDialog::EditEnumerationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditEnumerationDialog)
{
    ui->setupUi(this);
}


EditEnumerationDialog::~EditEnumerationDialog()
{
    delete ui;
}

void EditEnumerationDialog::discardItems()
{
    m_items.clear();
}

void EditEnumerationDialog::assureItems() const
{
    if (m_items.empty())
    {
        QList<QListWidgetItem*> const items = ui->itemList->findItems(".*", Qt::MatchRegExp);
        QList<QListWidgetItem*>::const_iterator const first = items.begin();
        QList<QListWidgetItem*>::const_iterator const last = items.end();

        std::transform(first, last, std::back_inserter(m_items), TransformListItemToString());
    }
}

EditEnumerationDialog::const_iterator EditEnumerationDialog::begin() const
{
    assureItems();
    return m_items.begin();
}

EditEnumerationDialog::const_iterator EditEnumerationDialog::end() const
{
    assureItems();
    return m_items.end();
}

void EditEnumerationDialog::on_addItemButton_clicked()
{
    discardItems();
    QString const text = QString("Entry ") + QVariant(ui->itemList->count()).toString();
    QListWidgetItem *const item = new QListWidgetItem(text, ui->itemList);

    ui->itemList->setCurrentItem(item);
    ui->currentItem->setText(text);
    ui->currentItem->selectAll();
    ui->currentItem->setFocus();
}

void EditEnumerationDialog::on_removeItemButton_clicked()
{
    discardItems();
    QListWidgetItem *const item = ui->itemList->currentItem();
    delete item;
}

void EditEnumerationDialog::on_currentItem_editingFinished()
{
    discardItems();
    QListWidgetItem *const item = ui->itemList->currentItem();

    if (item != 0)
        item->setText(ui->currentItem->text());
}

void EditEnumerationDialog::on_itemList_itemSelectionChanged()
{
    QListWidgetItem *const item = ui->itemList->currentItem();

    if (item != 0)
        ui->currentItem->setText(item->text());
}
