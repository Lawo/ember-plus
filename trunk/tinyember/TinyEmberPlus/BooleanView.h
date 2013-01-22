#ifndef __TINYEMBER_BOOLEANVIEW_H
#define __TINYEMBER_BOOLEANVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_BooleanView.h"

/** Forward declaration */
namespace gadget 
{
    class BooleanParameter;
}

/**
 * A view which can be used to edit a boolean parameter.
 */
class BooleanView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        /**
         * Initializes a new BooleanView.
         * @param parent The parent widget of this view.
         * @param parameter The parameter to display.
         */
        BooleanView(QWidget* parent, gadget::BooleanParameter* parameter);

        /** Destructor */
        virtual ~BooleanView();

    public slots:
        /** Reloads all properties */
        void updateUi();

        /** Updates the parameter's description */
        void updateDescription();

        /**
         * Updates the parameter's value.
         * @param index The new parameter value.
         */
        void updateValue(int index);

        /**
         * Updates the parameter access.
         * @param The new access.
         */
        void updateAccess(int access);

    private:
        /**
         * Updates the user interface.
         * @param state The new parameter state.
         */
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        Ui::BooleanView* m_view;
        gadget::BooleanParameter* m_parameter;
};

#endif//__TINYEMBER_BOOLEANVIEW_H
