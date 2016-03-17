#include <ember/glow/util/Validation.hpp>
#include <qmessagebox.h>
#include "gadget/Node.h"
#include "util/StringConverter.h"
#include "util/Validation.h"
#include "CreateNodeDialog.h"

CreateNodeDialog::CreateNodeDialog(QWidget* parent, gadget::Node const* owner)
    : QDialog(parent)
    , m_dialog(new Ui::CreateNodeDialog())
    , m_owner(owner)
{
    m_dialog->setupUi(this);
    m_dialog->okButton->setEnabled(false);

    if (owner != nullptr)
    {
        auto const identifier = util::StringConverter::toUtf8QString(owner->displayName());
        setWindowTitle("Create new child node for \"" + identifier + "\"");
    }
}


CreateNodeDialog::~CreateNodeDialog(void)
{
    delete m_dialog;
}

std::string CreateNodeDialog::identifier() const
{
    auto const identifier = util::StringConverter::toUtf8StdString(m_dialog->identifierText->text());
    return identifier;
}

std::string CreateNodeDialog::description() const
{
    auto const description = util::StringConverter::toUtf8StdString(m_dialog->descriptionText->text());
    return description;
}


void CreateNodeDialog::identifierChanged(QString text)
{
    auto const identifier = util::StringConverter::toUtf8StdString(text);
    auto const isValidIdentifier = libember::glow::util::Validation::isIdentifierValid(identifier);
    auto const isUniqueIdentifier = util::Validation::isUniqueIdentifier(m_owner, identifier);
    auto const enableOkButton = isValidIdentifier && isUniqueIdentifier;

    if (enableOkButton)
    {
        m_dialog->okButton->setEnabled(true);
        m_dialog->descriptionText->setText(text);
        m_dialog->statusText->setText("");
    }
    else
    {
        m_dialog->okButton->setEnabled(false);

        if (isValidIdentifier == false)
        {
            QString const text = "The identifier must start with a letter or '_' and must not contain the '/' character";
            QMessageBox::warning(nullptr, "Validation error", text);
        }

        if (isUniqueIdentifier == false)
        {
            m_dialog->statusText->setText("This identifier is already in use");
        }
        else
        {
            m_dialog->statusText->setText("");
        }
    }
}
