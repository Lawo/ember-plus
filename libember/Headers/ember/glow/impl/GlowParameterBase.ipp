/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP

#include "../../util/Inline.hpp"
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
    void GlowParameterBase::setDescription(std::string const& description)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Description(), description));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setIdentifier(std::string const& identifier)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Identifier(), identifier));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFormula(std::string const& providerToConsumer, std::string const& consumerToProvider)
    {
        std::ostringstream stream;
        stream << providerToConsumer << "\n" << consumerToProvider;

        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Formula(), stream.str()));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFormat(std::string const& format)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Format(), format));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setEnumeration(std::string const& enumeration)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Enumeration(), enumeration));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMinimum(double minimum)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Minimum(), minimum));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMinimum(int minimum)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Minimum(), minimum));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMaximum(double maximum)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Maximum(), maximum));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setMaximum(int maximum)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Maximum(), maximum));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setFactor(int factor)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Factor(), factor));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(double value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(int value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setDefault(std::string const& value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Default(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(double value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(int value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setValue(std::string const& value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Value(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStep(double value)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Step(), value));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setAccess(Access const& access)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Access(), access.value()));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setType(ParameterType const& type)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::Type(), type.value()));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStreamIdentifier(int identifier)
    {
        contents().push_back(new dom::VariantLeaf(GlowTags::ParameterContents::StreamIdentifier(), identifier));
    }

    LIBEMBER_INLINE
    void GlowParameterBase::setStreamDescriptor(StreamFormat const& format, int offset)
    {
        ber::Tag const tag = GlowTags::ParameterContents::StreamDescriptor();
        GlowStreamDescriptor* descriptor = new GlowStreamDescriptor(tag, format, offset);

        contents().push_back(descriptor);
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowParameterBase::children()
    {
        iterator const first = begin();
        iterator const last = end();
        dom::Node* collection = find_node<dom::Node>(first, last, m_childrenTag);

        if (collection == 0)
        {
            collection = new GlowElementCollection(m_childrenTag);
            insert(last, collection);
        }

        return dynamic_cast<GlowElementCollection*>(collection);
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::description() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Description());

        return util::ValueConverter::toValue(leaf, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowParameterBase::identifier() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Identifier());

        return util::ValueConverter::toValue(leaf, std::string());
    }

    LIBEMBER_INLINE
    Formula GlowParameterBase::formula() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Formula());

        std::string const term = util::ValueConverter::toValue(leaf, std::string());
        std::size_t splitIndex = term.find('\n');

        if (splitIndex != std::string::npos)
        {
            std::string const providerToConsumer = term.substr(0, splitIndex);
            std::string const consumerToProvider = term.substr(splitIndex + 1);
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
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Format());

        return util::ValueConverter::toValue(leaf, std::string());
    }

    LIBEMBER_INLINE
    Enumeration GlowParameterBase::enumeration() const
    {
        Contents const& content = contents();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(content.begin(), content.end(), GlowTags::ParameterContents::Enumeration());

        std::string enumeration = util::ValueConverter::toValue(leaf, std::string());
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
        Contents const& content = contents();
        GlowStringIntegerCollection const* enumeration = find_node<GlowStringIntegerCollection>(content.begin(), content.end(), GlowTags::ParameterContents::EnumMap());
        std::list<std::pair<std::string, int> > list;

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

        return Enumeration(list.begin(), list.end());
    }

    LIBEMBER_INLINE
    MinMax GlowParameterBase::minimum() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Minimum());

        return MinMax::fromLeaf(leaf);
    }

    LIBEMBER_INLINE
    MinMax GlowParameterBase::maximum() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Maximum());

        return MinMax::fromLeaf(leaf);
    }

    LIBEMBER_INLINE
    int GlowParameterBase::factor() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Factor());

        return util::ValueConverter::toValue(leaf, 1);
    }

    LIBEMBER_INLINE
    Value GlowParameterBase::defaultValue() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Default());

        return Value::fromLeaf(leaf);
    }

    LIBEMBER_INLINE
    Value GlowParameterBase::value() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Value());

        return Value::fromLeaf(leaf);
    }

    LIBEMBER_INLINE
    double GlowParameterBase::step() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Step());

        return util::ValueConverter::toValue(leaf, 1.0);
    }

    LIBEMBER_INLINE
    Access GlowParameterBase::access() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Access());

        return static_cast<Access::_Domain>(util::ValueConverter::toValue(leaf, static_cast<int>(Access::ReadOnly)));
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
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::Type());

        return static_cast<ParameterType::_Domain>(util::ValueConverter::toValue(leaf, 1));
    }

    LIBEMBER_INLINE
    int GlowParameterBase::streamIdentifier() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find_node<dom::VariantLeaf>(first, last, GlowTags::ParameterContents::StreamIdentifier());

        return util::ValueConverter::toValue(leaf, -1);
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowParameterBase::children() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        dom::Node const* collection = find_node<dom::Node>(first, last, m_childrenTag);

        return dynamic_cast<GlowElementCollection const*>(collection);
    }

    LIBEMBER_INLINE
    GlowStreamDescriptor const* GlowParameterBase::streamDescriptor() const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        GlowStreamDescriptor const* value = find_node<GlowStreamDescriptor>(first, last, GlowTags::ParameterContents::StreamDescriptor());

        return value;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETERBASE_IPP
