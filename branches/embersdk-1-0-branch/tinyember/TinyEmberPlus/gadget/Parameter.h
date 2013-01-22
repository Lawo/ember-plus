#ifndef __TINYEMBER_GADGET_PARAMETER_H
#define __TINYEMBER_GADGET_PARAMETER_H

#include <list>
#include <memory>
#include "../Types.h"
#include "Access.h"
#include "DirtyStateListener.h"
#include "Formula.h"
#include "ParameterField.h"
#include "ParameterType.h"
#include "StreamDescriptor.h"
#include "Subscriber.h"

namespace gadget
{
    class Node;
    class ParameterTypeVisitor;
    class ParameterTypeVisitorConst;

    class Parameter : public Subscribable
    {
        public:
            typedef DirtyStateListener<ParameterFieldState::flag_type, Parameter const*> DirtyStateListener;

            virtual ~Parameter();

            virtual void accept(ParameterTypeVisitor& visitor) = 0;

            virtual void accept(ParameterTypeVisitorConst const& visitor) const = 0;

            virtual std::string toDisplayValue() const = 0;

            int number() const;

            int streamIdentifier() const;

            String const& identifier() const;

            String const& description() const;

            ParameterType const type() const;

            Node const* parent() const;

            Access access() const;

            Formula const& formula() const;

            ParameterFieldState const& dirtyState() const;

            StreamDescriptor const* streamDescriptor() const;

            bool isDirty() const;

            void clearDirtyState();

            void setDescription(String const& value);

            void setAccess(Access const& value);

            void setFormula(Formula const& formula);

            void setStreamIdentifier(int value);

            void setStreamDescriptor(std::shared_ptr<StreamDescriptor> descriptor = nullptr);

            void setStreamDescriptor(StreamFormat const& format, unsigned offset);

            void registerListener(DirtyStateListener* listener);

            void unregisterListener(DirtyStateListener* listener);

            bool hasStreamIdentifier() const;

            bool hasStreamDescriptor() const;

        protected:
            Parameter(ParameterType const& type, Node* parent, String const& identifier, int number);

            void markDirty(ParameterField const& field, bool notify);

            void notify() const;

        protected:
            virtual void subscribed();

            virtual void unsubscribed();

            virtual void subscriptionCountChanged(size_type newCount);

        private:
            ParameterType const m_type;
            int const m_number;
            int m_streamIdentifier;
            String const m_identifier;
            String m_description;
            Node* m_parent;
            Formula m_formula;
            ParameterFieldState m_state;
            Access::value_type m_access;
            std::list<DirtyStateListener*> m_listeners;
            std::shared_ptr<StreamDescriptor> m_streamDescriptor;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETER_H
