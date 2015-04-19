#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "BooleanParameter.h"
#include "EnumParameter.h"
#include "IntegerParameter.h"
#include "RealParameter.h"
#include "StringParameter.h"
#include "StreamManager.h"
#include "Parameter.h"

namespace gadget
{
    void StreamManager::RandomValueGenerator::visit(BooleanParameter* parameter)
    {
        auto const result = std::rand() % 100;
        auto const value = result < 50;
        parameter->setValue(value);
    }

    void StreamManager::RandomValueGenerator::visit(EnumParameter* parameter)
    {
        if (parameter->size() > 0)
        {
            auto const min = 0;
            auto const max = parameter->size() - 1;
            auto const next = std::rand() % (max - min);

            parameter->setIndex(next);
        }
    }

    void StreamManager::RandomValueGenerator::visit(StringParameter* parameter)
    {
        auto const length = std::min((StreamManager::size_type)4, parameter->maxLength());
        auto value = std::string(length, ' ');
        for (auto i = 0U; i < length; ++i)
        {
            auto const character = rand() % 26;
            value[i] = 'a' + character;
        }

        parameter->setValue(value);
    }

    void StreamManager::RandomValueGenerator::visit(IntegerParameter* parameter)
    {
        auto const min = parameter->minimum();
        auto const max = parameter->maximum();
        auto const range = max - min;
        auto const sample = 1.0 * rand() / RAND_MAX;
        
        auto const value = static_cast<int>(sample * range + min);
        parameter->setValue(value);
    }

    void StreamManager::RandomValueGenerator::visit(RealParameter* parameter)
    {
        auto const min = parameter->minimum();
        auto const max = parameter->maximum();
        auto const range = max - min;
        auto const sample = 1.0 * rand() / RAND_MAX;
        
        auto const value = (sample * range + min);
        parameter->setValue(value);
    }


    StreamManager& StreamManager::instance()
    {
        static StreamManager instance;
        return instance;
    }

    bool StreamManager::isParameterTransmittedViaStream(Parameter const* parameter) const
    {
        if (parameter->hasStreamDescriptor() || parameter->hasStreamIdentifier())
            return true;
        else
            return false;
    }

    void StreamManager::updateValues()
    {
        auto first = begin();
        auto const last = end();
        auto valueGenerator = RandomValueGenerator();
        for ( ; first != last; ++first)
        {
            (*first)->accept(valueGenerator);
        }
    }

    void StreamManager::registerParameter(Parameter* parameter)
    {
        auto const first = std::begin(m_parameters);
        auto const last = std::end(m_parameters);
        auto const result = std::find_if(first, last, [&parameter](decltype(*first) entry) -> bool
        {
            return entry == parameter;
        });

        if (result == last)
        {
            m_parameters.push_back(parameter);
        }
    }

    void StreamManager::unregisterParameter(Parameter* parameter)
    {
        auto const first = std::begin(m_parameters);
        auto const last = std::end(m_parameters);
        auto const where = std::find_if(first, last, [&parameter](decltype(*first) entry) -> bool
        {
            return entry == parameter;
        });

        if (where != last)
        {
            m_parameters.erase(where);
        }
    }

    StreamManager::const_iterator StreamManager::begin() const
    {
        return m_parameters.begin();
    }

    StreamManager::const_iterator StreamManager::end() const
    {
        return m_parameters.end();
    }

    StreamManager::size_type StreamManager::size() const
    {
        return m_parameters.size();
    }
}
