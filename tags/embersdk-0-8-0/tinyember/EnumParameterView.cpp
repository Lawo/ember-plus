#include <algorithm>
#include <iostream>
#include <vector>
#include "EditEnumerationDialog.h"
#include "EnumParameterView.h"
#include "ui_EnumParameterView.h"
#include "gadgets/Parameter.h"

using namespace gadgets;
using namespace gadgets::states;

namespace
{
    /**
     * Functor to add a std::string to a QComboBox
     */
    struct QComboBoxAddItem
    {
        /**
         * Constructor
         * @param box Combobox to add the items to
         */
        QComboBoxAddItem(QComboBox* box)
            : box(box)
        {}

        /**
         * Converts the provided std::string to a QString
         * and adds it to the combo box.
         * @param text Text to add to the combo box.
         */
        void operator()(std::string const& text)
        {
            box->addItem(QString::fromStdString(text));
        }

        /**
         * Combo box to add items to
         */
        QComboBox *const box;
    };
}

EnumParameterView::EnumParameterView(QWidget* parent, gadgets::Parameter* parameter, gadgets::states::EnumState* state)
    : QFrame(parent)
    , ui(new Ui::EnumParameterView)
    , m_parameter(parameter)
    , m_state(state)
{
    // The signals of the value combo box must be temporarily disabled,
    // otherwise filling the combo box results in an overwrite of the
    // current value index.

    ui->setupUi(this);
    ui->valueComboBox->blockSignals(true);

    updateUi();

    ui->valueComboBox->blockSignals(false);
    parameter->addListener(this);
}

EnumParameterView::~EnumParameterView()
{
    m_parameter->removeListener(this);

    delete ui;
}

//virtual
void EnumParameterView::dirtyStateChanged(gadgets::Entity const&, gadgets::PropertyState const&)
{
    updateUi();
}

void EnumParameterView::updateUi()
{
    QString const description = QString::fromStdString(m_parameter->description());
    QString const identifier = QString::fromStdString(m_parameter->identifier());
    QString const number = QVariant(m_parameter->number()).toString();

    if (ui->descriptionText->text() != description)
        ui->descriptionText->setText(description);

    if (ui->identifierText->text() != identifier)
        ui->identifierText->setText(identifier);

    if (ui->numberText->text() != number)
        ui->numberText->setText(number);

    if (ui->isCommandCheckBox->isChecked() != m_state->isCommand())
        ui->isCommandCheckBox->setChecked(m_state->isCommand());

    if (ui->isWriteableBox->isChecked() != m_state->isWriteable())
        ui->isWriteableBox->setChecked(m_state->isWriteable());


    std::size_t const itemCount = ui->valueComboBox->count();
    bool const sequenceLengthEquals = m_state->size() == itemCount;
    bool sequenceChanged = !sequenceLengthEquals;

    if (sequenceLengthEquals)
    {
        EnumState::const_iterator it = m_state->begin();
        std::size_t index = 0;

        for(/** Nothing **/; index < itemCount; ++index, ++it)
        {
            std::string const itemText = ui->valueComboBox->itemText(index).toStdString();

             if(*it != itemText)
                 break;
        }

        sequenceChanged = !(index == itemCount);
    }

    if (sequenceChanged)
    {
        while(ui->valueComboBox->count() > 0)
        {
            ui->valueComboBox->removeItem(0);
        }

        EnumState::const_iterator const first = m_state->begin();
        EnumState::const_iterator const last = m_state->end();
        std::for_each(first, last, QComboBoxAddItem(ui->valueComboBox));
    }

    int const index = static_cast<int>(m_state->index());

    if (ui->valueComboBox->currentIndex() != index)
        ui->valueComboBox->setCurrentIndex(index);
}

void EnumParameterView::updateEntityDescription()
{
    m_parameter->setDescription(ui->descriptionText->text().toStdString());
}

void EnumParameterView::updateEntityValue(int value)
{
    if (value > -1)
    {
        m_state->setIndex(value);
    }
}

void EnumParameterView::updateEntityWriteable()
{
    m_state->setWriteable(ui->isWriteableBox->isChecked());
}

void EnumParameterView::updateEntityCommand()
{
    m_state->setCommand(ui->isCommandCheckBox->isChecked());
}

void EnumParameterView::on_editItemsButton_clicked()
{
    EnumState::const_iterator const first = m_state->begin();
    EnumState::const_iterator const last = m_state->end();
    EditEnumerationDialog dialog(this, first, last);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_state->assign(dialog.begin(), dialog.end());
    }
}
