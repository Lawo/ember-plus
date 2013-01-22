#ifndef __LIBEMBER_GLOW_GLOWTAGS_IPP
#define __LIBEMBER_GLOW_GLOWTAGS_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow
{
    const ber::Tag GlowTags::Number             = ber::make_tag(ber::Class::Application, GlowProperty::Number);
    const ber::Tag GlowTags::Identifier         = ber::make_tag(ber::Class::Application, GlowProperty::Identifier);
    const ber::Tag GlowTags::Description        = ber::make_tag(ber::Class::Application, GlowProperty::Description);
    const ber::Tag GlowTags::Value              = ber::make_tag(ber::Class::Application, GlowProperty::Value);
    const ber::Tag GlowTags::Minimum            = ber::make_tag(ber::Class::Application, GlowProperty::Minimum);
    const ber::Tag GlowTags::Maximum            = ber::make_tag(ber::Class::Application, GlowProperty::Maximum);
    const ber::Tag GlowTags::IsWriteable        = ber::make_tag(ber::Class::Application, GlowProperty::IsWriteable);
    const ber::Tag GlowTags::Format             = ber::make_tag(ber::Class::Application, GlowProperty::Format);
    const ber::Tag GlowTags::Enumeration        = ber::make_tag(ber::Class::Application, GlowProperty::Enumeration);
    const ber::Tag GlowTags::Factor             = ber::make_tag(ber::Class::Application, GlowProperty::Factor);
    const ber::Tag GlowTags::IsOnline           = ber::make_tag(ber::Class::Application, GlowProperty::IsOnline);
    const ber::Tag GlowTags::Formula            = ber::make_tag(ber::Class::Application, GlowProperty::Formula);
    const ber::Tag GlowTags::Step               = ber::make_tag(ber::Class::Application, GlowProperty::Step);
    const ber::Tag GlowTags::Default            = ber::make_tag(ber::Class::Application, GlowProperty::Default);
    const ber::Tag GlowTags::IsCommand          = ber::make_tag(ber::Class::Application, GlowProperty::IsCommand);
    const ber::Tag GlowTags::StreamIdentifier   = ber::make_tag(ber::Class::Application, GlowProperty::StreamIdentifier);
    const ber::Tag GlowTags::Children           = ber::make_tag(ber::Class::Application, GlowProperty::Children);

    const ber::Tag GlowTags::Root               = ber::make_tag(ber::Class::Application, 30);

    const ber::Tag GlowTags::ElementDefault     = ber::make_tag(ber::Class::ContextSpecific, 0);
    const ber::Tag GlowTags::NodeContents       = ber::make_tag(ber::Class::ContextSpecific, 0);
    const ber::Tag GlowTags::ParameterContents  = ber::make_tag(ber::Class::ContextSpecific, 0);
    const ber::Tag GlowTags::StreamValue        = ber::make_tag(ber::Class::ContextSpecific, 0);
}
}

#endif  // __LIBEMBER_GLOW_GLOWTAGS_IPP
