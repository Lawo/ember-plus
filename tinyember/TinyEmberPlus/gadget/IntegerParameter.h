#ifndef __TINYEMBER_GADGET_INTEGERPARAMETER_H
#define __TINYEMBER_GADGET_INTEGERPARAMETER_H

#include "Parameter.h"
#include "../Types.h"

namespace gadget
{
    class ParameterFactory;

    class IntegerParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef long value_type;

            value_type min() const;

            value_type max() const;

            value_type value() const;

            String const& format() const;

            void setMin(value_type value);

            void setMax(value_type value);

            void setValue(value_type value);

            void setFormat(String const& value);

            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            virtual void accept(ParameterTypeVisitor& visitor);

            virtual std::string toDisplayValue() const;

        private:
            IntegerParameter(Node* parent, String const& identifier, int number, value_type min, value_type max, value_type value);

            value_type m_value;
            value_type m_min;
            value_type m_max;
            String m_format;
    };
}

#endif//__TINYEMBER_GADGET_INTEGERPARAMETER_H
