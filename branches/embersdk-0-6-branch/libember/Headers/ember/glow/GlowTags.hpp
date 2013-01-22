#ifndef __LIBEMBER_GLOW_GLOWTAGS_HPP
#define __LIBEMBER_GLOW_GLOWTAGS_HPP

#include "../util/Api.hpp"
#include "GlowProperty.hpp"

namespace libember { namespace glow
{ 
    /**
     * Lists all application specific tags that are used by glow to store properties of 
     * nodes, parameters and other object types.
     */
    struct LIBEMBER_API GlowTags
    {
        static const ber::Tag Number; 
        static const ber::Tag Identifier;
        static const ber::Tag Description;
        static const ber::Tag Value;
        static const ber::Tag Minimum;
        static const ber::Tag Maximum;
        static const ber::Tag IsWriteable;
        static const ber::Tag Format;
        static const ber::Tag Enumeration;
        static const ber::Tag Factor;
        static const ber::Tag IsOnline;
        static const ber::Tag Formula;
        static const ber::Tag Step;
        static const ber::Tag Default;
        static const ber::Tag IsCommand;
        static const ber::Tag StreamIdentifier;
        static const ber::Tag Children;

        static const ber::Tag Root;
        static const ber::Tag ElementDefault;
        static const ber::Tag NodeContents;
        static const ber::Tag ParameterContents;
        static const ber::Tag StreamValue;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowTags.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTAGS_HPP

