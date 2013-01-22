#include "NodeView.h"
#include "gadget/Node.h"
#include "util/StringConverter.h"

NodeView::NodeView(QWidget* parent, gadget::Node* node)
    : QFrame(parent)
    , m_view(new Ui::NodeView())
    , m_node(node)
{
    m_view->setupUi(this);
    updateUi();
}

NodeView::~NodeView()
{
    delete m_view;
}

void NodeView::updateUi()
{
    auto const identifier = util::StringConverter::toUtf8QString(m_node->identifier());
    auto const description = util::StringConverter::toUtf8QString(m_node->description());
    auto const number = QVariant(m_node->number()).toString();

    if (m_view->identifierText->text() != identifier)
        m_view->identifierText->setText(identifier);

    if (m_view->descriptionText->text() != description)
        m_view->descriptionText->setText(description);

    if (m_view->numberText->text() != number)
        m_view->numberText->setText(number);
}

void NodeView::updateDescription()
{
    auto const description = util::StringConverter::toUtf8StdString(m_view->descriptionText->text());
    m_node->setDescription(description);
}
