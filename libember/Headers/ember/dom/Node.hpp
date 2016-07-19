/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_NODE_HPP
#define __LIBEMBER_DOM_NODE_HPP

#include "../ber/Type.hpp"
#include "../ber/Tag.hpp"
#include "../util/OctetStream.hpp"

namespace libember { namespace dom
{
    /** Forward declaration for friend access. */
    class Container;

    /**
     * Common base class for all types of nodes a tree.
     */
    class LIBEMBER_API Node
    {
        friend class Container;
        public:
            /**
             * Virtualized copy constructor that creates a deep copy of the DOM
             * subtree rooted at this node on the heap. The resulting root node
             * has no parent set and may therefor be used to transfer a subtree
             * from one parent node to another, by a subsequent call to
             * Container::insert().
             * @return A pointer to the root node of the copied subtree.
             * @throw An exception derived from std::exception if cloning the
             *      node failed for some reason.
             * @see Container::insert()
             */
            virtual Node* clone() const = 0;

            /**
             * Public virtual destructor as this base class allows ownership.
             */
            virtual ~Node();

            /**
             * Return the application tag of this node.
             * @return The application tag of this node.
             */
            ber::Tag applicationTag() const;

            /**
             * Return The type tag of this node.
             * @return The type tag of this node.
             */
            ber::Tag typeTag() const;

            /**
             * Return a mutable pointer to the parent node in which this node
             * is contained.
             * @return A pointer to the parent node or 0 if this node is the
             *      root of a tree.
             */
            Node* parent();
            
            /**
             * Return an immutable pointer to the parent node in which this
             * node is contained.
             * @return A pointer to the parent node or 0 if this node is the
             *      root of a tree.
             */
            Node const* parent() const;

            /**
             * Clear the dirty state of this node by updating any cached internal
             * state that might relate to the encoded representation of this node.
             * The post-condition to calling this method is that an immediate
             * call to isDirty() returns false.
             */
            void update() const;

            /**
             * Encode the BER representation of this node to the stream buffer
             * provided in @p output. In case this node is currently marked dirty
             * this method automatically calls update() before actually encoding
             * it's contents.
             * @param output a reference to the stream buffer, to which the contents
             *      of this node should be encoded.
             * @see update()
             */
            void encode(util::OctetStream& output) const;

            /**
             * Return the number of bytes the BER representation of this node
             * requires. In case this node is currently marked dirty this method
             * automatically calls update() before actually returning the encoded
             * length.
             * @return The number of bytes this node will require when encoded,
             *      without being marked dirty in the meantime.
             * @see update()
             */
            std::size_t encodedLength() const;

        protected:
            /**
             * Return the type tag that should be used to tag the inner frame when
             * encoding the contents of this node.
             * @return The type tag of this node type.
             */
            virtual ber::Tag typeTagImpl() const = 0;

            /**
             * Clear the dirty state of this node by updating any cached internal
             * state that might relate to the encoded representation of this node.
             * @note This method is never called directly, but instead is called
             *      indirectly by a call to update(), which clears the dirty flag
             *      after invoking this method.
             * @see update()
             */
            virtual void updateImpl() const = 0;

            /**
             * Encode the BER representation of this node to the stream buffer
             * provided in @p output. 
             * @param output a reference to the stream buffer, to which the contents
             *      of this node should be encoded.
             * @note This method is never called directly, but instead is called
             *      indirectly by a call to encode(), which makes sure that the node
             *      is not in a dirty state before relaying the call to this
             *      implementation.
             * @see encode()
             */
            virtual void encodeImpl(util::OctetStream& output) const = 0;

            /**
             * Return the number of bytes the BER representation of this node
             * requires. 
             * @return The number of bytes this node will require when encoded,
             *      without being marked dirty in the meantime.
             * @note This method is never called directly, but instead is called
             *      indirectly by a call to encodedLength(), which makes sure
             *      that the node is not in a dirty state before relaying the call
             *      to this implementation.
             * @see encodedLength()
             */
            virtual std::size_t encodedLengthImpl() const = 0;

        protected:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because its BER encoding has not yet been calculated.
             */
            explicit Node(ber::Tag tag);

            /**
             * Copy constructor that initializes the instance as a copy of
             * @p other, with the exception that the parent pointer is
             * initialized with 0, thus making the copy a new root node.
             * @param other a constant reference to the Node of which the
             *      created node should be made a copy.
             * @note Please note that a newly constructed node is
             *      always marked dirty, because its BER encoding has
             *      not yet been calculated.
             */
            Node(Node const& other);

            /**
             * Set the parent container of this node.  
             * @param parent a pointer to the container node to the node of which
             *      this node should be made a child. If 0 is passed this node
             *      serves as the root node of a tree.
             */
            void setParent(Node* parent);

            /**
             * Indicate that the contents of this node or any of it's children
             * changed and a recalculating of the cached BER encoding is
             * required.
             * The post-condition to calling this method is that an immediate
             * call to isDirty() returns true.
             */
            void markDirty() const;

            /**
             * Return whether or not the node is currently in the dirty state.
             * @return True if the Node is currently in the dirty state, otherwise
             *      false.
             */
            bool isDirty() const;

        private:
            /**
             * Private and unimplemented assignment operator to disallow assignment
             * of nodes to each other.
             */
            Node& operator=(Node const&);

        private:
            ber::Tag m_applicationTag;
            Node* m_parent;
            mutable bool m_dirty;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Node.ipp"
#endif

#endif  // __LIBEMBER_DOM_NODE_HPP
