#ifndef CREATEPARAMETERDIALOG_H
#define CREATEPARAMETERDIALOG_H

#include <QDialog>

/** Forward declarations **/
namespace Ui {
    class CreateParameterDialog;
}

namespace gadgets {
    class Node;
    class Parameter;
}

/**
  * Creates a new parameter
  */
class CreateParameterDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent window
     */
    explicit CreateParameterDialog(QWidget *parent);

    /** Destructor **/
    virtual ~CreateParameterDialog();

    /**
     * Creates a new parameter based on the current settings.
     * @param parent Parent node.
     * @return Returns the created parameter.
     */
    gadgets::Parameter* createParameter(gadgets::Node& parent) const;

private slots:
    /**
     * Handles changes of the identifier string.
     * @param text The new text of the identifier
     */
    void identifierChanged(QString text);

private:
    Ui::CreateParameterDialog *ui;
};

#endif // CREATEPARAMETERDIALOG_H
