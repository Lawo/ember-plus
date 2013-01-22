#include "OneToNLinearMatrix.h"
#include "../ElementVisitor.h"
#include "detail/Connect.h"

namespace model { namespace matrix
{
   OneToNLinearMatrix::OneToNLinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount)
      : LinearMatrix(number, parent, identifier, notificationSink, targetCount, sourceCount)
   {
   }

   void OneToNLinearMatrix::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }

   bool OneToNLinearMatrix::connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      return detail::connectOneToN(target, sources, state, operation);
   }
}}
