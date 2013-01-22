#ifndef REALPARAMETERVIEW_H
#define REALPARAMETERVIEW_H

#include <QFrame>
#include "gadgets/Entity.h"
#include "gadgets/EntityStateListener.h"
#include "gadgets/states/NumericState.h"

namespace Ui {
    class RealParameterView;
}

/**
 * View for a real-type parameter
 */
class RealParameterView : public QFrame, gadgets::EntityStateListener
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param parameter Parameter
     * @param state The parameter's state
     */
    RealParameterView(QWidget *parent, gadgets::Parameter* parameter, gadgets::states::RealState *state);

    /**
     * Destructor
     */
    ~RealParameterView();

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
    void updateEntityWriteable();

    /**
     * Updates the entity's command flag
     */
    void updateEntityCommand();

    /**
     * Updates the entity's description
     */
    void updateEntityDescription();

private:
    Ui::RealParameterView *ui;
    gadgets::Parameter* m_parameter;
    gadgets::states::RealState *m_state;
};

#endif // REALPARAMETERVIEW_H
