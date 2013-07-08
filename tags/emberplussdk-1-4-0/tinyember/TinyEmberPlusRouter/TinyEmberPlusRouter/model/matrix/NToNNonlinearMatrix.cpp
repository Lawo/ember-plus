#include "../ElementVisitor.h"
#include "NToNNonlinearMatrix.h"
#include "detail/Connect.h"

namespace model { namespace matrix
{
   void NToNNonlinearMatrix::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }

   bool NToNNonlinearMatrix::connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      return detail::connectNToN(target, sources, state, operation);
   }
}}
