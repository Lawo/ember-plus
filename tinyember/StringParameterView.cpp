#include "StringParameterView.h"
#include "ui_StringParameterView.h"
#include "gadgets/Parameter.h"

StringParameterView::StringParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::StringState* state)
    : QFrame(parent)
    , ui(new Ui::StringParameterView)
    , m_parameter(parameter)
    , m_state(state)
{
    ui->setupUi(this);

    m_parameter->addListener(this);

    updateUi();
}

StringParameterView::~StringParameterView()
{
    m_parameter->removeListener(this);

    delete ui;
}

//virtual
void StringParameterView::dirtyStateChanged(gadgets::Entity const&, gadgets::PropertyState const&)
{
    updateUi();
}

void StringParameterView::updateUi()
{
    QString const identifier = QString::fromStdString(m_parameter->identifier());
    QString const description = QString::fromStdString(m_parameter->description());
    QString const number = QVariant(m_parameter->number()).toString();
    QString const value = QString::fromStdString(m_state->value());

    if (ui->identifierText->text() != identifier)
        ui->identifierText->setText(identifier);

    if (ui->descriptionText->text() != description)
        ui->descriptionText->setText(description);

    if (ui->numberText->text() != number)
        ui->numberText->setText(number);

    if (ui->valueText->text() != value)
        ui->valueText->setText(value);

    if (ui->isWriteableBox->isChecked() != m_state->isWriteable())
        ui->isWriteableBox->setChecked(m_state->isWriteable());

    if (ui->isCommandCheckBox->isChecked() != m_state->isCommand())
        ui->isCommandCheckBox->setChecked(m_state->isCommand());
}

void StringParameterView::updateEntityValue()
{
    m_state->setValue(ui->valueText->text().toStdString());
}

void StringParameterView::updateEntityWriteable()
{
    m_state->setWriteable(ui->isWriteableBox->isChecked());
}

void StringParameterView::updateEntityCommand()
{
    m_state->setCommand(ui->isCommandCheckBox->isChecked());
}

void StringParameterView::updateEntityDescription()
{
    m_parameter->setDescription(ui->descriptionText->text().toStdString());
}
