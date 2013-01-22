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

