#ifndef __TINYEMBER_STRINGVIEW_H
#define __TINYEMBER_STRINGVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_StringView.h"

/** Forward declaration */
namespace gadget 
{
    class StringParameter;
}

/**
 * A view which can be used to edit a string parameter.
 */
class StringView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        /**
         * Initializes a new StringView.
         * @param parent The parent widget of this view.
         * @param parameter The parameter to display.
         */
        StringView(QWidget* parent, gadget::StringParameter* parameter);

        /** Destructor */
        virtual ~StringView(void);

    public slots:
        /** Reloads all properties */
        void updateUi();

        /** Updates the parameter's description. */
        void updateDescription();

        /** Updates the parameter's schema */
        void updateSchema();

        /** Updates the parameter's value. */
        void updateValue();

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
         * Updates the parameter's stream identifier.
         */
        void updateStreamIdentifier();

    private:
        /**
         * Updates the user interface.
         * @param state The new parameter state.
         */
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        Ui::StringView* m_view;
        gadget::StringParameter* m_parameter;
};

#endif//__TINYEMBER_STRINGVIEW_H
