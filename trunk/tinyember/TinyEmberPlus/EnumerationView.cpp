#include "EnumerationView.h"
#include "EditEnumerationDialog.h"
#include "gadget/EnumParameter.h"
#include "util/StreamFormatConverter.h"
#include "util/StringConverter.h"
#include <sstream>

EnumerationView::EnumerationView(QWidget* parent, gadget::EnumParameter* parameter)
    : QFrame(parent)
    , m_view(new Ui::EnumerationView())
    , m_parameter(parameter)
{
    m_view->setupUi(this);
    m_view->valueComboBox->blockSignals(true);

    auto streamFormats = util::StreamFormatConverter();
    for each(auto& entry in streamFormats)
    {
        auto const text = QString::fromStdString(entry.second);
        m_view->streamFormatBox->addItem(text);
    }

    updateUi();
    m_view->valueComboBox->blockSignals(false);
    m_parameter->registerListener(this);
}

EnumerationView::~EnumerationView(void)
{
    m_parameter->unregisterListener(this);
    delete m_view;
}

void EnumerationView::updateUi()
{
    auto const identifier = util::StringConverter::toUtf8QString(m_parameter->identifier());
    auto const description = util::StringConverter::toUtf8QString(m_parameter->description());
    auto const number = QVariant(m_parameter->number()).toString();
    auto const access = m_parameter->access().value();
    auto const schema = util::StringConverter::toUtf8QString(m_parameter->schema());

    if (m_view->identifierText->text() != identifier)
        m_view->identifierText->setText(identifier);

    if (m_view->descriptionText->text() != description)
        m_view->descriptionText->setText(description);

    if (m_view->schemaText->text() != schema)
        m_view->schemaText->setText(schema);

    if (m_view->numberText->text() != number)
        m_view->numberText->setText(number);

    auto const count = m_view->valueComboBox->count();
    auto const sequenceCountEquals = m_parameter->size() == count;
    auto sequenceChanged = !sequenceCountEquals;

    if (sequenceCountEquals)
    {
        auto it = m_parameter->begin();
        auto index = 0;
        for( ; index < count; ++index, ++it)
        {
            auto const text = ::util::StringConverter::toUtf8StdString(m_view->valueComboBox->itemText(index));
            if(*it != text)
                break;
        }

        sequenceChanged = !(index == count);
    }

    if (sequenceChanged)
    {
        auto box = m_view->valueComboBox;
        auto const first = m_parameter->begin();
        auto const last = m_parameter->end();
        while(box->count() > 0)
            box->removeItem(0);

        std::for_each(first, last, [&box](std::string const& item)
        {
            auto const text = util::StringConverter::toUtf8QString(item);
            box->addItem(text);
        });
    }

    auto const index = m_parameter->index();
    if (m_view->valueComboBox->currentIndex() != index)
        m_view->valueComboBox->setCurrentIndex(index);

    if (m_view->accessBox->currentIndex() != access)
        m_view->accessBox->setCurrentIndex(access);

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
}

void EnumerationView::updateEnumeration()
{
    auto const first = m_parameter->begin();
    auto const last = m_parameter->end();
    EditEnumerationDialog dialog(this, first, last);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_parameter->assign(std::begin(dialog), std::end(dialog));
    }
}

void EnumerationView::updateValue(int value)
{
    if (value > -1)
        m_parameter->setIndex(value);
}

void EnumerationView::updateDescription()
{
    auto const description = util::StringConverter::toUtf8StdString(m_view->descriptionText->text());
    m_parameter->setDescription(description);
}

void EnumerationView::updateSchema()
{
    auto const schema = util::StringConverter::toUtf8StdString(m_view->schemaText->text());
    m_parameter->setSchema(schema);
}

void EnumerationView::updateAccess(int access)
{
    if (access > -1)
    {
        m_parameter->setAccess(static_cast<gadget::Access::_Domain>(access));
    }
}

void EnumerationView::updateStreamIdentifierState(bool state)
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

void EnumerationView::updateStreamDescriptorState(bool state)
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

void EnumerationView::updateStreamIdentifier()
{
    auto const isChecked = m_view->checkUseStreamIdentifier->isChecked();
    if (isChecked)
    {
        auto const identifier = m_view->streamIdentifierBox->value();
        m_parameter->setStreamIdentifier(identifier);
    }
}

void EnumerationView::updateStreamFormat(int index)
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

void EnumerationView::updateStreamOffset()
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

void EnumerationView::notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*)
{
    updateUi();
}
