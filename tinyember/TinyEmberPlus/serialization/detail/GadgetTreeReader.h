#ifndef __TINYEMBER_SERIALIZATION_GADGETTREEREADER_H
#define __TINYEMBER_SERIALIZATION_GADGETTREEREADER_H

#include "../../gadget/Node.h"
#include <ember\Ember.hpp>

namespace gadget
{
    class Parameter;
    class EnumParameter;
    class IntegerParameter;
    class RealParameter;
    class StringParameter;
}

namespace serialization 
{
    class Archive;
}

namespace serialization { namespace detail
{
    class GadgetTreeReader
    {
        friend class Archive;
        private:
            explicit GadgetTreeReader(libember::util::OctetStream& input);

            ~GadgetTreeReader();

            void iterate(libember::glow::GlowRootElementCollection* collection);

            void iterate(gadget::Node* parent, libember::glow::GlowElementCollection* collection);

            void transformBase(gadget::Parameter* param, libember::glow::GlowParameter* source) const;

            void transform(gadget::EnumParameter* param, libember::glow::GlowParameter* source) const;

            void transform(gadget::IntegerParameter* param, libember::glow::GlowParameter* source) const;

            void transform(gadget::RealParameter* param, libember::glow::GlowParameter* source) const;

            void transform(gadget::StringParameter* param, libember::glow::GlowParameter* source) const;

        private:
            gadget::Node* m_root;
    };
}
}

#endif//__TINYEMBER_SERIALIZATION_GADGETTREEREADER_H
