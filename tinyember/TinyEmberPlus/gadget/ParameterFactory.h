#ifndef __TINYEMBER_GADGET_PARAMETERFACTORY_H
#define __TINYEMBER_GADGET_PARAMETERFACTORY_H

#include "../Types.h"

namespace gadget
{
    class EnumParameter;
    class IntegerParameter;
    class StringParameter;
    class RealParameter;
    class Node;

    class ParameterFactory
    {
        public:
            static IntegerParameter* create(Node* parent, String const& identifier, int min, int max, int value);

            static RealParameter* create(Node* parent, String const& identifier, double min, double max, double value);

            static StringParameter* create(Node* parent, String const& identifier, String const& value, std::size_t maxLength = 0);

            static EnumParameter* create(Node* parent, String const& identifier);
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERFACTORY_H
