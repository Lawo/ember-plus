#ifndef __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP
#define __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP

#include "../../util/Inline.hpp"
#include "../GlowCommand.hpp"
#include "../GlowElementCollection.hpp"
#include "../GlowNode.hpp"
#include "../GlowParameter.hpp"
#include "../GlowStreamCollection.hpp"
#include "../GlowStreamEntry.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    dom::NodeFactory& GlowNodeFactory::getFactory()
    {
        static GlowNodeFactory instance;
        return instance;
    }

    LIBEMBER_INLINE
    dom::Node* GlowNodeFactory::createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const
    {
        switch(type.value())
        {
            case GlowType::Command:
                return new GlowCommand(tag);

            case GlowType::ElementCollection:
                return new GlowElementCollection(tag);

            case GlowType::Node:
                return new GlowNode(tag);

            case GlowType::Parameter:
                return new GlowParameter(tag);

            case GlowType::StreamCollection:
                return new GlowStreamCollection(tag);

            case GlowType::StreamEntry:
                return new GlowStreamEntry(tag);

            default:
                return 0;
        }
    }

    LIBEMBER_INLINE
    GlowNodeFactory::GlowNodeFactory()
    {}
}
}

#endif  // __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP
