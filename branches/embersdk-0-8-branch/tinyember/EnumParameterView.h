#ifndef ENUMPARAMETERVIEW_H
#define ENUMPARAMETERVIEW_H

#include <QFrame>
#include "gadgets/EntityStateListener.h"
#include "gadgets/states/EnumState.h"

/** Forward declarations **/
namespace Ui {
    class EnumParameterView;
}

class EditEnumerationDialog;


/**
 * Property view for an enumerated parameter
 */
class EnumParameterView : public QFrame, gadgets::EntityStateListener
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param parameter Parameter
     * @param state Parameter's state
     */
    EnumParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::EnumState* state);

    /**
     * Destructor
     */
    ~EnumParameterView();

private:
    virtual void dirtyStateChanged(gadgets::Entity const& entity, gadgets::PropertyState const& state);

private slots:
    /**
     * Updates all ui elements with the parameter's properties
     */
    void updateUi();

    /**
     * Updates the parameter's value
     */
    void updateEntityValue(int value);

    /**
     * Updates the parameter's writeable flag
     */
    void updateEntityWriteable();

    /**
     * Updates the parameter's command flag
     */
    void updateEntityCommand();

    /**
     * Opens enumeration editor dialog
     */
    void on_editItemsButton_clicked();

    /**
     * Updates the entity's description
     */
    void updateEntityDescription();

private:
    Ui::EnumParameterView *ui;
    gadgets::Parameter* m_parameter;
    gadgets::states::EnumState* m_state;
};

#endif // ENUMPARAMETERVIEW_H
