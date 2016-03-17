#include <algorithm>
#include "util/StringConverter.h"
#include "EditEnumerationDialog.h"


EditEnumerationDialog::EditEnumerationDialog(QWidget* parent)
    : QDialog(parent)
    , m_dialog(new Ui::EditEnumerationDialog())
{
    m_dialog->setupUi(this);
}


EditEnumerationDialog::~EditEnumerationDialog(void)
{
    delete m_dialog;
}

EditEnumerationDialog::const_iterator EditEnumerationDialog::begin() const
{
    synchronize();
    return std::begin(m_items);
}

EditEnumerationDialog::const_iterator EditEnumerationDialog::end() const
{
    synchronize();
    return std::end(m_items);
}

EditEnumerationDialog::size_type EditEnumerationDialog::size() const
{
    return m_items.size();
}

void EditEnumerationDialog::discard()
{
    m_items.clear();
}

void EditEnumerationDialog::synchronize() const
{
    if (m_items.empty())
    {
        auto const list = m_dialog->itemList->findItems(".*", Qt::MatchRegExp);
        auto const first = std::begin(list);
        auto const last = std::end(list);
        std::transform(first, last, std::back_inserter(m_items), [](QListWidgetItem const* item) -> std::string
        {
            auto const text = item->text();
            return util::StringConverter::toUtf8StdString(text);
        });
    }
}

void EditEnumerationDialog::insertItem()
{
    discard();
    auto const text = QString("Entry ") + QVariant(m_dialog->itemList->count()).toString();
    auto const item = new QListWidgetItem(text, m_dialog->itemList);

    m_dialog->itemList->setCurrentItem(item);
    m_dialog->currentItem->setText(text);
    m_dialog->currentItem->selectAll();
    m_dialog->currentItem->setFocus();
}

void EditEnumerationDialog::removeItem()
{
    discard();
    auto const item = m_dialog->itemList->currentItem();
    delete item;
}

void EditEnumerationDialog::updateCurrentItem()
{
    discard();
    auto const item = m_dialog->itemList->currentItem();
    if (item != nullptr)
        item->setText(m_dialog->currentItem->text());
}

void EditEnumerationDialog::updateSelection()
{
    auto const item = m_dialog->itemList->currentItem();
    if (item != nullptr)
        m_dialog->currentItem->setText(item->text());
}
