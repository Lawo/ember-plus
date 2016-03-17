#ifndef __TINYEMBER_EDITENUMERATIONDIALOG_H
#define __TINYEMBER_EDITENUMERATIONDIALOG_H

#include <qdialog.h>
#include <list>
#include "ui_EditEnumerationDialog.h"
#include "util/StringConverter.h"

/**
 * A dialog to edit the entries of an enumeration parameter.
 */
class EditEnumerationDialog : public QDialog
{
    Q_OBJECT
    typedef std::vector<std::string> ItemCollection;
    public:
        typedef ItemCollection::const_iterator const_iterator;
        typedef ItemCollection::size_type size_type;

        /**
         * Constructor.
         * @param parent The parent widget of this dialog.
         */
        explicit EditEnumerationDialog(QWidget* parent);

        /**
         * Constructor.
         * @param parent The parent widget of this dialog.
         * @param first An iterator that points to the first enumeration entry to copy.
         * @param last An iterator that points one past the last enumeration entry to copy.
         */
        template<typename InputIterator>
        EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last);

        /** Destructor */
        virtual ~EditEnumerationDialog();

        /**
         * Returns an iterator that points to the first enumeration entry.
         * @return An iterator that points to the first enumeration entry.
         */
        const_iterator begin() const;

        /**
         * Returns an iterator that points to the first element beyond the buffer of enumeration entries.
         * @return An iterator that points to the first element beyond the buffer of enumeration entries.
         */
        const_iterator end() const;

        /**
         * Returns the number of enumeration entries.
         * @return The number of enumeration entries.
         */
        size_type size() const;

    private:
        /**
         * Clears the vector that contains the current enumeration.
         */
        void discard();

        /**
         * Synchronizes the local vector containing the enumeration with the items
         * available in the listview.
         */
        void synchronize() const;

    private slots:
        /**
         * This slot is invoked when a new entry shall be inserted.
         */
        void insertItem();

        /**
         * This slot is invoked when the selected entry shall be removed.
         */
        void removeItem();

        /**
         * Changes the value of the currently selected item to the text
         * written in the edit box.
         */
        void updateCurrentItem();

        /**
         * Changes the text of the edit box to the value of the currently
         * selected enumeration.
         */
        void updateSelection();

    private:
        mutable ItemCollection m_items;
        Ui::EditEnumerationDialog* m_dialog;
};

/******************************************************************************
 * Mandatory inline implementation                                            *
 ******************************************************************************/

template<typename InputIterator>
inline EditEnumerationDialog::EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last)
    : QDialog(parent)
    , m_dialog(new Ui::EditEnumerationDialog())
    , m_items(first, last)
{
    m_dialog->setupUi(this);

    for(auto& item : m_items)
    {
        auto const text = ::util::StringConverter::toUtf8QString(item);
        m_dialog->itemList->addItem(text);
    }
}

#endif//__TINYEMBER_EDITENUMERATIONDIALOG_H
