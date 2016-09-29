/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_IMPL_SEQUENCE_IPP
#define __LIBEMBER_DOM_IMPL_SEQUENCE_IPP

#include <stdexcept>
#include "../../util/Inline.hpp"
#include "../../ber/Tag.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    Sequence::Sequence(ber::Tag tag)
        : detail::ListContainer(tag)
    {}

    LIBEMBER_INLINE
    Sequence* Sequence::clone() const
    {
        return new Sequence(*this);
    }

    LIBEMBER_INLINE
    ber::Tag Sequence::typeTagImpl() const
    {
        return ber::make_tag(ber::Class::Universal, ber::Type::Sequence);
    }

}
}

#endif  // __LIBEMBER_DOM_IMPL_SEQUENCE_IPP

