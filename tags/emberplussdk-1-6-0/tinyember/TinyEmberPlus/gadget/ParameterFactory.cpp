#include <algorithm>
#include "ParameterFactory.h"
#include "BooleanParameter.h"
#include "EnumParameter.h"
#include "IntegerParameter.h"
#include "RealParameter.h"
#include "StringParameter.h"
#include "util/NumberFactory.h"

namespace gadget
{
    BooleanParameter* ParameterFactory::create(Node* parent, String const& identifier, bool value)
    {
        auto const number = util::NumberFactory::create(parent);
        auto const where = std::end(parent->m_parameters);
        auto parameter = new BooleanParameter(parent, identifier, number, value);
        parent->m_parameters.insert(where, parameter);

        return parameter;
    }

    IntegerParameter* ParameterFactory::create(Node* parent, String const& identifier, int minimum, int maximum, int value)
    {
        auto const number = util::NumberFactory::create(parent);
        auto const where = std::end(parent->m_parameters);
        auto parameter = new IntegerParameter(parent, identifier, number, minimum, maximum, value);
        parent->m_parameters.insert(where, parameter);

        return parameter;
    }

    RealParameter* ParameterFactory::create(Node* parent, String const& identifier, double minimum, double maximum, double value)
    {
        auto const number = util::NumberFactory::create(parent);
        auto const where = std::end(parent->m_parameters);
        auto parameter = new RealParameter(parent, identifier, number, minimum, maximum, value);
        parent->m_parameters.insert(where, parameter);

        return parameter;
    }

    StringParameter* ParameterFactory::create(Node* parent, String const& identifier, String const& value, std::size_t maxLength)
    {
        auto const number = util::NumberFactory::create(parent);
        auto const where = std::end(parent->m_parameters);
        auto parameter = new StringParameter(parent, identifier, number, value, maxLength);
        parent->m_parameters.insert(where, parameter);

        return parameter;
    }

    EnumParameter* ParameterFactory::create(Node* parent, String const& identifier)
    {
        auto const number = util::NumberFactory::create(parent);
        auto const where = std::end(parent->m_parameters);
        auto parameter = new EnumParameter(parent, identifier, number);
        parent->m_parameters.insert(where, parameter);

        return parameter;
    }
}
