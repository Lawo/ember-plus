/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_STATICLEAF_HPP
#define __LIBEMBER_DOM_STATICLEAF_HPP

#include "../ber/Value.hpp"
#include "Node.hpp"

//SimianIgnore

namespace libember { namespace dom
{
    /**
     * A leaf node type that contains a statically typed value.
     */
    template<typename ValueType>
    class StaticLeaf : public Node 
    {
        public:
            typedef ValueType value_type;

        public:
            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag and without a parent node.
             * @param tag the application tag of this node.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            explicit StaticLeaf(ber::Tag tag);

            /**
             * Constructor that initializes the node with the application tag
             * specified in @p tag, an initial value of @p value and without a
             * parent node.
             * @param tag the application tag of this node.
             * @param value the initial value the node should be initilized with.
             * @note Please note that a newly constructed node is always marked
             *      dirty, because the state variables affecting its encoded
             *      representation have not yet been calculated.
             */
            StaticLeaf(ber::Tag tag, value_type value);

            /**
             * Copy constructor that copies the contents of @p other to the
             * newly created instance.
             * @param other a constant reference to the leaf node whose
             *      contents should be copied.
             */
            StaticLeaf(StaticLeaf const& other);

            /**
             * Covariant override of Node::clone()
             * @see Node::clone()
             */
            virtual StaticLeaf* clone() const;

            /**
             * Accessor for the primitive value represented by this leaf node.
             * @return The primitive value represented by this leaf node.
             */
            value_type value() const;

            /**
             * Set the value of this node to @p value.
             * @param value the new value of this node.
             */
            void setValue(value_type value);

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
            value_type m_value;
            mutable std::size_t m_cachedLength;
    };



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline StaticLeaf<ValueType>::StaticLeaf(ber::Tag tag)
        : Node(tag), m_value(), m_cachedLength(0)
    {}

    template<typename ValueType>
    inline StaticLeaf<ValueType>::StaticLeaf(StaticLeaf const& other)
        : Node(static_cast<Node const&>(other)), m_value(other.m_value),
            m_cachedLength(0)
    {}

    template<typename ValueType>
    inline StaticLeaf<ValueType>* StaticLeaf<ValueType>::clone() const
    {
        return new StaticLeaf(*this);
    }

    template<typename ValueType>
    inline typename StaticLeaf<ValueType>::value_type StaticLeaf<ValueType>::value() const
    {
        return m_value;
    }

    template<typename ValueType>
    inline void StaticLeaf<ValueType>::setValue(value_type value)
    {
        m_value = value;
        markDirty();
    }

    template<typename ValueType>
    inline ber::Tag StaticLeaf<ValueType>::typeTagImpl() const
    {
        return ber::universalTag<value_type>();
    }

    template<typename ValueType>
    inline void StaticLeaf<ValueType>::updateImpl() const
    {
        std::size_t const innerTagLength = ber::encodedLength(typeTag());
        std::size_t const payloadLength  = ber::encodedLength(m_value);
        std::size_t const innerLength    = innerTagLength + ber::encodedLength(ber::make_length(payloadLength)) + payloadLength;

        std::size_t const outerTagLength = ber::encodedLength(applicationTag().toContainer());
        std::size_t const outerLength    = outerTagLength + ber::encodedLength(ber::make_length(innerLength)) + innerLength;

        m_cachedLength = outerLength;
    }

    template<typename ValueType>
    inline void StaticLeaf<ValueType>::encodeImpl(util::OctetStream& output) const
    {
        ber::Tag const innerTag = typeTag();
        std::size_t const innerTagLength = ber::encodedLength(innerTag);
        std::size_t const payloadLength  = ber::encodedLength(m_value);
        std::size_t const innerLength    = innerTagLength + ber::encodedLength(ber::make_length(payloadLength)) + payloadLength;
        
        // Encode the outer frame (as a container)
        ber::encode(output, applicationTag().toContainer());
        ber::encode(output, ber::make_length(innerLength));
        
        // Encode the inner frame
        ber::encode(output, innerTag);
        ber::encode(output, ber::make_length(payloadLength));

        // Encode the value
        ber::encode(output, m_value);
    }

    template<typename ValueType>
    inline std::size_t StaticLeaf<ValueType>::encodedLengthImpl() const
    {
        return m_cachedLength;
    }
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_DOM_STATICLEAF_HPP

