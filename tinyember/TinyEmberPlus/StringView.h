#ifndef __TINYEMBER_STRINGVIEW_H
#define __TINYEMBER_STRINGVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_StringView.h"

namespace gadget 
{
    class StringParameter;
}

class StringView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        StringView(QWidget* parent, gadget::StringParameter* parameter);

        virtual ~StringView(void);

    public slots:
        void updateUi();
        void updateDescription();
        void updateValue();
        void updateAccess(int access);
        void updateStreamIdentifierState(bool state);
        void updateStreamIdentifier();

    private:
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        Ui::StringView* m_view;
        gadget::StringParameter* m_parameter;
};

#endif//__TINYEMBER_STRINGVIEW_H
