/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP
#define __LIBEMBER_GLOW_GLOWNODEFACTORY_IPP

#include "../../util/Inline.hpp"
#include "../GlowCommand.hpp"
#include "../GlowElementCollection.hpp"
#include "../GlowFunction.hpp"
#include "../GlowInvocation.hpp"
#include "../GlowInvocationResult.hpp"
#include "../GlowStringIntegerCollection.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowNode.hpp"
#include "../GlowParameter.hpp"
#include "../GlowRootElementCollection.hpp"
#include "../GlowStreamCollection.hpp"
#include "../GlowStreamDescriptor.hpp"
#include "../GlowStreamEntry.hpp"
#include "../GlowTupleItemDescription.hpp"
#include "../GlowQualifiedFunction.hpp"
#include "../GlowQualifiedNode.hpp"
#include "../GlowQualifiedParameter.hpp"
#include "../GlowMatrix.hpp"
#include "../GlowQualifiedMatrix.hpp"
#include "../GlowTarget.hpp"
#include "../GlowSource.hpp"
#include "../GlowConnection.hpp"
#include "../GlowLabel.hpp"
#include "../GlowTemplate.hpp"
#include "../GlowQualifiedTemplate.hpp"

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

            case GlowType::Function:
                return new GlowFunction(tag);

            case GlowType::QualifiedFunction:
                return new GlowQualifiedFunction(tag);

            case GlowType::Invocation:
                return new GlowInvocation(tag);

            case GlowType::InvocationResult:
                return new GlowInvocationResult(tag);

            case GlowType::TupleItemDescription:
                return new GlowTupleItemDescription(tag);

            case GlowType::Template:
                return new GlowTemplate(tag);

            case GlowType::QualifiedTemplate:
                return new GlowQualifiedTemplate(tag);

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
