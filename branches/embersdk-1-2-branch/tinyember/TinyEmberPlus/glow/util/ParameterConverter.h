#ifndef __TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H
#define __TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H

#include "../../gadget/ParameterTypeVisitor.h"
#include "../../gadget/ParameterField.h"

/** Forward declaration */
namespace libember { namespace glow 
{
    class GlowContainer;
    class GlowParameterBase;
    class GlowRootElementCollection;
}
}

namespace glow { namespace util
{
    /**
     * This class provides methods to transform a gadget parameter into a GlowParameter.
     */
    class ParameterConverter : public gadget::ParameterTypeVisitorConst
    {
        public:
            /**
             * Transforms a single parameter into a GlowParameter and appends it to the passed parent.
             * @param parent The parent container to attach the parameter to.
             * @param parameter The parameter to transform.
             * @param fields Flags indicating which parameter properties to include in the conversion.
             * @return The created glow parameter.
             */
            static libember::glow::GlowParameterBase* create(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

            /**
             * Transforms a parameter into a GlowParameter and includes all parent nodes required to reach the parameter from the
             * root node. This method assumes that the passed parameter is the only item that is being transmitted, so it creates
             * the partial tree without performing any checks whether a node already exists or not.
             * @param root The root element collection to append the parent nodes to.
             * @param parameter The parameter to transform.
             * @param fields Flags indicating which parameter properties to include in the conversion.
             * @return The created glow parameter.
             */
            static libember::glow::GlowParameterBase* createStructured(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

            /**
             * Transforms a parameter into a QualifiedParameter and appends it to the passed root element collection.
             * @param parent The parent container to attach the parameter to.
             * @param parameter The parameter to transform.
             * @param fields Flags indicating which parameter properties to include in the conversion.
             * @return The created glow parameter.
             */
            static libember::glow::GlowParameterBase* createQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields = gadget::ParameterField::None);

        private:
            /**
             * Initializes a ParameterConverter which transforms a single parameter.
             * @param parent The container to add the transformed parameter to.
             * @param parameter The parameter to transform.
             * @param fields Flags indicating which parameter properties to include in the conversion.
             * @param makeQualified If set to true, a QualifiedParameter will be created. Otherwise, the default GlowParameter
             *      will be used.
             */
            ParameterConverter(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields, bool makeQualified);

            /**
             * Transforms an enumeration.
             * @param parameter The parameter to transform.
             */
            virtual void visit(gadget::EnumParameter const* parameter) const;

            /**
             * Transforms a string.
             * @param parameter The parameter to transform.
             */
            virtual void visit(gadget::StringParameter const* parameter) const;

            /**
             * Transforms an integer parameter.
             * @param parameter The parameter to transform.
             */
            virtual void visit(gadget::IntegerParameter const* parameter) const;

            /**
             * Transforms a real parameter.
             * @param parameter The parameter to transform.
             */
            virtual void visit(gadget::RealParameter const* parameter) const;

            /**
             * Transforms a boolean parameter.
             * @param parameter The parameter to transform.
             */
            virtual void visit(gadget::BooleanParameter const* parameter) const;

        private:
            gadget::ParameterFieldState m_fields;
            libember::glow::GlowParameterBase* m_parameter;
    };
}
}

#endif//__TINYEMBER_GLOW_UTIL_PARAMETERCONVERTER_H
