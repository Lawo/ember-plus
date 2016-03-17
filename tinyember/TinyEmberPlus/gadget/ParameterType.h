#ifndef __TINYEMBER_GADGET_PARAMETERTYPE_H
#define __TINYEMBER_GADGET_PARAMETERTYPE_H

namespace gadget
{
    /**
     * Scoped enumeration which contains the symbolic names of all supported
     * parameter types.
     */
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

            /**
             * Initializes a new Parameter type.
             * @param value The value to initialize this instance with.
             */
            ParameterType(_Domain value)
                : m_value(value)
            {}

            /**
             * Returns the current numeric value of this ParameterType instance.
             * @return The current numeric value.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERTYPE_H
