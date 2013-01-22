#ifndef __LIBEMBER_BER_OBJECTIDENTIFIER_HPP
#define __LIBEMBER_BER_OBJECTIDENTIFIER_HPP

#include <algorithm>

namespace libember { namespace ber
{
    /**
     * A simple template type that wraps an (unsigned) 64-Bit integer
     * representing an object identifier.
     */
    class ObjectIdentifier
    {
        public:
            typedef unsigned long long value_type;

        public:
            /**
             * Default constructor. Initializes the wrapped object identifier
             * with 0.
             */
            ObjectIdentifier();

            /**
             * Constructor that wraps the passed value.
             * @param value the value to wrap.
             * @note Please note that this constructor is intentionally non-explicit
             *      to allow for convenient implicit conversion, where it might be
             *      suitable.
             */
            ObjectIdentifier(value_type value);

            /**
             * Accessor method that returns a copy of the stored identifier.
             * @return A copy of the stored object identifier.
             */
            value_type value() const;

            /**
             * Accessor method that returns a reference to the stored identifier.
             * @return A reference to the stored object identifier.
             */
            value_type& value();

            /**
             * Swap method that exchanges the object identifier stored within
             * this instance with the object identifier stored in @p other.
             * @param other a reference to the ObjectIdentifier instance this
             *      instance should exchange its contents with.
             */
            void swap(ObjectIdentifier& other);

        private:
            value_type m_value;
    };


    /**
     * Free version of swap to allow it's usage through ADL.
     * @param lhs a reference to the first instance.
     * @param rhs a reference to a second instance whose contents should be
     *      swapped with those of @p lhs.
     */
    void swap(ObjectIdentifier& lhs, ObjectIdentifier& rhs);


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    inline void swap(ObjectIdentifier& lhs, ObjectIdentifier& rhs)
    {
        lhs.swap(rhs);
    }

}
}

#endif  // __LIBEMBER_BER_OBJECTIDENTIFIER_HPP


