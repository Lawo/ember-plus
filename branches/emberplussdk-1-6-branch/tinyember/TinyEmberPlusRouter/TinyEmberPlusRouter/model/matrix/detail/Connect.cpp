#include "Connect.h"

namespace model { namespace matrix { namespace detail
{
   bool connectOneToN(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      // only connect first source
      // do not disconnect

      auto firstSource = sources.begin();

      if(firstSource != sources.end())
      {
         target->connect(firstSource, firstSource + 1, true);
         return true;
      }

      return false;
   }

   bool connectNToN(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      // connect/disconnect all passed sources

      if(operation.value() == util::ConnectOperation::Disconnect)
         target->disconnect(sources.begin(), sources.end());
      else
         target->connect(sources.begin(), sources.end(), operation.value() == util::ConnectOperation::Absolute);

      return true;
   }
}}}
