/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWMATRIXBASE_IPP
#define __LIBEMBER_GLOW_GLOWMATRIXBASE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../util/Validation.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowMatrixBase::GlowMatrixBase(
        GlowType const& type,
        ber::Tag const& tag,
        ber::Tag const& contentsTag,
        ber::Tag const& childrenTag,
        ber::Tag const& targetsTag,
        ber::Tag const& sourcesTag,
        ber::Tag const& connectionsTag)
        : GlowContentElement(type, tag, contentsTag)
        , m_childrenTag(childrenTag)
        , m_targetsTag(targetsTag)
        , m_sourcesTag(sourcesTag)
        , m_connectionsTag(connectionsTag)
    {}

    LIBEMBER_INLINE
    bool GlowMatrixBase::contains(MatrixProperty const& property) const
    {
        return contents().contains(property);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setSchemaIdentifiers(std::string const& identifiers)
    {
        contents().set(GlowTags::MatrixContents::SchemaIdentifiers(), identifiers);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setDescription(std::string const& description)
    {
        contents().set(GlowTags::MatrixContents::Description(), description);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setIdentifier(std::string const& identifier)
    {
#ifndef LIBEMBER_DISABLE_IDENTIFIER_VALIDATION
        libember::glow::util::Validation::assertIdentifierValid(identifier);
#endif//LIBEMBER_DISABLE_IDENTIFIER_VALIDATION

        contents().set(GlowTags::MatrixContents::Identifier(), identifier);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setType(MatrixType const& type)
    {
        contents().set(GlowTags::MatrixContents::Type(), type.value());
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setAddressingMode(MatrixAddressingMode const& addressingMode)
    {
        contents().set(GlowTags::MatrixContents::AddressingMode(), addressingMode.value());
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setTargetCount(int targetCount)
    {
        contents().set(GlowTags::MatrixContents::TargetCount(), targetCount);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setSourceCount(int sourceCount)
    {
        contents().set(GlowTags::MatrixContents::SourceCount(), sourceCount);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setMaximumTotalConnects(int maximumTotalConnects)
    {
        contents().set(GlowTags::MatrixContents::MaximumTotalConnects(), maximumTotalConnects);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setMaximumConnectsPerTarget(int maximumConnectsPerTarget)
    {
        contents().set(GlowTags::MatrixContents::MaximumConnectsPerTarget(), maximumConnectsPerTarget);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setParametersLocation(int inlineSubid)
    {
        contents().set(GlowTags::MatrixContents::ParametersLocation(), inlineSubid);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setParametersLocation(ber::ObjectIdentifier const& basePath)
    {
        contents().set(GlowTags::MatrixContents::ParametersLocation(), basePath);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setGainParameterNumber(int gainParameterNumber)
    {
        contents().set(GlowTags::MatrixContents::GainParameterNumber(), gainParameterNumber);
    }

    LIBEMBER_INLINE
    void GlowMatrixBase::setTemplateReference(ber::ObjectIdentifier const& path)
    {
        contents().set(GlowTags::MatrixContents::TemplateReference(), path);
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowMatrixBase::labels()
    {
        iterator const first = contents().begin();
        iterator const last = contents().end();
        iterator const result = util::find_tag(first, last, GlowTags::MatrixContents::Labels());
        if (result != last)
        {
            return dynamic_cast<Sequence*>(&*result);
        }
        else
        {
            Sequence* collection = new Sequence(GlowTags::MatrixContents::Labels());
            contents().set(collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    GlowElementCollection* GlowMatrixBase::children()
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
    dom::Sequence* GlowMatrixBase::targets()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, m_targetsTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            dom::Sequence* collection = new dom::Sequence(m_targetsTag);
            insert(last, collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowMatrixBase::sources()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, m_sourcesTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            dom::Sequence* collection = new dom::Sequence(m_sourcesTag);
            insert(last, collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    dom::Sequence* GlowMatrixBase::connections()
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, m_connectionsTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence*>(&*result);
        }
        else
        {
            dom::Sequence* collection = new dom::Sequence(m_connectionsTag);
            insert(last, collection);
            return collection;
        }
    }

    LIBEMBER_INLINE
    std::string GlowMatrixBase::description() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::Description());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowMatrixBase::identifier() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::Identifier());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    std::string GlowMatrixBase::schemaIdentifiers() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::SchemaIdentifiers());
        return util::ValueConverter::valueOf(value, std::string());
    }

    LIBEMBER_INLINE
    MatrixType GlowMatrixBase::type() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::Type());
        int const type = util::ValueConverter::valueOf(value, int(MatrixType::OneToN));
        return static_cast<MatrixType::_Domain>(type);
    }

    LIBEMBER_INLINE
    MatrixAddressingMode GlowMatrixBase::addressingMode() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::AddressingMode());
        int const type = util::ValueConverter::valueOf(value, int(MatrixAddressingMode::Linear));
        return static_cast<MatrixAddressingMode::_Domain>(type);
    }

    LIBEMBER_INLINE
    int GlowMatrixBase::targetCount() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::TargetCount());
        return util::ValueConverter::valueOf(value, 0);
    }

    LIBEMBER_INLINE
    int GlowMatrixBase::sourceCount() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::SourceCount());
        return util::ValueConverter::valueOf(value, 0);
    }

    LIBEMBER_INLINE
    int GlowMatrixBase::maximumTotalConnects() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::MaximumTotalConnects());
        return util::ValueConverter::valueOf(value, 0);
    }

    LIBEMBER_INLINE
    int GlowMatrixBase::maximumConnectsPerTarget() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::MaximumConnectsPerTarget());
        return util::ValueConverter::valueOf(value, 0);
    }

    LIBEMBER_INLINE
    ParametersLocation GlowMatrixBase::parametersLocation() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::ParametersLocation());
        return ParametersLocation(value);
    }

    LIBEMBER_INLINE
    int GlowMatrixBase::gainParameterNumber() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::GainParameterNumber());
        return util::ValueConverter::valueOf(value, -1);
    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowMatrixBase::labels() const
    {
        const_iterator const first = contents().begin();
        const_iterator const last = contents().end();
        const_iterator const result = util::find_tag(first, last, GlowTags::MatrixContents::Labels());
        if (result != last)
        {
            return dynamic_cast<Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    GlowElementCollection const* GlowMatrixBase::children() const
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
    dom::Sequence const* GlowMatrixBase::targets() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, m_targetsTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowMatrixBase::sources() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, m_sourcesTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    dom::Sequence const* GlowMatrixBase::connections() const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, m_connectionsTag);
        if (result != last)
        {
            return dynamic_cast<dom::Sequence const*>(&*result);
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowMatrixBase::templateReference() const
    {
        ber::Value const value = contents().get(GlowTags::MatrixContents::TemplateReference());
        return util::ValueConverter::valueOf(value, ber::ObjectIdentifier());
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWMATRIXBASE_IPP
