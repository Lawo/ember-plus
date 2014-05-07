#ifndef __TINYEMBER_GADGET_PARAMETERFACTORY_H
#define __TINYEMBER_GADGET_PARAMETERFACTORY_H

#include "../Types.h"

namespace gadget
{
    /** Forward declarations */
    class BooleanParameter;
    class EnumParameter;
    class IntegerParameter;
    class StringParameter;
    class RealParameter;
    class Node;
    
    /**
     * Factory class which is able to create parameter instances for the default implementations.
     */
    class ParameterFactory
    {
        public:
            /**
             * Creates a new integer parameter.
             * @param parent The parameter's parent.
             * @param identifier The string identifier.
             * @param minimum The smallest value accepted.
             * @param maximum The largest value accepted.
             * @param value The initial parameter value.
             * @return The new parameter instance.
             */
            static IntegerParameter* create(Node* parent, String const& identifier, int minimum, int maximum, int value);

            /**
             * Creates a new real parameter.
             * @param parent The parameter's parent.
             * @param identifier The string identifier.
             * @param minimum The smallest value accepted.
             * @param maximum The largest value accepted.
             * @param value The initial parameter value.
             * @return The new parameter instance.
             */
            static RealParameter* create(Node* parent, String const& identifier, double minimum, double maximum, double value);

            /**
             * Creates new string parameter.
             * @param parent The parameter's parent.
             * @param identifier The string identifier.
             * @param value The initial string value.
             * @param maxLength The allowed length for the string value. If set to 0, the length is not limited.
             * @return The new parameter instance.
             */
            static StringParameter* create(Node* parent, String const& identifier, String const& value, std::size_t maxLength = 0);

            /**
             * Creates a new enumeration parameter.
             * @param parent The parameter's parent.
             * @param identifier The string identifier.
             * @return The new parameter instance.
             */
            static EnumParameter* create(Node* parent, String const& identifier);

            /**
             * Creates a new boolean parameter.
             * @param parent The parameter's parent.
             * @param identifier The string identifier.
             * @return The new parameter instance.
             */
            static BooleanParameter* create(Node* parent, String const& identifier, bool value);
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERFACTORY_H
