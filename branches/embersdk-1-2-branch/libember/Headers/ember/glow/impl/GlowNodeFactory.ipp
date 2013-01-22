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

#ifndef __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP
#define __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP

#include "../../util/Inline.hpp"
#include "../GlowCommand.hpp"
#include "../GlowElementCollection.hpp"
#include "../GlowStringIntegerCollection.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowNode.hpp"
#include "../GlowParameter.hpp"
#include "../GlowRootElementCollection.hpp"
#include "../GlowStreamCollection.hpp"
#include "../GlowStreamDescriptor.hpp"
#include "../GlowStreamEntry.hpp"
#include "../GlowQualifiedNode.hpp"
#include "../GlowQualifiedParameter.hpp"
#include "../GlowMatrix.hpp"
#include "../GlowQualifiedMatrix.hpp"
#include "../GlowTarget.hpp"
#include "../GlowSource.hpp"
#include "../GlowConnection.hpp"
#include "../GlowLabel.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    dom::NodeFactory& GlowNodeFactory::getFactory()
    {
        static GlowNodeFactory instance;
        return instance;
    }

    LIBEMBER_INLINE
    dom::Node* GlowNodeFactory::createApplicationDefinedNode(ber::Type const& type, ber::Tag const& tag) const
    {
        switch(type.value())
        {
            case GlowType::Command:
                return new GlowCommand(tag);

            case GlowType::ElementCollection:
                return new GlowElementCollection(tag);

            case GlowType::RootElementCollection:
                return new GlowRootElementCollection(tag);

            case GlowType::StringIntegerCollection:
                return new GlowStringIntegerCollection(tag);

            case GlowType::StringIntegerPair:
                return new GlowStringIntegerPair(tag);

            case GlowType::Node:
                return new GlowNode(tag);

            case GlowType::QualifiedNode:
                return new GlowQualifiedNode(tag);

            case GlowType::Parameter:
                return new GlowParameter(tag);

            case GlowType::QualifiedParameter:
                return new GlowQualifiedParameter(tag);

            case GlowType::StreamCollection:
                return new GlowStreamCollection(tag);

            case GlowType::StreamEntry:
                return new GlowStreamEntry(tag);

            case GlowType::StreamDescriptor:
                return new GlowStreamDescriptor(tag);

            case GlowType::Matrix:
                return new GlowMatrix(tag);

            case GlowType::QualifiedMatrix:
                return new GlowQualifiedMatrix(tag);

            case GlowType::Target:
                return new GlowTarget(tag);

            case GlowType::Source:
                return new GlowSource(tag);

            case GlowType::Connection:
                return new GlowConnection(tag);

            case GlowType::Label:
                return new GlowLabel(tag);

            default:
                return 0;
        }
    }

    LIBEMBER_INLINE
    GlowNodeFactory::GlowNodeFactory()
    {}
}
}

#endif  // __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP
