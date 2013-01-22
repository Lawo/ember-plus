#ifndef __TINYEMBER_GADGET_STRINGPARAMETER_H
#define __TINYEMBER_GADGET_STRINGPARAMETER_H

#include "Parameter.h"

namespace gadget
{
    class ParameterFactory;

    class StringParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef String value_type;
            typedef String::size_type size_type;
            typedef value_type const& const_reference;

            const_reference value() const;

            size_type maxLength() const;

            bool isIndefiniteLength() const;

            void setMaxLength(size_type value);

            void setValue(const_reference value);

            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            virtual void accept(ParameterTypeVisitor& visitor);

            virtual std::string toDisplayValue() const;

        private:
            StringParameter(Node* parent, String const& identifier, int number, const_reference value, size_type maxLength);

            value_type m_value;
            size_type m_maxLength;
    };
}

#endif//__TINYEMBER_GADGET_STRINGPARAMETER_H
