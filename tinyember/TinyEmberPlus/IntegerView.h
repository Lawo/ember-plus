#ifndef __TINYEMBER_INTEGERVIEW_H
#define __TINYEMBER_INTEGERVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_IntegerView.h"

namespace gadget
{
    class IntegerParameter;
}

class IntegerView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        explicit IntegerView(QWidget* parent, gadget::IntegerParameter* parameter);
        virtual ~IntegerView();

    private slots:
        void updateUi();
        void updateDescription();
        void updateValue();
        void updateMax();
        void updateMin();
        void updateFormat();
        void updateAccess(int access);
        void updateStreamIdentifierState(bool state);
        void updateStreamDescriptorState(bool state);
        void updateStreamIdentifier();
        void updateStreamFormat(int index);
        void updateStreamOffset();
        void updateRxTerm();
        void updateTxTerm();

    private:
        virtual void notifyStateChanged(gadget::ParameterFieldState const& state, gadget::Parameter const*);

    private:
        gadget::IntegerParameter* m_parameter;
        Ui::IntegerView* m_view;
};

#endif//__TINYEMBER_INTEGERVIEW_H
