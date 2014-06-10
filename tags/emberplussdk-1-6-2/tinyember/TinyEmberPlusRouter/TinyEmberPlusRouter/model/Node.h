#ifndef __TINYEMBERROUTER_MODEL_NODE_H
#define __TINYEMBERROUTER_MODEL_NODE_H

#include "Element.h"

namespace model
{
   class ElementVisitor;

   /**
     * Simple DOM element used to group other elements.
     * Translates to EmberPlus-Glow.Node or EmberPlus-Glow.QualifiedNode.
     */
   class Node : public Element
   {
   public:
      /**
        * Creates a new instance of Node.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        */
      Node(int number, Element* parent, std::string const& identifier);

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);
   };
}
#endif//__TINYEMBERROUTER_MODEL_NODE_H
