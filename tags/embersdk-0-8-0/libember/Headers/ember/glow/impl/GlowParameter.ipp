#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"
#include "../util/LeafToMinMax.hpp"
#include "../util/LeafToValue.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number)
        : GlowContentElement(GlowType::Parameter, GlowTags::ParameterContents())
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number, ber::Tag const& tag)
        : GlowContentElement(GlowType::Parameter, GlowTags::ParameterContents(), tag)
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(ber::Tag const& tag)
        : GlowContentElement(GlowType::Parameter, tag)
    {}

    LIBEMBER_INLINE
    bool GlowParameter::contains(GlowProperty const& property) const
    {
        if (property.value == GlowProperty::Number)
        {
            return find<dom::VariantLeaf>(begin(), end(), GlowTags::Number()) != 0;
        }
        else
        {
            return contents().contains(property);
        }
    }

    LIBEMBER_INLINE
    void GlowParameter::setDescription(std::string const& description)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Description(), description));
    }

    LIBEMBER_INLINE
    void GlowParameter::setIdentifier(std::string const& identifier)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Identifier(), identifier));
    }

    LIBEMBER_INLINE
    void GlowParameter::setFormula(std::string const& formula)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Formula(), formula));
    }

    LIBEMBER_INLINE
    void GlowParameter::setMinimum(double minimum)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Minimum(), minimum));
    }

    LIBEMBER_INLINE
    void GlowParameter::setMinimum(int minimum)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Minimum(), minimum));
    }

    LIBEMBER_INLINE
    void GlowParameter::setMaximum(double maximum)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Maximum(), maximum));
    }

    LIBEMBER_INLINE
    void GlowParameter::setMaximum(int maximum)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Maximum(), maximum));
    }

    LIBEMBER_INLINE
    void GlowParameter::setFactor(int factor)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Factor(), factor));
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(double value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(int value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(std::string const& value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(double value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(int value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(std::string const& value)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameter::setIsWriteable(bool isWriteable)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::IsWriteable(), isWriteable));
    }

    LIBEMBER_INLINE
    void GlowParameter::setIsCommand(bool isCommand)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::IsCommand(), isCommand));
    }

    LIBEMBER_INLINE
    void GlowParameter::setStreamIdentifier(int identifier)
    {
        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::StreamIdentifier(), identifier));
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowParameter::children()
    {
        Contents& content = contents();
        dom::Node* collection = find<dom::Node>(content.begin(), content.end(), GlowTags::Children());

        if (collection == 0)
        {
            collection = new GlowElementCollection(GlowTags::Children());
            content.insert(content.end(), collection);
        }

        return reinterpret_cast<GlowElementCollection*>(collection);
    }

    LIBEMBER_INLINE
    int GlowParameter::number() const
    {
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(begin(), end(), GlowTags::Number());
        return util::LeafToValueType::as(leaf, -1);
    }

    LIBEMBER_INLINE
    std::string GlowParameter::description() const
    {
        Contents const& content = contents();
        return util::LeafToValueType::as(find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Description()), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameter::identifier() const
    {
        Contents const& content = contents();
        return util::LeafToValueType::as(find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Identifier()), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameter::formula() const
    {
        Contents const& content = contents();
        return util::LeafToValueType::as(find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Formula()), std::string());
    }

    LIBEMBER_INLINE
    GlowParameter::EnumerationList GlowParameter::enumeration() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Enumeration());
        std::string enumeration = util::LeafToValueType::as(leaf, std::string());

        std::size_t first = 0;
        std::size_t last = enumeration.find('\n');
        EnumerationList list;

        while(last != std::string::npos)
        {
            list.push_back(enumeration.substr(first, (last - first)));

            first = last + 1;
            last = enumeration.find('\n');
        }

        return list;
    }

    LIBEMBER_INLINE
    MinMax GlowParameter::minimum() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Minimum());
        return util::LeafToMinMax::as(leaf);
    }

    LIBEMBER_INLINE
    MinMax GlowParameter::maximum() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Maximum());
        return util::LeafToMinMax::as(leaf);
    }

    LIBEMBER_INLINE
    int GlowParameter::factor() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Factor());
        return util::LeafToValueType::as(leaf, 1);
    }

    LIBEMBER_INLINE
    Value GlowParameter::defaultValue() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Default());
        return util::LeafToValue::as(leaf);
    }

    LIBEMBER_INLINE
    Value GlowParameter::value() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::Value());
        return util::LeafToValue::as(leaf);
    }

    LIBEMBER_INLINE
    bool GlowParameter::isWriteable() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::IsWriteable());
        return util::LeafToValueType::as(leaf, false);
    }

    LIBEMBER_INLINE
    bool GlowParameter::isCommand() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::IsCommand());
        return util::LeafToValueType::as(leaf, false);
    }

    LIBEMBER_INLINE
    int GlowParameter::streamIdentifier() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::StreamIdentifier());
        return util::LeafToValueType::as(leaf, false);
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowParameter::children() const
    {
        Contents const& content = contents();
        dom::Node const* collection = find<dom::Node>(content.begin(), content.end(), GlowTags::Children());

        return reinterpret_cast<GlowElementCollection const*>(collection);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_IPP
