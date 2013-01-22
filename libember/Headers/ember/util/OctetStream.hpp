#ifndef __LIBEMBER_UTIL_OCTETSTREAM_HPP
#define __LIBEMBER_UTIL_OCTETSTREAM_HPP

#include "StreamBuffer.hpp"

namespace libember { namespace util
{
    /**
     * Specialization of the StreamBuffer template for byte streams.
     */
    typedef StreamBuffer<unsigned char> OctetStream;
}
}

#endif  // __LIBEMBER_UTIL_OCTETSTREAM_HPP

