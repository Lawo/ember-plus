/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP

#include <sstream>
#include "../../util/Inline.hpp"
#include "../util/Validation.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowStreamDescriptor.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowParameterBase::GlowParameterBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag)
        : GlowContentElement(type, tag, contentsTag)
        , m_childrenTag(childrenTag)
    {
    }

    LIBEMBER_INLINE
    bool GlowParameterBase::contains(ParameterProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setSchemaIdentifiers(std::string const& identifiers)
    {
        contents().set(GlowTags::ParameterContents::SchemaIdentifiers(), identifiers);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDescription(std::string const& description)
    {
        contents().set(GlowTags::ParameterContents::Description(), description);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setIdentifier(std::string const& identifier)
    {
#ifndef LIBEMBER_DISABLE_IDENTIFIER_VALIDATION
        libember::glow::util::Validation::assertIdentifierValid(identifier);
#endif//LIBEMBER_DISABLE_IDENTIFIER_VALIDATION

        contents().set(GlowTags::ParameterContents::Identifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFormula(std::string const& providerToConsumer, std::string const& consumerToProvider)
    {
        std::ostringstream stream;
        stream << providerToConsumer << "\n" << consumerToProvider;

        contents().set(GlowTags::ParameterContents::Formula(), stream.str());
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFormat(std::string const& format)
    {
        contents().set(GlowTags::ParameterContents::Format(), format);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setEnumeration(std::string const& enumeration)
    {
        contents().set(GlowTags::ParameterContents::Enumeration(), enumeration);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMinimum(double minimum)
    {
        contents().set(GlowTags::ParameterContents::Minimum(), minimum);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMinimum(long minimum)
    {
        contents().set(GlowTags::ParameterContents::Minimum(), minimum);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMaximum(double maximum)
    {
        contents().set(GlowTags::ParameterContents::Maximum(), maximum);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMaximum(long maximum)
    {
        contents().set(GlowTags::ParameterContents::Maximum(), maximum);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFactor(int factor)
    {
        contents().set(GlowTags::ParameterContents::Factor(), factor);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(double value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(long value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(std::string const& value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(bool value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(ber::Octets const& value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(ber::Null const& value)
    {
        contents().set(GlowTags::ParameterContents::Default(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(double value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(long value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(std::string const& value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(bool value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(ber::Octets const& value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(ber::Null const& value)
    {
        contents().set(GlowTags::ParameterContents::Value(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStep(int value)
    {
        contents().set(GlowTags::ParameterContents::Step(), value);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setAccess(Access const& access)
    {
        contents().set(GlowTags::ParameterContents::Access(), access.value());
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setType(ParameterType const& type)
    {
        contents().set(GlowTags::ParameterContents::Type(), type.value());
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStreamIdentifier(int identifier)
    {
        contents().set(GlowTags::ParameterContents::StreamIdentifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStreamDescriptor(StreamFormat const& format, int offset)
    {
        ber::Tag const tag = GlowTags::ParameterContents::StreamDescriptor();
        contents().set(new GlowStreamDescriptor(tag, format, offset));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setIsOnline(bool isOnline)
    {
        contents().set(GlowTags::ParameterContents::IsOnline(), isOnline);
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setTemplateReference(ber::ObjectIdentifier const& path)
    {
        contents().set(GlowTags::ParameterContents::TemplateReference(), path);
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowParameterBase::children()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, m_childrenTag);
        if (result != last)
        {
            return dynamic_cast<GlowElementCollection*>(&*result);
        }
        else
        {
            GlowElementCollection* collection = new GlowElementCollection(m_childrenTag);
            insert(last, collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::description() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Description());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::identifier() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Identifier());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::schemaIdentifiers() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::SchemaIdentifiers());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    Formula GlowParameterBase::formula() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Formula());
        std::string const term = util::ValueConverter::valueOf(value, std::string());
        std::size_t const index = term.find('\n');

        if (index != std::string::npos)
        {
            std::string const providerToConsumer = term.substr(0, index);
            std::string const consumerToProvider = term.substr(index + 1);
            return Formula(providerToConsumer, consumerToProvider);
        }
        else
        {
            return Formula(term, term);
        }
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::format() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Format());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    Enumeration GlowParameterBase::enumeration() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Enumeration());
        std::string const enumeration = util::ValueConverter::valueOf(value, std::string());
        std::size_t first = 0;
        std::size_t last = enumeration.find('\n');
        std::list<std::string> list;

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

        return Enumeration(list.begin(), list.end());
    }

    LIBEMBER_INLINE
    Enumeration GlowParameterBase::enumerationMap() const
    {
        Contents::const_iterator const result = util::find_tag(contents().begin(), contents().end(), GlowTags::ParameterContents::EnumMap());
        std::list<std::pair<std::string, int> > list;

        if (result != contents().end())
        {
            GlowStringIntegerCollection const* enumeration = dynamic_cast<GlowStringIntegerCollection const*>(&*result);

            if (enumeration != 0)
            {
                GlowStringIntegerCollection::const_iterator first = enumeration->begin();
                GlowStringIntegerCollection::const_iterator const last = enumeration->end();
            
                for (/** Nothing */; first != last; ++first)
                {
                    dom::Node const& node = *first;
                    GlowStringIntegerPair const* entry = dynamic_cast<GlowStringIntegerPair const*>(&node);

                    if (entry != 0)
                    {
                        list.push_back(std::make_pair(entry->name(), entry->value()));
                    }
                }
            }
        }

        return Enumeration(list.begin(), list.end());
    }

    LIBEMBER_INLINE
    MinMax GlowParameterBase::minimum() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Minimum());
        return value ? MinMax(value) : MinMax();
    }

    LIBEMBER_INLINE
    MinMax GlowParameterBase::maximum() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Maximum());
        return value ? MinMax(value) : MinMax();
    }

    LIBEMBER_INLINE
    int GlowParameterBase::factor() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Factor());
        return util::ValueConverter::valueOf(value, int(1));
    }

    LIBEMBER_INLINE
    Value GlowParameterBase::defaultValue() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Default());
        return value ? Value(value) : Value();
    }

    LIBEMBER_INLINE
    Value GlowParameterBase::value() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Value());
        return value ? Value(value) : Value();
    }

    LIBEMBER_INLINE
    int GlowParameterBase::step() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Step());
        return util::ValueConverter::valueOf(value, int(1));
    }

    LIBEMBER_INLINE
    Access GlowParameterBase::access() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Access());
        int const access = util::ValueConverter::valueOf(value, int(Access::ReadOnly));
        return static_cast<Access::_Domain>(access);
    }

    LIBEMBER_INLINE
    bool GlowParameterBase::isWriteable() const
    {
        Access const access = this->access();
        return access.value() == Access::WriteOnly || access.value() == Access::ReadWrite;
    }

    LIBEMBER_INLINE
    ParameterType GlowParameterBase::type() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::Type());
        int const type = util::ValueConverter::valueOf(value, int(ParameterType::None));
        return static_cast<ParameterType::_Domain>(type);
    }

    LIBEMBER_INLINE
    ParameterType GlowParameterBase::effectiveType() const
    {
        ParameterType const embeddedType = type();
        if (embeddedType.value() == ParameterType::Trigger)
            return embeddedType;

        if (hasEnumeration())
            return ParameterType::Enum;

        Value const embeddedValue = value();

        if (embeddedValue.type().value() != ParameterType::None)
            return embeddedValue.type();

        return embeddedType;
    }

    LIBEMBER_INLINE
    bool GlowParameterBase::hasEnumeration() const
    {
        return contains(ParameterProperty::Enumeration) || contains(ParameterProperty::EnumMap);
    }

    LIBEMBER_INLINE
    int GlowParameterBase::streamIdentifier() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::StreamIdentifier());
        return util::ValueConverter::valueOf(value, int(-1));
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowParameterBase::children() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, m_childrenTag);
        if (result != last)
        {
            return dynamic_cast<GlowElementCollection const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    GlowStreamDescriptor const* GlowParameterBase::streamDescriptor() const
    {
        Contents::const_iterator const first = contents().begin();
        Contents::const_iterator const last = contents().end();
        Contents::const_iterator const result = util::find_tag(first, last, GlowTags::ParameterContents::StreamDescriptor());
        if (result != last)
        {
            return dynamic_cast<GlowStreamDescriptor const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    bool GlowParameterBase::isOnline() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::IsOnline());
        return util::ValueConverter::valueOf(value, true);
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowParameterBase::templateReference() const
    {
        ber::Value const value = contents().get(GlowTags::ParameterContents::TemplateReference());
        return util::ValueConverter::valueOf(value, ber::ObjectIdentifier());
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP
