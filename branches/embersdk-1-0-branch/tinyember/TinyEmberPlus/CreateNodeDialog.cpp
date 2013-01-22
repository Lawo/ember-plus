#include "CreateNodeDialog.h"
#include "util\StringConverter.h"

CreateNodeDialog::CreateNodeDialog(QWidget* parent)
    : QDialog(parent)
    , m_dialog(new Ui::CreateNodeDialog())
{
    m_dialog->setupUi( this );
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
    m_dialog->descriptionText->setText(text);
}
