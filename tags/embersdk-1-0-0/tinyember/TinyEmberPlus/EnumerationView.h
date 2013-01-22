#ifndef __TINYEMBER_ENUMERATIONVIEW_H
#define __TINYEMBER_ENUMERATIONVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_EnumerationView.h"

namespace gadget
{
    class EnumParameter;
}

class EnumerationView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        explicit EnumerationView(QWidget* parent, gadget::EnumParameter* parameter);

        virtual ~EnumerationView(void);

    private slots:
        void updateUi();
        void updateEnumeration();
        void updateValue(int value);
        void updateDescription();
        void updateAccess(int access);
        void updateStreamIdentifierState(bool state);
        void updateStreamDescriptorState(bool state);
        void updateStreamIdentifier();
        void updateStreamFormat(int index);
        void updateStreamOffset();

    private:
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        Ui::EnumerationView* m_view;
        gadget::EnumParameter* m_parameter;
};

#endif//__TINYEMBER_ENUMERATIONVIEW_H
