#ifndef __TINYEMBER_GADGET_REALPARAMETER_H
#define __TINYEMBER_GADGET_REALPARAMETER_H

#include "Parameter.h"
#include "../Types.h"

namespace gadget
{
    /** Forward declaration */
    class ParameterFactory;

    /**
     * A parameter that stores a real value.
     */
    class RealParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef double value_type;

            /**
             * Returns the smallest value allowed.
             * @return The smallest value allowed.
             */
            value_type minimum() const;

            /**
             * Returns the largest value allowed.
             * @return The largest value allowed.
             */
            value_type maximum() const;

            /**
             * Returns the current parameter value.
             * @return The current parameter value.
             */
            value_type value() const;

            /**
             * Returns the parameter's format string.
             * @return The parameter's format string.
             */
            String const& format() const;

            /**
             * Sets the smallest value allowed for this parameter.
             * @param value The new minimum value.
             */
            void setMin(value_type value);

            /**
             * Sets the largest value allowed for this parameter.
             * @param value The new maximum value.
             */
            void setMax(value_type value);

            /**
             * Updates the parameter's value.
             * @param value The new value to set.
             * @param forceNotification If set to true, the registered state listeners
             *      will be informed about the value update even if the value hasn't changed.
             */
            void setValue(value_type value, bool forceNotification = false);

            /**
             * Sets a new format string. This must be a C-style format string.
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
             * Initializes a new RealParameter instance.
             * @param parent The parameter's parent node.
             * @param identifier The string identifier of this parameter.
             * @param number The number of this parameter.
             * @param minimum The smallest value accepted by this parameter.
             * @param maximum The largest value accepted by this parameter.
             * @param value The initial parameter value.
             */
            RealParameter(Node* parent, String const& identifier, int number, value_type minimum, value_type maximum, value_type value);

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

#endif//__TINYEMBER_GADGET_REALPARAMETER_H
