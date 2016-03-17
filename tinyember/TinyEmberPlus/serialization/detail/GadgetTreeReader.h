#ifndef __TINYEMBER_SERIALIZATION_GADGETTREEREADER_H
#define __TINYEMBER_SERIALIZATION_GADGETTREEREADER_H

#include "../../gadget/Node.h"
#include <ember/Ember.hpp>

/** Forward declarations */
namespace gadget
{
    class Parameter;
    class BooleanParameter;
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
    /**
     * This reader class loads a ber encoded dom and creates a glow gadget tree.
     */
    class GadgetTreeReader
    {
        friend class serialization::Archive;
        private:
            /**
             * Initializes a new reader.
             * @param input The input stream containing the encoded data.
             */
            explicit GadgetTreeReader(libember::util::OctetStream& input);

            /** Destructor */
            ~GadgetTreeReader();

            /**
             * Loops through the children of the root element collection.
             * @param collection The root element collection.
             */
            void iterate(libember::glow::GlowRootElementCollection* collection);

            /**
             * Traverses the children stored in the element collection.
             * @param parent The gadget node where the children shall be appended to.
             */
            void iterate(gadget::Node* parent, libember::glow::GlowElementCollection* collection);

            /**
             * Reads all common parameter properties from the glow parameter and assigns them to the passed
             * parameter.
             * @param param The parameter to assign the glow parameter's properties to.
             * @param source The glow parameter to read the available properties from.
             */
            void transformBase(gadget::Parameter* param, libember::glow::GlowParameter* source) const;

            /**
             * Copies the enumeration parameter properties.
             * @param param the gadget parameter to assign the properties to.
             * @param source The glow parameter to read the properties from.
             */
            void transform(gadget::EnumParameter* param, libember::glow::GlowParameter* source) const;

            /**
             * Copies the integer parameter properties.
             * @param param the gadget parameter to assign the properties to.
             * @param source The glow parameter to read the properties from.
             */
            void transform(gadget::IntegerParameter* param, libember::glow::GlowParameter* source) const;

            /**
             * Copies the real parameter properties.
             * @param param the gadget parameter to assign the properties to.
             * @param source The glow parameter to read the properties from.
             */
            void transform(gadget::RealParameter* param, libember::glow::GlowParameter* source) const;

            /**
             * Copies the string parameter properties.
             * @param param the gadget parameter to assign the properties to.
             * @param source The glow parameter to read the properties from.
             */
            void transform(gadget::StringParameter* param, libember::glow::GlowParameter* source) const;

            /**
             * Copies the boolean parameter properties.
             * @param param the gadget parameter to assign the properties to.
             * @param source The glow parameter to read the properties from.
             */
            void transform(gadget::BooleanParameter* param, libember::glow::GlowParameter* source) const;

        private:
            gadget::Node* m_root;
    };
}
}

#endif//__TINYEMBER_SERIALIZATION_GADGETTREEREADER_H
