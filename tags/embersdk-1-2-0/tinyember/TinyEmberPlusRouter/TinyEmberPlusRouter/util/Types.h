#ifndef __TINYEMBERROUTER_UTIL_TYPES_H
#define __TINYEMBERROUTER_UTIL_TYPES_H

#include <ember/glow/ConnectionOperation.hpp>
#include <ember/ber/ObjectIdentifier.hpp>
#include <ember/glow/CommandType.hpp>

namespace util
{
   typedef libember::glow::ConnectionOperation ConnectOperation;
   typedef libember::ber::ObjectIdentifier Oid;
}

#endif//__TINYEMBERROUTER_UTIL_TYPES_H
