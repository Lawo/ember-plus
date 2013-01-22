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

#ifndef __LIBEMBER_GLOW_GLOWTAGS_IPP
#define __LIBEMBER_GLOW_GLOWTAGS_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow
{
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
    ber::Tag GlowTags::StreamEntry::StreamIdentifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamEntry::StreamValue()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 1);
    }

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

}
}

#endif  // __LIBEMBER_GLOW_GLOWTAGS_IPP
