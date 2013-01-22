#include "Node.h"
#include "ElementVisitor.h"

namespace model
{
   Node::Node(int number, Element* parent, std::string const& identifier)
      : Element(number, parent, identifier)
   {}

   // override
   void Node::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }
}
