/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_DOM_VARIANTLEAF_HPP
#define __LIBEMBER_DOM_VARIANTLEAF_HPP

#include "../ber/Value.hpp"
#include "Node.hpp"

//SimianIgnore

namespace libember { namespace dom
{
    /**
     * Base class for all leaf nodes. Serves as an extension point
     * in case functionality common to all leaf nodes has to be added.
     */
    class LIBEMBER_API VariantLeaf : public Node 
    {
        friend class VariantValue;
        public:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit VariantLeaf(ber::Tag tag);

            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag, an intial value specified in @p value and
             * without a parent node.
             * @param tag the application tag of this node.
             * @param value the initial value of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit VariantLeaf(ber::Tag tag, ber::Value value);

            /**
             * Copy constructor that copies the contents of @p other to the
             * newly created instance.
             * @param other a constant reference to the leaf node whose
             *      contents should be copied.
             */
            VariantLeaf(VariantLeaf const& other);

            /**
             * Covariant override of Node::clone()
             * @see Node::clone()
             */
            virtual VariantLeaf* clone() const;

            /**
             * Const qualified accessor for the primitive value represented by
             * this leaf node.
             * @return The type-erased primitive value represented by this leaf
             *      node.
             */
            ber::Value value() const;

            /**
             * Setter for the primitive value represented by this leaf node.
             * @param value the type-erased primitive value to which the value
             *      represented by this leaf should be set.
             */
            void setValue(ber::Value value);

        protected:
            /** @see Node::typeTagImpl() */
            virtual ber::Tag typeTagImpl() const;

            /** @see Node::updateImpl() */
            virtual void updateImpl() const;

            /** @see Node::encodeImpl() */
            virtual void encodeImpl(util::OctetStream& output) const;

            /** @see Node::encodedLengthImpl() */
            virtual std::size_t encodedLengthImpl() const;

        private:
            ber::Value m_value;
            mutable std::size_t m_cachedLength;
    };

}
}

//EndSimianIgnore

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/VariantLeaf.ipp"
#endif

#endif  // __LIBEMBER_DOM_VARIANTLEAF_HPP

