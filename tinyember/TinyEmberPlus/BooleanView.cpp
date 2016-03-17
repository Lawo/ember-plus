#include "BooleanView.h"
#include "gadget/BooleanParameter.h"
#include "util/StringConverter.h"
#include <sstream>

BooleanView::BooleanView(QWidget* parent, gadget::BooleanParameter* parameter)
    : QFrame(parent)
    , m_view(new Ui::BooleanView())
    , m_parameter(parameter)
{
    m_view->setupUi(this);
    m_parameter->registerListener(this);
    updateUi();
}

BooleanView::~BooleanView(void)
{
    m_parameter->unregisterListener(this);
    delete m_view;
}

void BooleanView::updateUi()
{
    auto const identifier = util::StringConverter::toUtf8QString(m_parameter->identifier());
    auto const description = util::StringConverter::toUtf8QString(m_parameter->description());
    auto const number = QVariant(m_parameter->number()).toString();
    auto const valueIndex = m_parameter->value() ? 1 : 0;
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

    if (m_view->valueBox->currentIndex() != valueIndex)
        m_view->valueBox->setCurrentIndex(valueIndex);

    if (m_view->accessBox->currentIndex() != access)
        m_view->accessBox->setCurrentIndex(access);

}

void BooleanView::updateDescription()
{
    auto const description = util::StringConverter::toUtf8StdString(m_view->descriptionText->text());
    m_parameter->setDescription(description);
}

void BooleanView::updateSchema()
{
    auto const schema = util::StringConverter::toUtf8StdString(m_view->schemaText->text());
    m_parameter->setSchema(schema);
}

void BooleanView::updateValue(int index)
{
    if (index == 0)
        m_parameter->setValue(false);
    else if (index == 1)
        m_parameter->setValue(true);
}

void BooleanView::updateAccess(int access)
{
    if (access > -1)
    {
        m_parameter->setAccess(static_cast<gadget::Access::_Domain>(access));
    }
}

void BooleanView::notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*)
{
    updateUi();
}

