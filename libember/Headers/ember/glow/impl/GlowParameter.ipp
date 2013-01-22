#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueTypeToVariantLeaf.hpp"
#include "../util/LeafToValueType.hpp"
#include "../util/LeafToMinMax.hpp"
#include "../util/LeafToValue.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number)
        : GlowContentElement(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents())
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number, ber::Tag const& tag)
        : GlowContentElement(GlowType::Parameter, tag, GlowTags::Parameter::Contents())
    {
        insert(begin(), util::ValueTypeToVariantLeaf::create(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(ber::Tag const& tag)
        : GlowContentElement(GlowType::Parameter, tag, GlowTags::Parameter::Contents())
    {}

    LIBEMBER_INLINE
    bool GlowParameter::contains(ParameterProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowParameter::setDescription(std::string const& description)
    {
        append(GlowTags::Parameter::Description(), description);
    }

    LIBEMBER_INLINE
    void GlowParameter::setIdentifier(std::string const& identifier)
    {
        append(GlowTags::Parameter::Identifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowParameter::setFormula(std::string const& formula)
    {
        append(GlowTags::Parameter::Formula(), formula);
    }

    LIBEMBER_INLINE
    void GlowParameter::setFormat(std::string const& format)
    {
        append(GlowTags::Parameter::Format(), format);
    }

    LIBEMBER_INLINE
    void GlowParameter::setEnumeration(std::string const& enumeration)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::Parameter::Enumeration();
        dom::Node *const value = util::ValueTypeToVariantLeaf::create(tag, enumeration);

        content.insert(where, value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setMinimum(double minimum)
    {
        append(GlowTags::Parameter::Minimum(), minimum);
    }

    LIBEMBER_INLINE
    void GlowParameter::setMinimum(int minimum)
    {
        append(GlowTags::Parameter::Minimum(), minimum);
    }

    LIBEMBER_INLINE
    void GlowParameter::setMaximum(double maximum)
    {
        append(GlowTags::Parameter::Maximum(), maximum);
    }

    LIBEMBER_INLINE
    void GlowParameter::setMaximum(int maximum)
    {
        append(GlowTags::Parameter::Maximum(), maximum);
    }

    LIBEMBER_INLINE
    void GlowParameter::setFactor(int factor)
    {
        append(GlowTags::Parameter::Factor(), factor);
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(double value)
    {
        append(GlowTags::Parameter::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(int value)
    {
        append(GlowTags::Parameter::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setDefault(std::string const& value)
    {
        append(GlowTags::Parameter::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(double value)
    {
        append(GlowTags::Parameter::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(int value)
    {
        append(GlowTags::Parameter::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setValue(std::string const& value)
    {
        append(GlowTags::Parameter::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setStep(double value)
    {
        append(GlowTags::Parameter::Step(), value);
    }

    LIBEMBER_INLINE
    void GlowParameter::setIsWriteable(bool isWriteable)
    {
        append(GlowTags::Parameter::IsWriteable(), isWriteable);
    }

    LIBEMBER_INLINE
    void GlowParameter::setIsCommand(bool isCommand)
    {
        append(GlowTags::Parameter::IsCommand(), isCommand);
    }

    LIBEMBER_INLINE
    void GlowParameter::setStreamIdentifier(int identifier)
    {
        append(GlowTags::Parameter::StreamIdentifier(), identifier);
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowParameter::children()
    {
        Contents& content = contents();
        Contents::iterator const first = content.begin();
        Contents::iterator const last = content.end();
        ber::Tag const tag = GlowTags::Parameter::Children();
        dom::Node* collection = find<dom::Node>(first, last, tag);

        if (collection == 0)
        {
            collection = new GlowElementCollection(tag);
            content.insert(last, collection);
        }

        return dynamic_cast<GlowElementCollection*>(collection);
    }

    LIBEMBER_INLINE
    int GlowParameter::number() const
    {
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(begin(), end(), GlowTags::Parameter::Number());
        return util::LeafToValueType::as(leaf, -1);
    }

    LIBEMBER_INLINE
    std::string GlowParameter::description() const
    {
        return getContentValue(GlowTags::Parameter::Description(), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameter::identifier() const
    {
        return getContentValue(GlowTags::Parameter::Identifier(), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameter::formula() const
    {
        return getContentValue(GlowTags::Parameter::Formula(), std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameter::format() const
    {
        return getContentValue(GlowTags::Parameter::Format(), std::string());
    }

    LIBEMBER_INLINE
    GlowParameter::EnumerationList GlowParameter::enumeration() const
    {
        std::string enumeration = getContentValue(GlowTags::Parameter::Enumeration(), std::string());
        std::size_t first = 0;
        std::size_t last = enumeration.find('\n');
        EnumerationList list;

        while(last != std::string::npos)
        {
            list.push_back(enumeration.substr(first, (last - first)));

            first = last + 1;
            last = enumeration.find('\n', first);
        }

        if (first < enumeration.length())
        {
            list.push_back(enumeration.substr(first));
        }

        return list;
    }

    LIBEMBER_INLINE
    GlowParameter::EnumerationEntryList GlowParameter::enumMap() const
    {
        Contents const& content = contents();
        ber::Tag const tag = GlowTags::Parameter::EnumMap();
        GlowStringIntegerCollection const* enumCollection = find<GlowStringIntegerCollection>(content.begin(), content.end(), tag);

        EnumerationEntryList entries;

        if (enumCollection != 0)
        {
            GlowStringIntegerCollection::const_iterator first = enumCollection->begin();
            GlowStringIntegerCollection::const_iterator const last = enumCollection->end();
            
            for (/** Nothing */; first != last; ++first)
            {
                dom::Node const& node = *first;
                GlowStringIntegerPair const* entry = dynamic_cast<GlowStringIntegerPair const*>(&node);

                if (entry != 0)
                {
                    entries.push_back(std::make_pair(entry->name(), entry->value()));
                }
            }
        }

        return entries;
    }

    LIBEMBER_INLINE
    MinMax GlowParameter::minimum() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const& tag = GlowTags::Parameter::Minimum();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToMinMax::as(leaf);
    }

    LIBEMBER_INLINE
    MinMax GlowParameter::maximum() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const& tag = GlowTags::Parameter::Maximum();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToMinMax::as(leaf);
    }

    LIBEMBER_INLINE
    int GlowParameter::factor() const
    {
        return getContentValue(GlowTags::Parameter::Factor(), 1);
    }

    LIBEMBER_INLINE
    Value GlowParameter::defaultValue() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Parameter::Default();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValue::as(leaf);
    }

    LIBEMBER_INLINE
    Value GlowParameter::value() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Parameter::Value();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValue::as(leaf);
    }

    LIBEMBER_INLINE
    double GlowParameter::step() const
    {
        return getContentValue(GlowTags::Parameter::Step(), 1.0);
    }

    LIBEMBER_INLINE
    bool GlowParameter::isWriteable() const
    {
        return getContentValue(GlowTags::Parameter::IsWriteable(), false);
    }

    LIBEMBER_INLINE
    bool GlowParameter::isCommand() const
    {
        return getContentValue(GlowTags::Parameter::IsCommand(), false);
    }

    LIBEMBER_INLINE
    int GlowParameter::streamIdentifier() const
    {
        return getContentValue(GlowTags::Parameter::StreamIdentifier(), -1);
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowParameter::children() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        ber::Tag const tag = GlowTags::Parameter::Children();
        dom::Node const* collection = find<dom::Node>(first, last, tag);

        return dynamic_cast<GlowElementCollection const*>(collection);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_IPP
