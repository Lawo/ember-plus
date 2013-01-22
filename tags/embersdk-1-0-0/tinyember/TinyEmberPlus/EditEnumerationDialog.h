#ifndef __TINYEMBER_EDITENUMERATIONDIALOG_H
#define __TINYEMBER_EDITENUMERATIONDIALOG_H

#include <qdialog.h>
#include <list>
#include "ui_EditEnumerationDialog.h"
#include "util\StringConverter.h"

class EditEnumerationDialog : public QDialog
{
    Q_OBJECT
    typedef std::vector<std::string> ItemCollection;
    public:
        typedef ItemCollection::const_iterator const_iterator;
        typedef ItemCollection::size_type size_type;

        explicit EditEnumerationDialog(QWidget* parent);

        template<typename InputIterator>
        EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last);

        virtual ~EditEnumerationDialog();

        const_iterator begin() const;

        const_iterator end() const;

        size_type size() const;

    private:
        void discard();

        void assure() const;

    private slots:
        void insertItem();

        void removeItem();

        void updateCurrentItem();

        void updateSelection();

    private:
        mutable ItemCollection m_items;
        Ui::EditEnumerationDialog* m_dialog;
};

template<typename InputIterator>
inline EditEnumerationDialog::EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last)
    : QDialog(parent)
    , m_dialog(new Ui::EditEnumerationDialog())
    , m_items(first, last)
{
    m_dialog->setupUi(this);

    for each(auto& item in m_items)
    {
        auto const text = ::util::StringConverter::toUtf8QString(item);
        m_dialog->itemList->addItem(text);
    }
}

#endif//__TINYEMBER_EDITENUMERATIONDIALOG_H
