#ifndef __TINYEMBER_GADGET_BOOLEANPARAMETER_H
#define __TINYEMBER_GADGET_BOOLEANPARAMETER_H

#include "Parameter.h"

namespace gadget
{
    /** Forward declarations */
    class ParameterFactory;

    /**
     * Represents a boolean parameter
     */
    class BooleanParameter : public Parameter
    {
        friend class ParameterFactory;
        public:
            typedef bool value_type;

            /**
             * Returns the current value of this parameter.
             * @return The current value of this parameter.
             */
            value_type value() const;

            /**
             * Updates the parameter's value.
             * @param value The new value to set.
             * @param forceNotification If set to true, the implementation is forced to mark the value dirty
             *      which causes the state listener to be notififed.
             */
            void setValue(value_type value, bool forceNotification = false);

            /** @see Parameter */
            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            /** @see Parameter */
            virtual void accept(ParameterTypeVisitor& visitor);

            /** @see Parameter */
            virtual std::string toDisplayValue() const;

        protected:
            /**
             * Initializes a new BooleanParameter.
             * @param parent The parent node of this parameter.
             * @param identifier The string identifier of this parameter.
             * @param number The number identifying this parameter.
             * @param value The initial parameter value.
             */
            BooleanParameter(Node* parent, String const& identifier, int number, value_type value);

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
    };
}

#endif//__TINYEMBER_GADGET_BOOLEANPARAMETER_H
