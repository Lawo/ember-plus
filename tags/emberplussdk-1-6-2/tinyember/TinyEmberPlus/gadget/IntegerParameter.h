#ifndef __TINYEMBER_GADGET_INTEGERPARAMETER_H
#define __TINYEMBER_GADGET_INTEGERPARAMETER_H

#include "Parameter.h"
#include "../Types.h"

namespace gadget
{
    /** Forward declaration */
    class ParameterFactory;

    /**
     * A parameter that stores an integer value.
     */
    class IntegerParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef long value_type;

            /**
             * Returns the allowed minimum value.
             * @return The allowed minimum value.
             */
            value_type minimum() const;

            /**
             * Returns the allowed maximum value.
             * @return The allowed maximum value.
             */
            value_type maximum() const;

            /**
             * Returns the current parameter value.
             * @return The current parameter value.
             */
            value_type value() const;

            /**
             * Returns the current format string.
             * @return The current format string.
             */
            String const& format() const;

            /**
             * Sets the smallest value this parameter accepts.
             * @param value The new minimum value.
             */
            void setMin(value_type value);

            /**
             * Sets the largest value this parameter accepts.
             * @param value The new maximum value.
             */
            void setMax(value_type value);

            /**
             * Updates the parameter's value and informs the dirty state listener if the value changes.
             * @param value The new value to set.
             * @param forceNotification If set to true, any attached state listener will be informed
             *      about the value update even if the value didn't change.
             */
            void setValue(value_type value, bool forceNotification = false);

            /**
             * Updates the format string. This must be a C-style format string.
             * @param value The new format string.
             */
            void setFormat(String const& value);

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitor& visitor);

            /** @see Parameter::toDisplayValue() */
            virtual std::string toDisplayValue() const;

        protected:
            /**
             * Initializes a new IntegerParameter.
             * @param parent The node owning this parameter.
             * @param identifier The parameter's identifier.
             * @param number The parameter's number.
             * @param max The accepted maximum for this parameter.
             * @param min The accepted minimum for this parameter.
             * @param value The initial parameter value.
             */
            IntegerParameter(Node* parent, String const& identifier, int number, value_type min, value_type max, value_type value);

            /**
             * This method is invoked by the setValue method when a value changes or the forceNotification
             * flag is set to true. The default implementation is empty.
             * @param value The new value.
             * @param forceNotification Indicates whether an dirty event shall be signalled, even if
             *      the value didn't change.
             */
            virtual void setValueImpl(value_type value, bool forceNotification);

        private:
            value_type m_value;
            value_type m_min;
            value_type m_max;
            String m_format;
    };
}

#endif//__TINYEMBER_GADGET_INTEGERPARAMETER_H
