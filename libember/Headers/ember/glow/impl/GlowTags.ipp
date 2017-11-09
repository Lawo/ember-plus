/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTAGS_IPP
#define __LIBEMBER_GLOW_GLOWTAGS_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow
{
    /**
     * Global Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Root()
    {
        return ber::make_tag(ber::Class::Application, 0);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ElementDefault()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    /**
     * Command specific tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Command::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Command::DirFieldMask()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Command::Invocation()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * QualifiedNode Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedNode::Path()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedNode::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedNode::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * Node Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * NodeContent Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::NodeContents::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::Identifier);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::NodeContents::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::Description);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::NodeContents::IsRoot()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::IsRoot);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::NodeContents::IsOnline()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::IsOnline);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::NodeContents::SchemaIdentifiers()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::SchemaIdentifiers);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::NodeContents::TemplateReference()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::TemplateReference);
    }

    /**
     * QualifiedParameter Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedParameter::Path()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedParameter::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::QualifiedParameter::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * Parameter Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * ParameterContent Tags
     */

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Identifier);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Description);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Value()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Value);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Minimum()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Minimum);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Maximum()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Maximum);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Access()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Access);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Format()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Format);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Enumeration()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Enumeration);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Factor()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Factor);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::IsOnline()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::IsOnline);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Formula()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Formula);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Step()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Step);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Default()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Default);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::Type()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Type);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::ParameterContents::StreamIdentifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::StreamIdentifier);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ParameterContents::EnumMap()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::EnumMap);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ParameterContents::StreamDescriptor()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::StreamDescriptor);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ParameterContents::SchemaIdentifiers()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::SchemaIdentifiers);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ParameterContents::TemplateReference()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::TemplateReference);
    }

    /**
     * StreamEntry Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamEntry::StreamIdentifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamEntry::StreamValue()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * StringIntegerPair Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::StringIntegerPair::Name()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StringIntegerPair::Value()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * StreamDescriptor Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamDescriptor::Format()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamDescriptor::Offset()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * Matrix Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Targets()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 3);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Sources()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 4);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Matrix::Connections()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 5);
    }

    /**
     * QualifiedMatrix Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Path()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Targets()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 3);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Sources()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 4);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedMatrix::Connections()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 5);
    }

    /**
     * MatrixContents Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::Identifier);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::Description);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::Type()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::Type);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::AddressingMode()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::AddressingMode);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::TargetCount()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::TargetCount);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::SourceCount()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::SourceCount);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::MaximumTotalConnects()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::MaximumTotalConnects);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::MaximumConnectsPerTarget()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::MaximumConnectsPerTarget);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::ParametersLocation()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::ParametersLocation);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::GainParameterNumber()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::GainParameterNumber);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::Labels()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::Labels);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::SchemaIdentifiers()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::SchemaIdentifiers);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::MatrixContents::TemplateReference()
    {
        return ber::make_tag(ber::Class::ContextSpecific, MatrixProperty::TemplateReference);
    }

    /**
     * Signal Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Signal::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    /**
     * Connection Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Connection::Target()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Connection::Sources()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Connection::Operation()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Connection::Disposition()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 3);
    }

    /**
     * Label Tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Label::BasePath()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Label::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * Function tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Function::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Function::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Function::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * QualifiedFunction tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedFunction::Path()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedFunction::Contents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedFunction::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    /**
     * FunctionContents tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::FunctionContents::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::FunctionContents::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::FunctionContents::Arguments()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::FunctionContents::Result()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 3);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::FunctionContents::TemplateReference()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 4);
    }

    /**
     * TupleItemDescription tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::TupleItemDescription::Type()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::TupleItemDescription::Name()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * Invocation tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::Invocation::InvocationId()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Invocation::Arguments()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    /**
     * InvocationResult tags
     */

    LIBEMBER_INLINE
    ber::Tag GlowTags::InvocationResult::InvocationId()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::InvocationResult::Success()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
        ber::Tag GlowTags::InvocationResult::Result()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }


    LIBEMBER_INLINE
    ber::Tag GlowTags::Template::Number()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
        ber::Tag GlowTags::Template::Element()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
        ber::Tag GlowTags::Template::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }


    LIBEMBER_INLINE
    ber::Tag GlowTags::QualifiedTemplate::Path()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
        ber::Tag GlowTags::QualifiedTemplate::Element()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

    LIBEMBER_INLINE
        ber::Tag GlowTags::QualifiedTemplate::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 2);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTAGS_IPP
