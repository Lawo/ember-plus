#include "CreateParameterDialog.h"
#include "util/StringConverter.h"

CreateParameterDialog::CreateParameterDialog(QWidget* parent)
    : QDialog(parent)
    , m_dialog(new Ui::CreateParameterDialog())
{
    m_dialog->setupUi(this);
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
    m_dialog->descriptionText->setText(text);
}
