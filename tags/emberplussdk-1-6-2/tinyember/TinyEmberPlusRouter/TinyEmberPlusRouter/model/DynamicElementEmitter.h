#ifndef __TINYEMBERROUTER_MODEL_DYNAMICTELEMENTEMITTER_H
#define __TINYEMBERROUTER_MODEL_DYNAMICTELEMENTEMITTER_H

#include "../util/Types.h"

namespace model
{
   class Element;

   /**
     * Interface an Element may implement to emit descendant Elements
     * on-the-fly.
     */
   class DynamicElementEmitter
   {
   public:
      /**
        * Creates a new descendant Element representing the parameter
        * or node at the passed path.
        * @return A new instance of a class derived from Element,
        *    which must be freed by the caller.
        */
      virtual Element* emitDescendant(util::Oid const& path) = 0;
   };
}

#endif//__TINYEMBERROUTER_MODEL_DYNAMICTELEMENTEMITTER_H
