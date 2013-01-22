#ifndef EDITENUMERATIONDIALOG_H
#define EDITENUMERATIONDIALOG_H

#include <iterator>
#include <list>
#include <QDialog>
#include "Types.h"
#include "ui_EditEnumerationDialog.h"

namespace Ui {
    class EditEnumerationDialog;
}

/**
 * Simple dialog wich can be used to modify or create an enumeration.
 * The class or dialog opening this dialog must connect to the "accepted" signal in order
 * to be notified when the user finished eiditing.
 */
class EditEnumerationDialog : public QDialog
{
    Q_OBJECT

    typedef std::list<String> ItemList;

public:
    typedef ItemList::const_iterator const_iterator;
    typedef ItemList::size_type size_type;
    typedef ItemList::difference_type difference_type;

    /**
     * Constructor
     * @param parent Parent widget
     */
    explicit
    EditEnumerationDialog(QWidget* parent = 0);

    /**
     * Constructor
     * @param parent Parent widget
     * @param items Initial enumeration items
     */
    template<typename InputIterator>
    EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last);

    /**
     * Destructor
     */
    virtual ~EditEnumerationDialog();

    /**
     * Returns the first enumeration item
     * @return Returns the first enumeration item.
     */
    const_iterator begin() const;

    /**
     * Returns the end of the enumeration item buffer.
     * @return Returns the end of the enumeration item buffer.
     */
    const_iterator end() const;

private slots:
    void on_addItemButton_clicked();
    void on_removeItemButton_clicked();
    void on_currentItem_editingFinished();
    void on_itemList_itemSelectionChanged();

private:
    /**
     * Resets the item list.
     */
    void discardItems();

    /**
     * If the item list is empty, it will be recreated.
     */
    void assureItems() const;

private:
    Ui::EditEnumerationDialog *ui;
    mutable ItemList m_items;
};

/******************************************************************************
 * Mandatory inline implementation                                            *
 ******************************************************************************/

template<typename InputIterator>
inline EditEnumerationDialog::EditEnumerationDialog(QWidget* parent, InputIterator first, InputIterator last)
    : QDialog(parent)
    , ui(new Ui::EditEnumerationDialog)
    , m_items(first, last)
{
    ui->setupUi(this);

    for(/** Nothing **/; first != last; ++first)
    {
        ui->itemList->addItem(QString::fromStdString(*first));
    }
}

#endif // EDITENUMERATIONDIALOG_H
