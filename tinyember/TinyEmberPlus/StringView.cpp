#include "StringView.h"
#include "gadget/StringParameter.h"
#include "util/StringConverter.h"
#include <sstream>

StringView::StringView(QWidget* parent, gadget::StringParameter* parameter)
    : QFrame(parent)
    , m_view(new Ui::StringView())
    , m_parameter(parameter)
{
    m_view->setupUi(this);
    m_parameter->registerListener(this);
    updateUi();
}

StringView::~StringView(void)
{
    m_parameter->unregisterListener(this);
    delete m_view;
}

void StringView::updateUi()
{
    auto const identifier = util::StringConverter::toUtf8QString(m_parameter->identifier());
    auto const description = util::StringConverter::toUtf8QString(m_parameter->description());
    auto const number = QVariant(m_parameter->number()).toString();
    auto const value = QString::fromUtf8(m_parameter->value().c_str());
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

    if (m_view->valueText->text() != value)
        m_view->valueText->setText(value);

    if (m_view->accessBox->currentIndex() != access)
        m_view->accessBox->setCurrentIndex(access);

    auto const hasStreamIdentifier = m_parameter->hasStreamIdentifier();
    if (m_view->checkUseStreamIdentifier->isChecked() != hasStreamIdentifier)
        m_view->checkUseStreamIdentifier->setChecked(hasStreamIdentifier);

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

    if (m_parameter->dirtyState().isSet(::gadget::ParameterField::SubscriptionCount))
    {
        auto const subscriptions = m_parameter->subscribers();
        auto stream = std::stringstream();
        auto const suffix = subscriptions != 1 ? "subscribers" : "subscriber";
        stream << "Streaming (" << subscriptions << " " << suffix << ")";

        m_view->streamingGroupBox->setTitle(QString::fromStdString(stream.str()));
    }
}

void StringView::updateDescription()
{
    auto const description = util::StringConverter::toUtf8StdString(m_view->descriptionText->text());
    m_parameter->setDescription(description);
}

void StringView::updateSchema()
{
    auto const schema = util::StringConverter::toUtf8StdString(m_view->schemaText->text());
    m_parameter->setSchema(schema);
}

void StringView::updateValue()
{
    auto const value = util::StringConverter::toUtf8StdString(m_view->valueText->text());
    m_parameter->setValue(value);
}

void StringView::updateAccess(int access)
{
    if (access > -1)
    {
        m_parameter->setAccess(static_cast<gadget::Access::_Domain>(access));
    }
}

void StringView::updateStreamIdentifierState(bool state)
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

void StringView::updateStreamIdentifier()
{
    auto const isChecked = m_view->checkUseStreamIdentifier->isChecked();
    if (isChecked)
    {
        auto const identifier = m_view->streamIdentifierBox->value();
        m_parameter->setStreamIdentifier(identifier);
    }
}

void StringView::notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*)
{
    updateUi();
}

