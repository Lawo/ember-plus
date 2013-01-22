#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H

#include <QFrame>
#include "gadgets/EntityStateListener.h"
#include "gadgets/states/StringState.h"

/** Forward declarations **/
namespace Ui {
    class StringParameterView;
}

/**
 * Property View for a String Parameter
 */
class StringParameterView : public QFrame, gadgets::EntityStateListener
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param parameter Parameter
     * @param state The parameter's state
     */
    StringParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::StringState* state);

    /**
     * Destructor
     */
    ~StringParameterView();

private:
    /**
     * @see EntityStateListener
     */
    virtual void dirtyStateChanged(gadgets::Entity const& entity, gadgets::PropertyState const& state);

private slots:
    /**
     * Updates all UI elements that display a property of the entity
     */
    void updateUi();

    /**
     * Updates the entity's value
     */
    void updateEntityValue();

    /**
     * Updates the entity's writeable flag
     */
    void updateEntityWriteable();

    /**
     * Updates the entity's command flag
     */
    void updateEntityCommand();

    /**
     * Updates the entity's description text
     */
    void updateEntityDescription();

private:
    Ui::StringParameterView *ui;
    gadgets::Parameter* m_parameter;
    gadgets::states::StringState* m_state;
};

#endif // STRINGPARAMETER_H
