#ifndef __TINYEMBER_GADGET_PARAMETERTYPE_H
#define __TINYEMBER_GADGET_PARAMETERTYPE_H

namespace gadget
{
    struct ParameterType
    {
        public:
            typedef enum _Domain
            {
                Integer = 1,
                Real,
                String,
                Boolean,
                Trigger,
                Enum,
                Octets,
            };

            typedef std::size_t value_type;

            ParameterType(_Domain value)
                : m_value(value)
            {}

            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERTYPE_H
