#ifndef __TINYEMBER_REALVIEW_H
#define __TINYEMBER_REALVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_RealView.h"

/** Forward declaration */
namespace gadget
{
    class RealParameter;
}

/**
 * A view displaying the properties of a real parameter.
 */
class RealView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        /**
         * Initializes a new RealView.
         * @param parent The parent widget of this view.
         * @param parameter The parameter to display.
         */
        explicit RealView(QWidget* parent, gadget::RealParameter* parameter);

        /** Destructor */
        virtual ~RealView();

    private slots:
        /**
         * Reloads all properties.
         */
        void updateUi();

        /** Updates the parameter's description. */
        void updateDescription();

        /** Updates the paramter's value. */
        void updateValue();

        /** Updates the parameter's maximum value. */
        void updateMax();

        /** Updates the parameter's minimum value. */
        void updateMin();

        /** Updates the parameter's format string. */
        void updateFormat();

        /**
         * Updates the access type.
         * @param access The new access type.
         */
        void updateAccess(int access);

        /**
         * This slot is invoked when the check box of the stream identifier changes.
         * @param state The new checkbox state.
         */
        void updateStreamIdentifierState(bool state);

        /**
         * This slot is invoked when the check box of the stream descriptor changes.
         * @param state The new checkbox state.
         */
        void updateStreamDescriptorState(bool state);

        /**
         * Updates the parameter's stream identifier.
         */
        void updateStreamIdentifier();

        /**
         * Updates the parameter's stream format.
         * @param index The index of the new stream format. See @see util::StreamFormatConverter for details
         *      about the different formats.
         */
        void updateStreamFormat(int index);

        /**
         * Updates the parameters stream offset.
         */
        void updateStreamOffset();

        /**
         * Updates the parameter's rx term.
         */
        void updateRxTerm();

        /**
         * Updates the parameter's tx term.
         */
        void updateTxTerm();

    private:
        /**
         * Updates the user interface.
         * @param state The new parameter state.
         */
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        gadget::RealParameter* m_parameter;
        Ui::RealView* m_view;
};

#endif//__TINYEMBER_REALVIEW_H
