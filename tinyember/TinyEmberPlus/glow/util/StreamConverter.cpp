#include <map>
#include <memory>
#include <vector>
#include <ember\Ember.hpp>
#include "StreamConverter.h"
#include "../../gadget/EnumParameter.h"
#include "../../gadget/IntegerParameter.h"
#include "../../gadget/RealParameter.h"
#include "../../gadget/StringParameter.h"
#include "../../gadget/Parameter.h"
#include "../../gadget/StreamManager.h"

using namespace ::libember::glow;

namespace glow { namespace util 
{
    libember::glow::GlowStreamEntry* StreamConverter::SingleStreamEntryFactory::create(gadget::Parameter* parameter)
    {
        auto converter = SingleStreamEntryFactory(parameter);
        return converter.m_entry;
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::EnumParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), static_cast<int>(parameter->index()));
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::StringParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), parameter->value());
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::IntegerParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), parameter->value());
    }

    void StreamConverter::SingleStreamEntryFactory::visit(gadget::RealParameter* parameter)
    {
        m_entry = new GlowStreamEntry(parameter->streamIdentifier(), parameter->value());
    }

    StreamConverter::SingleStreamEntryFactory::SingleStreamEntryFactory(gadget::Parameter* parameter)
        : m_entry(nullptr)
    {
        parameter->accept(*this);
    }




    libember::glow::GlowStreamCollection* StreamConverter::create(libember::glow::GlowStreamCollection* root, gadget::StreamManager const& manager)
    {
        typedef std::vector<gadget::Parameter*> ParameterCollection;
        std::map<int, std::unique_ptr<ParameterCollection> > dic;

        {
            auto it = std::begin(manager);
            auto const last= std::end(manager);
            for (; it != last; ++it)
            {
                auto& parameter = *it;
                auto const identifier = parameter->streamIdentifier();

                if (dic.find(identifier) == std::end(dic))
                {
                    dic[identifier] = std::unique_ptr<ParameterCollection>(new ParameterCollection());
                }

                dic[identifier]->push_back(parameter);
            }
        }

        {
            for each(auto& pair in dic)
            {
                auto& streams = *pair.second;
                auto const identifier = pair.first;
                auto const size = streams.size();

                if (size == 1 && streams.front()->hasStreamDescriptor() == false)
                {
                    auto parameter = streams.front();
                    if (parameter->isSubscribed())
                    {
                        auto entry = SingleStreamEntryFactory::create(parameter);
                        root->insert(entry);
                    }
                }
                else if (size >= 1)
                {
                    auto const first = std::begin(streams);
                    auto const last = std::end(streams);
                    auto const result = std::max_element(first, last, [](decltype(*first) max, decltype(*first) cur) -> bool
                    {
                        if (max->hasStreamDescriptor() && cur->hasStreamDescriptor())
                        {
                            return cur->streamDescriptor()->offset() > max->streamDescriptor()->offset();
                        }
                        else if (max->hasStreamDescriptor())
                        {
                            return false;
                        }
                        else if (cur->hasStreamDescriptor())
                        {
                            return true;
                        }

                        return false;
                    });

                    auto const isSubscribed = std::any_of(first, last, [](decltype(*first) stream) -> bool
                    {
                        return stream->isSubscribed();
                    });

                    if (result != last && isSubscribed)
                    {
                        auto descriptor = (*result)->streamDescriptor();
                        auto const format = descriptor->format();
                        auto const offset = descriptor->offset();
                        auto const size = offset + format.size();

                        auto buffer = std::vector<unsigned char>(size, 0x00);
                        for each(auto parameter in streams)
                        {
                            encode(parameter, std::begin(buffer), std::end(buffer));
                        }

                        root->insert(identifier, std::begin(buffer), std::end(buffer));
                    }
                }
            }
        }

        return root;
    }
}
}
