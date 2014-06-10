#ifndef __TINYEMBERROUTER_UTIL_TYPES_H
#define __TINYEMBERROUTER_UTIL_TYPES_H

#include <ember/glow/ConnectionOperation.hpp>
#include <ember/ber/ObjectIdentifier.hpp>
#include <ember/glow/CommandType.hpp>
#include <ember/glow/Value.hpp>
#include <ember/glow/ParameterType.hpp>


namespace util
{
   typedef libember::glow::ConnectionOperation ConnectOperation;
   typedef libember::ber::ObjectIdentifier Oid;
   typedef libember::glow::Value VariantValue;
   typedef std::vector<libember::glow::Value> VariantValueVector;
   typedef libember::glow::ParameterType VariantType;

   class TupleItem
   {
   public:
      TupleItem(VariantType const& type, std::string const& name)
         : m_type(type)
         , m_name(name)
      {
      }

      VariantType const& type() const { return m_type; }
      std::string const& name() const { return m_name; }

   private:
      VariantType m_type;
      std::string m_name;
   };
}

#endif//__TINYEMBERROUTER_UTIL_TYPES_H
