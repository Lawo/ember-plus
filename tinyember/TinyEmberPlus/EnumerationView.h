#ifndef __TINYEMBER_ENUMERATIONVIEW_H
#define __TINYEMBER_ENUMERATIONVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_EnumerationView.h"

/** Forward declaration */
namespace gadget
{
    class EnumParameter;
}

/**
 * A view which can be used to edit an enumeration parameter.
 */
class EnumerationView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        /**
         * Initializes a new EnumerationView.
         * @param parent The parent widget of this view.
         * @param parameter The parameter to display.
         */
        explicit EnumerationView(QWidget* parent, gadget::EnumParameter* parameter);

        /** Destructor */
        virtual ~EnumerationView();

    private slots:
        /** Reloads all properties. */
        void updateUi();

        /**
         * Updates the parameter's enumeration.
         */
        void updateEnumeration();

        /**
         * Updates the parameter value.
         * @param The new value.
         */
        void updateValue(int value);

        /** Updates the parameter's description. */
        void updateDescription();

        /**
         * Updates the access type.
         * @param access The new access.
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

    private:
        /**
         * Updates the user interface.
         * @param state The new parameter state.
         */
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        Ui::EnumerationView* m_view;
        gadget::EnumParameter* m_parameter;
};

#endif//__TINYEMBER_ENUMERATIONVIEW_H
