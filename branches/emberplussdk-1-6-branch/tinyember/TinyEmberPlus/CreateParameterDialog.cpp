#include <ember/glow/util/Validation.hpp>
#include <qmessagebox.h>
#include "CreateParameterDialog.h"
#include "gadget/Node.h"
#include "util/StringConverter.h"
#include "util/Validation.h"

CreateParameterDialog::CreateParameterDialog(QWidget* parent, gadget::Node const* owner)
    : QDialog(parent)
    , m_dialog(new Ui::CreateParameterDialog())
    , m_owner(owner)
{
    m_dialog->setupUi(this);
    m_dialog->okButton->setEnabled(false);

    if (owner != nullptr)
    {
        auto const identifier = util::StringConverter::toUtf8QString(owner->displayName());
        setWindowTitle("Create new parameter for \"" + identifier + "\"");
    }
}

CreateParameterDialog::~CreateParameterDialog(void)
{
    delete m_dialog;
}

gadget::ParameterType const CreateParameterDialog::type() const
{
    auto const typestring = m_dialog->typeBox->currentText().toLower().toStdString();
    if (typestring == "integer")
        return gadget::ParameterType::Integer;
    else if (typestring == "real")
        return gadget::ParameterType::Real;
    else if (typestring == "enumeration")
        return gadget::ParameterType::Enum;
    else if (typestring == "boolean")
        return gadget::ParameterType::Boolean;
    else
        return gadget::ParameterType::String;
}

std::string CreateParameterDialog::identifier() const
{
    auto const identifier = util::StringConverter::toUtf8StdString(m_dialog->identifierText->text());
    return identifier;
}

std::string CreateParameterDialog::description() const
{
    auto const description = util::StringConverter::toUtf8StdString(m_dialog->descriptionText->text());
    return description;
}

void CreateParameterDialog::identifierChanged(QString text)
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
