#ifndef INTPARAMETERVIEW_H
#define INTPARAMETERVIEW_H

#include <QFrame>
#include "gadgets/Entity.h"
#include "gadgets/EntityStateListener.h"
#include "gadgets/States/NumericState.h"

/** Forward declarations **/
namespace Ui {
    class IntParameterView;
}

/**
 * Property view for an integer-type parameter
 */
class IntParameterView : public QFrame, gadgets::EntityStateListener
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param parameter Parameter
     * @param state Parameter's state
     */
    IntParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::IntState* state);

    /**
     * Destructor
     */
    ~IntParameterView();

private:
    virtual void dirtyStateChanged(gadgets::Entity const& entity, gadgets::PropertyState const& state);

private slots:
    /**
     * Updates all UI items with the parameter's properties
     */
    void updateUi();

    /**
     * Updates the entity's min value
     */
    void updateEntityMin();

    /**
     * Updates the entity's max value
     */
    void updateEntityMax();

    /**
     * Updates the entity's value
     */
    void updateEntityValue();

    /**
     * Updates the entity's format string
     */
    void updateEntityFormat();

    /**
     * Updates the entity's writeable flag
     */
    void updateEntityIsWriteable();

    /**
     * Updates the entity's command flag
     */
    void updateEntityIsCommand();

    /**
     * Updates the entity's description
     */
    void updateEntityDescription();

private:
    Ui::IntParameterView *ui;
    gadgets::Parameter *m_parameter;
    gadgets::states::IntState* m_state;
};

#endif // INTPARAMETERVIEW_H
