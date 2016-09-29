/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_CONTAINER_HPP
#define __LIBEMBER_DOM_CONTAINER_HPP

#include <list>
#include "Node.hpp"
#include "../util/TypeErasedIterator.hpp"

namespace libember { namespace dom
{
    /**
     * Base class for all container nodes.
     */
    class LIBEMBER_API Container : public Node
    {
        public:
            typedef util::TypeErasedIterator<Node>          iterator;
            typedef util::TypeErasedIterator<Node const>    const_iterator;
            typedef std::size_t                             size_type;

        public:
            /**
             * Return whether or not this container node is currently empty.
             * @return True if this container node is currently empty,
             *      otherwise false.
             */
            virtual bool empty() const = 0;

            /**
             * Return the number of child nodes contained within this
             * container node.
             * @return The number of child nodes contained within this
             *      container node.
             */
            virtual size_type size() const = 0;

            /**
             * Return an iterator referring to the first child node contained
             * within this container node.
             * @return An iterator referring to the first child node contained
             *      within this container node.
             */
            virtual iterator begin() = 0;

            /**
             * Return a const iterator referring to the first child node
             * contained within this container node.
             * @return A const iterator referring to the first child node
             *      contained within this container node.
             */
            virtual const_iterator begin() const = 0;

            /**
             * Return an iterator referring to the element one past the last
             * element in the sequence of child nodes contained within this
             * container node.
             * @return An iterator referring to the element one past the last
             *      element in the sequence of child nodes contained within
             *      this container node.
             */
            virtual iterator end() = 0;

            /**
             * Return a const iterator referring to the element one past the
             * last element in the sequence of child nodes contained within this
             * container node.
             * @return A const iterator referring to the element one past the
             *      last element in the sequence of child nodes contained within
             *      this container node.
             */
            virtual const_iterator end() const = 0;

            /**
             * Try to insert the node referred to by @p child to the sequence of
             * child nodes at the position referred to by @p where. If successful,
             * This operation transfers ownership of @p node to this container and
             * therefor @p node must not be deleted by means other than removing it
             * from this container.
             * @param where an iterator referring to a position where the child
             *      should be inserted.
             * @param child a pointer referring to the node that should be
             *      inserted.
             * @return An iterator referring to the child node.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow inserting elements or
             *      inserting the specific element failed for some reason.
             *      In this case the container does not assume ownership of @p node
             *      and it's the callers responsibility to properly free the node.
             * @note Depending on the specific container type the iterator passed
             *      in @p where merely serves as a hint to the insert routine and
             *      may not be considered a binding requirement.
             * 
             */
            iterator insert(iterator const& where, Node* child);

            /**
             * Clear this container node by removing all child nodes contained
             * within this node.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow the container to be
             *      cleared, or clearing the container failed for some reason.
             */
            void clear();

            /**
             * Erase the child node referred to by @p i from this container.
             * @param i an iterator referring to the child node that should be
             *      erased.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow the element to be erased, or
             *      erasing the node failed for some reason.
             */
            void erase(iterator const& i);

            /**
             * Erase the range of child nodes referred to by @p first and
             * @p last from this container.
             * @param first an iterator referring to the first element in the
             *      sequence of child nodes that should be erased.
             * @param last an iterator referring to the element one past the
             *      last element of the sequence of child nodes that should be
             *      erased.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow the elements to be erased,
             *      or erasing a node failed for some reason.
             */
            void erase(iterator const& first, iterator const& last);

        protected:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit Container(ber::Tag tag);

            /**
             * Helper that ensures that the parent setting of the (presumed) child
             * node is set to this container.
             * This method allows derived classes, to add child nodes to
             * themselves, without:
             * 1. Having to go through the (indirectly) virtual insert() method
             *    which may not safely be called within a constructor.
             * 2. Having to be befriended with Node to be able to call
             *    Node::setParent() directly.
             * @param child a pointer to the (presumed) child node whose parent
             *      pointer should be set. Must not be 0.
             * @throw std::runtime_error if the node passed in @p child already
             *      has a non-null parent set.
             */
            void fixParent(Node* child);

            /**
             * Try to insert the node referred to by @p child to the sequence of
             * child nodes at the position referred to by @p where. If successful,
             * This operation transfers ownership of @p node to this container
             * and therefor @p node must not be deleted by means other than
             * removing it from this container.
             * @param where an iterator referring to a position where the child
             *      should be inserted.
             * @param child a pointer referring to the node that should be
             *      inserted.
             * @return An iterator referring to the child node.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow inserting elements or
             *      inserting the specific element failed for some reason.
             *      In this case the container does not assume ownership of @p node
             *      and it's the callers responsibility to properly free the node.
             * @note Depending on the specific container type the iterator passed
             *      in @p where merely serves as a hint to the insert routine and
             *      may not be considered a binding requirement.
             * @note This method is never called directly, instead it is invoked
             *      through a call to the insert() method, which makes sure the
             *      node is not already owned by another container and marks this
             *      container dirty if the node was successfully added.
             */
            virtual iterator insertImpl(iterator const& where, Node* child) = 0;

            /**
             * Erase the range of child nodes referred to by @p first and
             * @p last from this container.
             * @param first an iterator referring to the first element in the
             *      sequence of child nodes that should be erased.
             * @param last an iterator referring to the element one past the
             *      last element of the sequence of child nodes that should be
             *      erased.
             * @throw An exception derived from std::runtime_error if the
             *      container policy does not allow the elements to be erased,
             *      or erasing a node failed for some reason.
             * @note This method is never called directly, it is called
             *      indirectly through the erase() method, which takes care of
             *      marking the container dirty after the child nodes have been
             *      successfully deleted. 
             */
            virtual void eraseImpl(iterator const& first, iterator const& last) = 0;

         private:
            /**
             * Private and unimplemented assignment operator to disallow assignment
             * of nodes to each other.
             */
            Container& operator=(Container const&);
   };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Container.ipp"
#endif

#endif  // __LIBEMBER_DOM_CONTAINER_HPP

