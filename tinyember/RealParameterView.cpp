#include "RealParameterView.h"
#include "ui_RealParameterView.h"
#include "gadgets/Parameter.h"

RealParameterView::RealParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::RealState *state)
    : QFrame(parent)
    , ui(new Ui::RealParameterView)
    , m_parameter(parameter)
    , m_state(state)
{
    ui->setupUi(this);

    m_parameter->addListener(this);

    updateUi();
}

RealParameterView::~RealParameterView()
{
    m_parameter->removeListener(this);

    delete ui;
}

void RealParameterView::updateUi()
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
void RealParameterView::dirtyStateChanged(gadgets::Entity const&, gadgets::PropertyState const&)
{
    updateUi();
}

void RealParameterView::updateEntityDescription()
{
    m_parameter->setDescription(ui->descriptionText->text().toStdString());
}

void RealParameterView::updateEntityMin()
{
    m_state->setMin(ui->minBox->value());
}

void RealParameterView::updateEntityMax()
{
    m_state->setMax(ui->maxBox->value());
}

void RealParameterView::updateEntityValue()
{
    m_state->setValue(ui->valueBox->value());
}

void RealParameterView::updateEntityFormat()
{
    m_state->setFormat(ui->formatText->text().toStdString());
}

void RealParameterView::updateEntityWriteable()
{
    m_state->setWriteable(ui->isWriteableBox->checkState());
}

void RealParameterView::updateEntityCommand()
{
    m_state->setCommand(ui->isCommandCheckBox->checkState());
}
