#include "Function.h"
#include "ElementVisitor.h"

namespace model
{
   Function::~Function()
   {
      if(m_delegate != nullptr)
         delete m_delegate;
   }

   // override
   void Function::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }
}
