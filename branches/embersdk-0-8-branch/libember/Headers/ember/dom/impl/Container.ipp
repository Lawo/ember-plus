#ifndef __LIBEMBER_DOM_IMPL_CONTAINER_IPP
#define __LIBEMBER_DOM_IMPL_CONTAINER_IPP

#include <stdexcept>
#include "../../util/Inline.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    Container::Container(ber::Tag tag)
        : Node(tag)
    {}

    LIBEMBER_INLINE
    Container::iterator Container::insert(iterator where, Node* child)
    {
        if (child->parent() != 0)
        {
            throw std::runtime_error("Attempt to add a node already owned by a container node.");
        }
        iterator const result = insertImpl(where, child);
        result->setParent(this);
        markDirty();
        return result;
    }

    LIBEMBER_INLINE
    void Container::fixParent(Node* child)
    {
        if (child->parent() != 0)
        {
            throw std::runtime_error("Attempt to reset parent of an already owned child node.");
        }
        child->setParent(this);
    }

    LIBEMBER_INLINE
    void Container::clear()
    {
        erase(begin(), end());
    }

    LIBEMBER_INLINE
    void Container::erase(iterator i)
    {
        iterator const first = i++;
        iterator const last  = i;
        erase(first, last);
    }

    LIBEMBER_INLINE
    void Container::erase(iterator first, iterator last)
    {
        size_type const oldSize = size();
        try
        {
            eraseImpl(first, last);
        }
        catch (...)
        {
            size_type const newSize = size();
            if (newSize != oldSize)
            {
                markDirty();
            }
            throw;
        }
        markDirty();
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_CONTAINER_IPP

