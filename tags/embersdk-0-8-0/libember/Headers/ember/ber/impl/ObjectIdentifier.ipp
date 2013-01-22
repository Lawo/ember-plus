#ifndef __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP
#define __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace ber
{
    LIBEMBER_INLINE
    ObjectIdentifier::ObjectIdentifier()
        : m_value()
    {}

    LIBEMBER_INLINE
    ObjectIdentifier::ObjectIdentifier(value_type value)
        : m_value(value)
    {}

    LIBEMBER_INLINE
    ObjectIdentifier::value_type ObjectIdentifier::value() const
    {
        return m_value;
    }

    LIBEMBER_INLINE
    ObjectIdentifier::value_type& ObjectIdentifier::value()
    {
        return m_value;
    }

    LIBEMBER_INLINE
    void ObjectIdentifier::swap(ObjectIdentifier& other)
    {
        using std::swap;
        swap(m_value, other.m_value);
    }
}
}

#endif // __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP

