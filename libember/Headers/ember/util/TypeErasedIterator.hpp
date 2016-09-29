/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_TYPEERASEDITERATOR_HPP
#define __LIBEMBER_UTIL_TYPEERASEDITERATOR_HPP

#include <typeinfo>
#include <iterator>
#include "../meta/RemoveCV.hpp"

//SimianIgnore

namespace libember { namespace util
{
    /** Forward declaration. */
    template<typename ValueType>
    class TypeErasedIterator;
    
    /**
     * Equality comparison operator for TypeErasedIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return True if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise false.
     */
    template<typename ValueType>
    bool operator==(TypeErasedIterator<ValueType> const& lhs, TypeErasedIterator<ValueType> const& rhs);

    /**
     * Inequality comparison operator for TypeErasedIterator specializations.
     * @param lhs a constant reference to the first type erased iterator.
     * @param rhs a constant reference to the second type erased iterator.
     * @return False if @p lhs and @p rhs refer the same element within a sequence,
     *      otherwise true.
     */
    template<typename ValueType>
    bool operator!=(TypeErasedIterator<ValueType> const& lhs, TypeErasedIterator<ValueType> const& rhs);

    /**
     * An erasure type for iterators reducing the static polymorphism to their
     * value type only.
     * @note Please note that this is a rather minimal implementation relying a lot
     *      on the ability to obtain a pointer to the referred element from a wrapped
     *      iterator.
     */
    template<typename ValueType>
    class TypeErasedIterator
    {
        friend bool operator==<>(TypeErasedIterator<ValueType> const&, TypeErasedIterator<ValueType> const&);
        public:
            typedef std::forward_iterator_tag                   iterator_category;
            typedef typename meta::RemoveConst<ValueType>::type value_type;
            typedef ValueType&                                  reference;
            typedef ValueType*                                  pointer;
            typedef std::ptrdiff_t                              difference_type;

        public:
            /**
             * Default constructor. Initializes the instance in a singular state.
             */
            TypeErasedIterator();

            /**
             * Copy constructor. Initializes the instance by copying the state
             * from @p other.
             * @param other the TypeErasedIterator instance whose contents should
             *      be copied.
             */
            TypeErasedIterator(TypeErasedIterator const& other);

            /**
             * Single value constructor that initializes this instance by wrapping
             * the specific iterator passed in @p iterator, provided that it's
             * value type matches ValueType.
             * @param iterator the specific iterator to initialize this instance
             *      with.
             */
            template<typename IteratorType>
            TypeErasedIterator(IteratorType iterator);

            /** Destructor. */
            ~TypeErasedIterator();

            /**
             * Overloaded assignment operator that copies the state of the
             * TypeErasedIterator instance passed via @p other.
             * @param other the TypeErasedIterator instance whose state should
             *      be copied into this instance.
             */
            TypeErasedIterator& operator=(TypeErasedIterator other);

            /**
             * Overloaded dereference operator. Returns a reference to the
             * element this iterator refers to. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined. 
             * @return A reference to the element referred to by this iterator.
             */
            reference operator*() const;

            /**
             * Overloaded member access operator. Returns a pointer to the
             * element this iterator refers to. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined. 
             * @return A pointer to the element referred to by this iterator.
             */
            pointer operator->() const;

            /**
             * Overloaded pre increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A reference to this instance.
             */
            TypeErasedIterator& operator++();

            /**
             * Overloaded post increment operator. Advances the currently wrapped
             * iterator so that it refers to the element following the current
             * element in the controlled sequence. If this iterator is in a
             * singular state, either because it does not currently wrap any
             * specific iterator, or because the currently wrapped iterator is
             * non-dereferencable, the behaviour of this method is undefined.
             * @return A TypeErasedIterator referring to the current, not
             *      incremented position.
             */
            TypeErasedIterator operator++(int);

            /**
             * Inplace swap function that exchanges the state of this instance
             * with the state of @p other. This method is guaranteed not to
             * throw any exceptions.
             * @param other a reference to the TypeErasedIterator instance
             *      whose contents should be exchanged with those of this
             *      instance.
             */
            void swap(TypeErasedIterator& other);

            /**
             * Type recovery method,
             * @return A copy of the wrapped iterator.
             * @throw std::bad_cast if DestType does not match the type of the
             *      wrapped iterator.
             */
            template<typename DestType>
            DestType as() const;

        private:
            /**
             * Base class for the internal payload implementations.
             */
            struct Payload
            {
                virtual Payload* clone() const = 0;
                virtual void preIncrement() = 0;
                virtual pointer getPointer() const = 0;
                virtual std::type_info const& typeId() const = 0;
                virtual bool equals(Payload const* other) const = 0;
                virtual ~Payload() {}
            };

            /**
             * Template class derived from the Payload base class used to
             * virtualize the operations performed upon a specific iterator
             * type.
             */
            template<typename IteratorType>
            class PayloadImpl : public Payload
            {
                public:
                    explicit PayloadImpl(IteratorType iterator);

                    virtual Payload* clone() const;
                    virtual void preIncrement();
                    virtual pointer getPointer() const;
                    virtual std::type_info const& typeId() const;
                    virtual bool equals(Payload const* other) const;

                    IteratorType value() const;

                private:
                    IteratorType m_iterator;
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
    template<typename ValueType>
    void swap(TypeErasedIterator<ValueType>& lhs, TypeErasedIterator<ValueType>& rhs);



    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename ValueType>
    inline TypeErasedIterator<ValueType>::TypeErasedIterator()
        : m_payload(0)
    {}

    template<typename ValueType>
    inline TypeErasedIterator<ValueType>::TypeErasedIterator(TypeErasedIterator const& other)
        : m_payload((other.m_payload != 0) ? other.m_payload->clone() : 0)
    {}

    template<typename ValueType>
    template<typename IteratorType>
    inline TypeErasedIterator<ValueType>::TypeErasedIterator(IteratorType iterator)
        : m_payload(new PayloadImpl<IteratorType>(iterator))
    {}

    template<typename ValueType>
    inline TypeErasedIterator<ValueType>::~TypeErasedIterator()
    {
        delete m_payload;
    }

    template<typename ValueType>
    inline TypeErasedIterator<ValueType>& TypeErasedIterator<ValueType>::operator=(TypeErasedIterator other)
    {
        swap(other);
        return *this;
    }

    template<typename ValueType>
    inline typename TypeErasedIterator<ValueType>::reference TypeErasedIterator<ValueType>::operator*() const
    {
        return *(m_payload->getPointer());
    }

    template<typename ValueType>
    inline typename TypeErasedIterator<ValueType>::pointer TypeErasedIterator<ValueType>::operator->() const
    {
        return m_payload->getPointer();
    }

    template<typename ValueType>
    inline TypeErasedIterator<ValueType>& TypeErasedIterator<ValueType>::operator++()
    {
        if (m_payload)
        {
            m_payload->preIncrement();
        }
        return *this;
    }

    template<typename ValueType>
    inline TypeErasedIterator<ValueType> TypeErasedIterator<ValueType>::operator++(int)
    {
        TypeErasedIterator const copy(*this);
        if (m_payload)
        {
            m_payload->preIncrement();
        }
        return copy;
    }

    template<typename ValueType>
    inline void TypeErasedIterator<ValueType>::swap(TypeErasedIterator& other)
    {
        using std::swap;
        swap(m_payload, other.m_payload);
    }

    template<typename ValueType>
    template<typename DestType>
    inline DestType TypeErasedIterator<ValueType>::as() const
    {
        if ((m_payload == 0) || (m_payload->typeId() != typeid(DestType)))
        {
            throw std::bad_cast();
        }
        return static_cast<PayloadImpl<DestType> const*>(m_payload)->value();
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::PayloadImpl(IteratorType iterator)
        : m_iterator(iterator)
    {}

    template<typename ValueType>
    template<typename IteratorType>
    inline typename TypeErasedIterator<ValueType>::Payload* TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::clone() const
    {
        return new PayloadImpl(*this);
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline void TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::preIncrement()
    {
        ++m_iterator;
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline typename TypeErasedIterator<ValueType>::pointer TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::getPointer() const
    {
        return &(*m_iterator);
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline std::type_info const& TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::typeId() const
    {
        return typeid(IteratorType);
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline bool TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::equals(Payload const* other) const
    {
        return (other != 0) && (other->typeId() == typeId()) && (m_iterator == static_cast<PayloadImpl const*>(other)->m_iterator);
    }

    template<typename ValueType>
    template<typename IteratorType>
    inline IteratorType TypeErasedIterator<ValueType>::PayloadImpl<IteratorType>::value() const
    {
        return m_iterator;
    }

    template<typename ValueType>
    void swap(TypeErasedIterator<ValueType>& lhs, TypeErasedIterator<ValueType>& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename ValueType>
    bool operator==(TypeErasedIterator<ValueType> const& lhs, TypeErasedIterator<ValueType> const& rhs)
    {
        // While this implementation is actually quite neat, as it also works
        // with different iterator types somehow referring to the same unique
        // value, it crashes with STL implementations that more rigidly enforce
        // the non-derefererencability of iterators in singular state such as
        // the STL implementation shipped with Microsoft Visual C++.
        //typename TypeErasedIterator<ValueType>::pointer const lhsPtr = (lhs.m_payload != 0) ? lhs.m_payload->getPointer() : 0;
        //typename TypeErasedIterator<ValueType>::pointer const rhsPtr = (rhs.m_payload != 0) ? rhs.m_payload->getPointer() : 0;
        //return (lhsPtr == rhsPtr);

        typename TypeErasedIterator<ValueType>::Payload const* const lhPayload = lhs.m_payload;
        typename TypeErasedIterator<ValueType>::Payload const* const rhPayload = rhs.m_payload;
        if ((lhPayload == 0) || (rhPayload == 0))
        {
            return (lhPayload == rhPayload);
        }
        else
        {
            return lhPayload->equals(rhPayload);
        }
    }

    template<typename ValueType>
    inline bool operator!=(TypeErasedIterator<ValueType> const& lhs, TypeErasedIterator<ValueType> const& rhs)
    {
        return !(lhs == rhs);
    }
}
}

//EndSimianIgnore

#endif  // __LIBEMBER_UTIL_TYPEERASEDITERATOR_HPP

