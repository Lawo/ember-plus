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

#ifndef __LIBEMBER_GLOW_PARAMETERSLOCATION_HPP
#define __LIBEMBER_GLOW_PARAMETERSLOCATION_HPP

#include <sstream>
#include "../ber/ObjectIdentifier.hpp"
#include "util/ValueConverter.hpp"
#include "ParametersLocationKind.hpp"

namespace libember { namespace glow 
{
    /**
     * Helper class that may contain different data types (int, ObjectIdentifier).
     * This class is used for the parametersLocation property of GlowMatrixBase.
     */
    class ParametersLocation
    {
    public:
        /**
         * Initializes a new instance of ParametersLocation with a base path
         * and sets kind to ParametersLocationKind::BasePath.
         * @param basePath The base path this object currently stores.
         */
        ParametersLocation(ber::ObjectIdentifier const& basePath);

        /**
         * Initializes a new instance of ParametersLocation with an inline subid
         * and sets kind to ParametersLocationKind::Inline.
         * @param inlineSubid The inline subid this object currently stores.
         */
        ParametersLocation(int inlineSubid);

        /**
         * Initializes a new instance of ParametersLocation from a type erased ber::Value.
         * @param The type erased ber value to read the value from.
         */
        ParametersLocation(ber::Value const& value);

        /**
         * Copy Constructor
         * @param other Instance to copy the value from.
         */
        ParametersLocation(ParametersLocation const& other);

        /**
         * Destructor
         */
        virtual ~ParametersLocation();

        /**
         * Returns the kind of parameters location.
         * @return The kind of parameters location.
         */
        ParametersLocationKind const& kind() const;

        /**
         * Returns the base path if kind is ParametersLocation::BasePath.
         * @return the base path if kind is ParametersLocation::BasePath.
         * @throw std::runtime_error if kind is not ParametersLocation::BasePath.
         */
        ber::ObjectIdentifier const& basePath() const;

        /**
         * Returns the inline subid if kind is ParametersLocation::Inline.
         * @return the inline subid if kind is ParametersLocation::Inline.
         * @throw std::runtime_error if kind is not ParametersLocation::Inline.
         */
        int inlineSubid() const;

    private:
        union
        {
            ber::ObjectIdentifier* m_basePath;
            int m_inlineSubid;
        };

        ParametersLocationKind m_kind;
    };


    /******************************************************
     * Inline implementation                              *
     ******************************************************/

    inline ParametersLocation::ParametersLocation(ber::ObjectIdentifier const& basePath)
        : m_kind(ParametersLocationKind::BasePath)
    {
        m_basePath = new ber::ObjectIdentifier(basePath);
        m_kind = ParametersLocationKind::BasePath;
    }

    inline ParametersLocation::ParametersLocation(int inlineSubid)
        : m_kind(ParametersLocationKind::BasePath)
    {
        m_inlineSubid = inlineSubid;
        m_kind = ParametersLocationKind::Inline;
    }

    inline ParametersLocation::ParametersLocation(ber::Value const& value)
        : m_kind(ParametersLocationKind::None)
    {
        ber::Tag const type = value.universalTag();
        if (type.getClass() == ber::Class::Universal)
        {
            switch(type.number())
            {
                case ber::Type::Integer:
                    m_inlineSubid = util::ValueConverter::valueOf(value, 0);
                    m_kind = ParametersLocationKind::Inline;
                    return;
                case ber::Type::RelativeObject:
                    m_basePath = new ber::ObjectIdentifier(util::ValueConverter::valueOf(value, ber::ObjectIdentifier()));
                    m_kind = ParametersLocationKind::BasePath;
                    return;
            }
        }
    }

    inline ParametersLocation::ParametersLocation(ParametersLocation const& other)
        : m_kind(other.m_kind)
    {
        switch(m_kind.value())
        {
            case ParametersLocationKind::BasePath:
                if (other.m_basePath != 0)
                    m_basePath = new ber::ObjectIdentifier(*other.m_basePath);
                else
                    m_basePath = 0;
                break;

            case ParametersLocationKind::Inline:
                m_inlineSubid = other.m_inlineSubid;
                break;
        }
    }

    inline ParametersLocation::~ParametersLocation()
    {
        if (m_kind.value() == ParametersLocationKind::BasePath && m_basePath != 0)
            delete m_basePath;
    }

    inline ParametersLocationKind const& ParametersLocation::kind() const
    {
        return m_kind;
    }

    inline ber::ObjectIdentifier const& ParametersLocation::basePath() const
    {
        if (m_kind.value() != ParametersLocationKind::BasePath)
            throw std::runtime_error("Expected a universal tag. But found a different tag instead.");

        return *m_basePath;
    }

    inline int ParametersLocation::inlineSubid() const
    {
        if (m_kind.value() != ParametersLocationKind::Inline)
            throw std::runtime_error("Expected a universal tag. But found a different tag instead.");

        return m_inlineSubid;
    }
}
}

#endif // __LIBEMBER_GLOW_PARAMETERSLOCATION_HPP
