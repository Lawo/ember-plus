/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_VALUE_HPP
#define __LIBEMBER_BER_VALUE_HPP

#include <typeinfo>
#include "../util/Api.hpp"
#include "traits/CodecTraits.hpp"

namespace libember { namespace ber
{
    /**
     * A type-safe non-discriminated union type that allows introspection of all
     * properties related to the BER encoding of the stored value and its
     * specific type.
     */
    class LIBEMBER_API Value
    {
        public:
            /**
             * Default constructor. Initializes the instance with a singular state.
             */
            Value();
                
            /**
             * Copy constructor.
             * @param other a constant reference to the value whose contents should
             *      be copied into the newly created instance.
             */
            Value(Value const& other);

            /**
             * Constructor that initializes this instance by wrapping the value
             * passed in @p value.
             * @note Please note that this constructor is intentionally
             *      non-explicit.
             */
            template<typename ValueType>
            Value(ValueType value);

            /** Destructor. */
            virtual ~Value();

            /**
             * Non-throwing swap method. Exchanges the contents of this instance
             * with those of @p other.
             * @param other a reference to the instance with whom the contents of
             *      this instance should be exchanged.
             */
            void swap(Value& other);

            /**
             * Assignment operator that copies the value state of @p other
             * into this instance.
             * @param other The value whose contents should be copied.
             * @note Please note that because of the availability of a
             *      non-explicit, single argument value constructor it is
             *      possible to directly assign values to an instance through
             *      this assignment operator.
             * @note The parameter @p other is taken by value to enable copy
             *      elision.
             */
            Value& operator=(Value other);

            /**
             * Implicit conversion to boolean operator to test whether
             * the object is in a regular state, i.e. has a valid wrapped
             * value.
             * @return True if this object is in a regular state, otherwise
             *      false.
             * @todo Replace with safe-bool idiom.
             */
            operator bool() const;

            /**
             * Return a type_info instance describing the type of the
             * wrapped value.
             * @return A constant reference type_info instance describing the
             *      type of the wrapped value.
             * @note Please note that invoking this method is only valid for
             *      instances which are in a regular state as indicated by
             *      their implicit conversion to bool. Invoking this method
             *      on instances in singular state results in undefined
             *      behaviour.
             */
            std::type_info const& typeId() const;

            /**
             * Return the universal BER tag corresponding to the type of
             * the currently wrapped value.
             * @return The universal BER tag corresponding to the type of
             *      the currently wrapped value.
             * @note Please note that invoking this method is only valid for
             *      instances which are in a regular state as indicated by
             *      their implicit conversion to bool. Invoking this method
             *      on instances in singular state results in undefined
             *      behaviour.
             */
            Tag universalTag() const;

            /**
             * Return the number of bytes required to encode the currently
             * wrapped value.
             * @return The number of bytes needed to encode the currently
             *      wrapped value.
             * @note Please note that invoking this method is only valid for
             *      instances which are in a regular state as indicated by
             *      their implicit conversion to bool. Invoking this method
             *      on instances in singular state results in undefined
             *      behaviour.
             */
            std::size_t encodedLength() const;

            /**
             * Encode the wrapped value to the stream buffer passed in
             * @p output and return the number of bytes that were required
             * to encode the currently wrapped value.
             * @param output a reference to the stream buffer to which the
             *      currently wrapped value should be encoded.
             * @note Please note that invoking this method is only valid for
             *      instances which are in a regular state as indicated by
             *      their implicit conversion to bool. Invoking this method
             *      on instances in singular state results in undefined
             *      behaviour.
             */
            void encode(util::OctetStream& output) const;

            /**
             * Accessor function to retrieve the currently wrapped value.
             * @return The currently wrapped value.
             * @throw std::bad_cast if the instance is currently in a singular
             *      state or the type of the wrapped value does not match the
             *      requested destination type.
             */
            template<typename DestType>
            DestType as() const;

        private:
            /**
             * Type-erasure structure that makes the operations defined in the
             * various encoding traits template dynamically available through
             * virtual functions.
             */
            class LIBEMBER_API Payload
            {
                public:
                    /**
                     * Return the universal tag describing the contained type.
                     * @return The universal tag describing the contained type.
                     */
                    virtual Tag universalTag() const = 0;

                    /**
                     * Return the number of bytes required to encode the wrapped
                     * value.
                     * @return The number of bytes required to encoded the wrapped
                     *      value.
                     */
                    virtual std::size_t encodedLength() const = 0;

                    /**
                     * Encode the wrapped value to the stream buffer referred to
                     * by @p output and return the number of bytes that were
                     * required to encode the value.
                     * @param output a reference to the stream buffer to which the
                     *      wrapped value should be encoded.
                     */
                    virtual void encode(util::OctetStream& output) const = 0;

                    /**
                     * Return the type_info structure of the wrapped value type.
                     * @return A constant reference to the type_info instance describing
                     *      the wrapped value type. 
                     */
                    virtual std::type_info const& typeId() const = 0;

                    /**
                     * Increment the reference count of this payload instance by one.
                     * @return The this pointer.
                     */
                    Payload* addRef();

                    /**
                     * Decrement the reference count of this payload instance by one.
                     * If the reference count reaches zero, this instance deletes
                     * itself.
                     */
                    void releaseRef();

                protected:
                    /**
                     * Default constructor.
                     * Initializes the instances with a reference count of one.
                     */
                    Payload();

                    /** Virtual destructor. */
                    virtual ~Payload();

                private:
                    /**
                     * Private, unimplemented copy constructor to make instances
                     * non-copyable.
                     */
                    Payload(Payload const&);

                    /**
                     * Private, unimplemented assignment operator to make
                     * instances non-copyable.
                     */
                    Payload& operator=(Payload const&);

                private:
                    unsigned long m_refCount;
            };

            /**
             * Templated payload implementation that wraps and holds actual
             * typed values.
             */
            template<typename ValueType>
            class PayloadImpl
                : public Payload
            {
                public:
                    /**
                     * Default constructor. Initializes the instance with a reference
                     * count of one and default constructs the wrapped value.
                     */
                    PayloadImpl();

                    /**
                     * Constructor that initializes the instance with a reference
                     * count of one and the wrapped value as a copy of @p value.
                     * @param value the value this instance should wrap.
                     */
                    explicit PayloadImpl(ValueType value);

                    /**
                     * Accessor to retrieve the held value.
                     * @return The held value.
                     */
                    ValueType value() const;

                    /** @see Payload::universalTag() */
                    virtual Tag universalTag() const;

                    /** @see Payload::encodedLength() */
                    virtual std::size_t encodedLength() const;

                    /** @see Payload::encode() */
                    virtual void encode(util::OctetStream& output) const;

                    /** @see Payload::encodedLength() */
                    virtual std::type_info const& typeId() const;

                private:
                    ValueType m_value;
            };

        private:
            Payload* m_payload;
    };

    /**
     * Free version of swap to allow it's usage through ADL.
     * @param lhs a reference to the first instance.
     * @param rhs a reference to a second instance whose contents should be
     *      swapped with those of @p lhs.
     */
    void swap(Value& lhs, Value& rhs);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline Value::Value(ValueType value)
        : m_payload(new PayloadImpl<ValueType>(value))
    {}

    template<typename DestType>
    inline DestType Value::as() const
    {
        if ((m_payload == 0) || (m_payload->typeId() != typeid(DestType)))
        {
            throw std::bad_cast();
        }
        return static_cast<PayloadImpl<DestType> const*>(m_payload)->value();
    }

    template<typename ValueType>
    inline Value::PayloadImpl<ValueType>::PayloadImpl()
        : Payload(), m_value()
    {}

    template<typename ValueType>
    inline Value::PayloadImpl<ValueType>::PayloadImpl(ValueType value)
        : Payload(), m_value(value)
    {}

    template<typename ValueType>
    inline ValueType Value::PayloadImpl<ValueType>::value() const
    {
        return m_value;
    }

    template<typename ValueType>
    inline Tag Value::PayloadImpl<ValueType>::universalTag() const
    {
        return UniversalTagTraits<ValueType>::universalTag();
    }

    template<typename ValueType>
    inline std::size_t Value::PayloadImpl<ValueType>::encodedLength() const
    {
        return EncodingTraits<ValueType>::encodedLength(m_value);
    }

    template<typename ValueType>
    inline void Value::PayloadImpl<ValueType>::encode(util::OctetStream& output) const
    {
        EncodingTraits<ValueType>::encode(output, m_value);
    }

    template<typename ValueType>
    inline std::type_info const& Value::PayloadImpl<ValueType>::typeId() const
    {
        return typeid(ValueType);
    }

    inline void swap(Value& lhs, Value& rhs)
    {
        lhs.swap(rhs);
    }

}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Value.ipp"
#endif

#endif  // __LIBEMBER_BER_VALUE_HPP


