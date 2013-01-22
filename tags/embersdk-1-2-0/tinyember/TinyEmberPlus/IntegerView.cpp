#include "IntegerView.h"
#include "gadget/IntegerParameter.h"
#include "util/StreamFormatConverter.h"
#include "util/StringConverter.h"
#include <sstream>

IntegerView::IntegerView(QWidget* parent, gadget::IntegerParameter* parameter)
    : QFrame(parent)
    , m_view(new Ui::IntegerView())
    , m_parameter(parameter)
{
    m_view->setupUi(this);
    m_parameter->registerListener(this);

    auto streamFormats = util::StreamFormatConverter();
    for each(auto& entry in streamFormats)
    {
        auto const text = QString::fromStdString(entry.second);
        m_view->streamFormatBox->addItem(text);
    }

    updateUi();
}

IntegerView::~IntegerView()
{
    m_parameter->unregisterListener(this);
    delete m_view;
}

void IntegerView::updateUi()
{
    auto const identifier = util::StringConverter::toUtf8QString(m_parameter->identifier());
    auto const description = util::StringConverter::toUtf8QString(m_parameter->description());
    auto const number = QVariant(m_parameter->number()).toString();
    auto const format = QString::fromStdString(m_parameter->format());
    auto const access = m_parameter->access().value();

    if (m_view->identifierText->text() != identifier)
        m_view->identifierText->setText(identifier);

    if (m_view->descriptionText->text() != description)
        m_view->descriptionText->setText(description);

    if (m_view->numberText->text() != number)
        m_view->numberText->setText(number);

    if (m_view->formatText->text() != format)
        m_view->formatText->setText(format);

    if (m_view->minBox->value() != m_parameter->minimum())
        m_view->minBox->setValue(m_parameter->minimum());

    if (m_view->maxBox->value() != m_parameter->maximum())
        m_view->maxBox->setValue(m_parameter->maximum());

    if (m_view->valueBox->value() != m_parameter->value())
        m_view->valueBox->setValue(m_parameter->value());

    if (m_view->accessBox->currentIndex() != access)
        m_view->accessBox->setCurrentIndex(access);

    auto const& formula = m_parameter->formula();
    m_view->toProviderTermText->setText(QString::fromStdString(formula.consumerToProvider()));
    m_view->toConsumerTermText->setText(QString::fromStdString(formula.providerToConsumer()));

    auto const hasStreamIdentifier = m_parameter->hasStreamIdentifier();
    if (m_view->checkUseStreamIdentifier->isChecked() != hasStreamIdentifier)
        m_view->checkUseStreamIdentifier->setChecked(hasStreamIdentifier);

    auto const hasStreamDescriptor = m_parameter->hasStreamDescriptor();
    if (m_view->checkUseStreamDescriptor->isChecked() != hasStreamDescriptor)
        m_view->checkUseStreamDescriptor->setChecked(hasStreamDescriptor);

    if (hasStreamIdentifier)
    {
        auto const streamIdentifier = m_parameter->streamIdentifier();
        if (m_view->streamIdentifierBox->value() != streamIdentifier && m_view->streamIdentifierBox->hasFocus() == false)
            m_view->streamIdentifierBox->setValue(streamIdentifier);
    }
    else
    {
        if (m_view->streamIdentifierBox->value() != -1)
            m_view->streamIdentifierBox->setValue(-1);
    }

    if (hasStreamDescriptor)
    {
        auto const descriptor = m_parameter->streamDescriptor();
        auto const offset = descriptor->offset();
        auto const format = QString::fromStdString(util::StreamFormatConverter::toString(descriptor->format()));

        if (m_view->streamOffsetBox->value() != offset && m_view->streamOffsetBox->hasFocus() == false)
            m_view->streamOffsetBox->setValue(offset);

        if (m_view->streamFormatBox->currentText() != format)
        {
            auto const index = m_view->streamFormatBox->findText(format);
            m_view->streamFormatBox->setCurrentIndex(index);
        }
    }
    else
    {
        if (m_view->streamFormatBox->currentIndex() != 0)
            m_view->streamFormatBox->setCurrentIndex(0);

        if (m_view->streamOffsetBox->value() != 0)
            m_view->streamOffsetBox->setValue(0);
    }

    if (m_parameter->dirtyState().isSet(::gadget::ParameterField::SubscriptionCount))
    {
        auto const subscriptions = m_parameter->subscribers();
        auto stream = std::stringstream();
        auto const suffix = subscriptions != 1 ? "subscribers" : "subscriber";
        stream << "Streaming (" << subscriptions << " " << suffix << ")";

        m_view->streamingGroupBox->setTitle(QString::fromStdString(stream.str()));
    }

    m_view->displayValueText->setText(QString::fromStdString(m_parameter->toDisplayValue()));
}

void IntegerView::updateDescription()
{
    auto const description = util::StringConverter::toUtf8StdString(m_view->descriptionText->text());
    m_parameter->setDescription(description);
}

void IntegerView::updateValue()
{
    auto const value = m_view->valueBox->value();
    m_parameter->setValue(value);
}

void IntegerView::updateRxTerm()
{
    auto const rxterm = m_view->toConsumerTermText->text().toStdString();
    auto const formula = gadget::Formula(rxterm, m_parameter->formula().consumerToProvider());

    m_parameter->setFormula(formula);
}

void IntegerView::updateTxTerm()
{
    auto const txterm = m_view->toProviderTermText->text().toStdString();
    auto const formula = gadget::Formula(m_parameter->formula().providerToConsumer(), txterm);

    m_parameter->setFormula(formula);
}

void IntegerView::updateMax()
{
    auto const value = m_view->maxBox->value();
    m_parameter->setMax(value);
}

void IntegerView::updateMin()
{
    auto const value = m_view->minBox->value();
    m_parameter->setMin(value);
}

void IntegerView::updateFormat()
{
    auto const format = m_view->formatText->text().toStdString();
    m_parameter->setFormat(format);
}

void IntegerView::updateAccess(int access)
{
    if (access > -1)
    {
        m_parameter->setAccess(static_cast<gadget::Access::_Domain>(access));
    }
}

void IntegerView::updateStreamIdentifierState(bool state)
{
    if (state)
    {
        if (m_parameter->hasStreamIdentifier())
        {
            auto const identifier = m_parameter->streamIdentifier();
            if (m_view->streamIdentifierBox->value() != identifier)
                m_view->streamIdentifierBox->setValue(identifier);
        }
        else
        {
            m_parameter->setStreamIdentifier(0);
        }
    }
    else
    {
        m_parameter->setStreamIdentifier(-1);
    }
}

void IntegerView::updateStreamDescriptorState(bool state)
{
    if (state)
    {
        if (m_parameter->hasStreamDescriptor())
        {
            auto const descriptor = m_parameter->streamDescriptor();
            auto const format = QString::fromStdString(util::StreamFormatConverter::toString(descriptor->format()));
            auto const offset = descriptor->offset();

            if (m_view->streamFormatBox->currentText() != format)
                m_view->streamFormatBox->setCurrentIndex(m_view->streamFormatBox->findText(format));

            if (m_view->streamOffsetBox->value() != offset)
                m_view->streamOffsetBox->setValue(offset);
        }
        else
        {
            auto const offset = m_view->streamOffsetBox->value();
            auto const format = util::StreamFormatConverter::fromString(
                m_view->streamFormatBox->currentText().toStdString());

            m_parameter->setStreamDescriptor(format, offset);
        }
    }
    else
    {
        m_parameter->setStreamDescriptor(nullptr);
    }
}

void IntegerView::updateStreamIdentifier()
{
    auto const isChecked = m_view->checkUseStreamIdentifier->isChecked();
    if (isChecked)
    {
        auto const identifier = m_view->streamIdentifierBox->value();
        m_parameter->setStreamIdentifier(identifier);
    }
}

void IntegerView::updateStreamFormat(int index)
{
    auto const isChecked = m_view->checkUseStreamDescriptor->isChecked();
    if (isChecked)
    {
        auto const formatString = m_view->streamFormatBox->currentText().toStdString();
        auto const format = util::StreamFormatConverter::fromString(formatString);
        if (m_parameter->hasStreamDescriptor())
        {
            auto const offset = m_parameter->streamDescriptor()->offset();
            m_parameter->setStreamDescriptor(format, offset);
        }
        else
        {
            auto const offset = m_view->streamOffsetBox->value();
            m_parameter->setStreamDescriptor(format, offset);
        }
    }
}

void IntegerView::updateStreamOffset()
{
    auto const isChecked = m_view->checkUseStreamDescriptor->isChecked();
    if (isChecked)
    {
        auto const offset = m_view->streamOffsetBox->value();
        if (m_parameter->hasStreamDescriptor())
        {
            auto const format = m_parameter->streamDescriptor()->format();
            m_parameter->setStreamDescriptor(format, offset);
        }
        else
        {
            auto const formatString = m_view->streamFormatBox->currentText().toStdString();
            auto const format = util::StreamFormatConverter::fromString(formatString);
            m_parameter->setStreamDescriptor(format, offset);
        }
    }
}

void IntegerView::notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*)
{
    updateUi();
}
