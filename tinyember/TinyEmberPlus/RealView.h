#ifndef __TINYEMBER_REALVIEW_H
#define __TINYEMBER_REALVIEW_H

#include <qframe.h>
#include "gadget/Parameter.h"
#include "ui_RealView.h"

namespace gadget
{
    class RealParameter;
}

class RealView : public QFrame, gadget::Parameter::DirtyStateListener
{
    Q_OBJECT
    public:
        explicit RealView(QWidget* parent, gadget::RealParameter* parameter);
        virtual ~RealView();

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
        gadget::RealParameter* m_parameter;
        Ui::RealView* m_view;
};

#endif//__TINYEMBER_REALVIEW_H
