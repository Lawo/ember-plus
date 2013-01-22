#ifndef __TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H
#define __TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H

#include "../../gadget/ParameterTypeVisitor.h"
#include "../../gadget/ParameterField.h"

namespace libember { namespace glow 
{
    class GlowContainer;
    class GlowParameterBase;
    class GlowRootElementCollection;
}
}

namespace glow { namespace util
{
    class ParameterConverter : public gadget::ParameterTypeVisitorConst
    {
        public:
            static libember::glow::GlowParameterBase* create(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

            static libember::glow::GlowParameterBase* createStructured(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

            static libember::glow::GlowParameterBase* createQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

        private:
            ParameterConverter(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields, bool makeQualified);

            virtual void visit(gadget::EnumParameter const* parameter) const;

            virtual void visit(gadget::StringParameter const* parameter) const;

            virtual void visit(gadget::IntegerParameter const* parameter) const;

            virtual void visit(gadget::RealParameter const* parameter) const;

        private:
            gadget::ParameterFieldState m_fields;
            libember::glow::GlowParameterBase* m_parameter;
    };
}
}

#endif//__TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H
