#include "IntParameterView.h"
#include "ui_IntParameterView.h"
#include "gadgets/Parameter.h"

IntParameterView::IntParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::IntState* state)
    : QFrame(parent)
    , ui(new Ui::IntParameterView)
    , m_parameter(parameter)
    , m_state(state)
{
    ui->setupUi(this);

    m_parameter->addListener(this);

    updateUi();
}

IntParameterView::~IntParameterView()
{
    m_parameter->removeListener(this);

    delete ui;
}

void IntParameterView::updateUi()
{
    QString const identifier = QString::fromStdString(m_parameter->identifier());
    QString const description = QString::fromStdString(m_parameter->description());
    QString const number = QVariant(m_parameter->number()).toString();
    QString const format = QString::fromStdString(m_state->format());

    if (ui->identifierText->text() != identifier)
        ui->identifierText->setText(identifier);

    if (ui->descriptionText->text() != description)
        ui->descriptionText->setText(description);

    if (ui->numberText->text() != number)
        ui->numberText->setText(number);

    if (ui->formatText->text() != format)
        ui->formatText->setText(format);

    if (ui->minBox->value() != m_state->min())
        ui->minBox->setValue(m_state->min());

    if (ui->maxBox->value() != m_state->max())
        ui->maxBox->setValue(m_state->max());

    if (ui->valueBox->value() != m_state->value())
        ui->valueBox->setValue(m_state->value());

    if (ui->isWriteableBox->isChecked() != m_state->isWriteable())
        ui->isWriteableBox->setChecked(m_state->isWriteable());

    if (ui->isCommandCheckBox->isChecked() != m_state->isCommand())
        ui->isCommandCheckBox->setChecked(m_state->isCommand());
}

//virtual
void IntParameterView::dirtyStateChanged(gadgets::Entity const&, gadgets::PropertyState const&)
{
    updateUi();
}
void IntParameterView::updateEntityDescription()
{
    m_parameter->setDescription(ui->descriptionText->text().toStdString());
}

void IntParameterView::updateEntityMin()
{
    m_state->setMin(ui->minBox->value());
}

void IntParameterView::updateEntityMax()
{
    m_state->setMax(ui->maxBox->value());
}

void IntParameterView::updateEntityValue()
{
    m_state->setValue(ui->valueBox->value());
}

void IntParameterView::updateEntityFormat()
{
    m_state->setFormat(ui->formatText->text().toStdString());
}

void IntParameterView::updateEntityIsWriteable()
{
    m_state->setWriteable(ui->isWriteableBox->checkState());
}

void IntParameterView::updateEntityIsCommand()
{
    m_state->setCommand(ui->isCommandCheckBox->checkState());
}
