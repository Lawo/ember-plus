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
    ber::Tag GlowTags::Node::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::Identifier);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::Description);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::Children);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Node::IsRoot()
    {
        return ber::make_tag(ber::Class::ContextSpecific, NodeProperty::IsRoot);
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
    ber::Tag GlowTags::Parameter::Identifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Identifier);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Description()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Description);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Value()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Value);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Minimum()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Minimum);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Maximum()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Maximum);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::IsWriteable()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::IsWriteable);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Format()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Format);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Enumeration()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Enumeration);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Factor()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Factor);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::IsOnline()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::IsOnline);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Formula()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Formula);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Step()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Step);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Default()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Default);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::IsCommand()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::IsCommand);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::StreamIdentifier()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::StreamIdentifier);
    }

    LIBEMBER_INLINE 
    ber::Tag GlowTags::Parameter::Children()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::Children);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Parameter::EnumMap()
    {
        return ber::make_tag(ber::Class::ContextSpecific, ParameterProperty::EnumMap);
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
}
}

#endif  // __LIBEMBER_GLOW_GLOWTAGS_IPP
