#ifndef __LIBS101_DTD_HPP
#define __LIBS101_DTD_HPP

namespace libs101
{
    /**
     * Struct enumerating the DTD types available.
     */ 
    struct Dtd
    {
        enum _Domain
        {
            Glow = 1
        };


        typedef unsigned char value_type;

        /**
         * Non-explicit constructor to allow implicit conversion.
         * @param value A value from _Domain to initialize this instance with.
         */
        Dtd(_Domain value)
            : value(value)
        {}

        /**
         * DTD value.
         */
        value_type const value;
    };
}

#endif  // __LIBS101_DTD_HPP

