#ifndef __TINYEMBER_GADGET_STRINGPARAMETER_H
#define __TINYEMBER_GADGET_STRINGPARAMETER_H

#include "Parameter.h"

namespace gadget
{
    /** Forward declarations */
    class ParameterFactory;

    /**
     * Implementation of a parameter class that stores a string value.
     */
    class StringParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef String value_type;
            typedef String::size_type size_type;
            typedef value_type const& const_reference;

            /**
             * Returns a reference to the current parameter value.
             * @return A reference to the current parameter value.
             */
            const_reference value() const;

            /**
             * Returns the maximum allowed length for this parameter's string value. If the length
             * is not limited, 0 is being returned.
             * @return the maximum allowed length for this parameter's string value.
             */
            size_type maxLength() const;

            /**
             * Returns true when the length of the string value is not limited.
             * @return true when the length of the string value is not limited.
             */
            bool isIndefiniteLength() const;

            /**
             * Sets the allowed length for the string value of this parameter.
             * @param value The new maximum length allowed. To remove the length limitation,
             *      pass zero.
             */
            void setMaxLength(size_type value);

            /**
             * Updates the parameter's value.
             * @param value The new value to set.
             * @param forceNotification If set to true, the registered event listeners will be
             *      informed about the value update, even if the value hasn't changed.
             */
            void setValue(const_reference value, bool forceNotification = false);

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitor& visitor);

            /** @see Parameter::toDisplayValue() */
            virtual std::string toDisplayValue() const;

        protected:
            /**
             * Initializes a new StringParameter.
             * @param parent The paramenter's parent node.
             * @param identifier The identifier string.
             * @param number The parameter's number.
             * @param value The initial value.
             * @param maxLength The allowed length for the string value or zero, if the length is not limited.
             */
            StringParameter(Node* parent, String const& identifier, int number, const_reference value, size_type maxLength);

            /**
             * This method is invoked by the setValue method when a value changes or the forceNotification
             * flag is set to true. The default implementation is empty.
             * @param value The new value.
             * @param forceNotification Indicates whether a dirty event shall be signaled, even if
             *      the value didn't change.
             */
            virtual void setValueImpl(value_type value, bool forceNotification);

        private:
            value_type m_value;
            size_type m_maxLength;
    };
}

#endif//__TINYEMBER_GADGET_STRINGPARAMETER_H
