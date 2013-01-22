#ifndef __TINYEMBER_GLOW_UTIL_NODECONVERTER_H
#define __TINYEMBER_GLOW_UTIL_NODECONVERTER_H

#include "../../gadget/Node.h"

namespace libember { namespace glow 
{
    class GlowContainer;
    class GlowNodeBase;
    class GlowRootElementCollection;
}
}


namespace glow { namespace util 
{
    class NodeConverter
    {
        public:
            static libember::glow::GlowNodeBase* create(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

            static libember::glow::GlowNodeBase* createStructured(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

            static libember::glow::GlowNodeBase* createQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

        private:
            NodeConverter(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields, bool makeQualified);

            libember::glow::GlowNodeBase* m_node;
    };
}
}

#endif//__TINYEMBER_GLOW_UTIL_NODECONVERTER_H
