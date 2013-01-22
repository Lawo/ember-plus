#ifndef __LIBEMBER_GLOW_GLOWTAGS_IPP
#define __LIBEMBER_GLOW_GLOWTAGS_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow
{
    LIBEMBER_INLINE
    ber::Tag GlowTags::Number()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Number);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Identifier()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Identifier);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Description()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Description);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Value()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Value);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Minimum()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Minimum);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Maximum()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Maximum);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::IsWriteable()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::IsWriteable);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Format()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Format);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Enumeration()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Enumeration);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Factor()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Factor);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::IsOnline()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::IsOnline);
    }
    
    LIBEMBER_INLINE
    ber::Tag GlowTags::Formula()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Formula);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Step()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Step);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Default()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Default);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::IsCommand()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::IsCommand);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamIdentifier()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::StreamIdentifier);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Children()
    {
        return ber::make_tag(ber::Class::Application, GlowProperty::Children);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::Root()
    {
        return ber::make_tag(ber::Class::Application, 30);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ElementDefault()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::NodeContents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::ParameterContents()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }

    LIBEMBER_INLINE
    ber::Tag GlowTags::StreamValue()
    {
        return ber::make_tag(ber::Class::ContextSpecific, 0);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTAGS_IPP
