#ifndef __LIBS101_BYTE_HPP
#define __LIBS101_BYTE_HPP

namespace libs101 
{
    /**
     * Struct enumerating the bytes having a special meaning in 
     * the S101 framing.
     */ 
    struct Byte
    {
        enum _Domain
        {
            CE = 0xFD,
            BoF = 0xFE,
            EoF = 0xFF,
            XOR = 0x20,
            Invalid = 0xF8,
        };


        typedef unsigned char value_type;

        /**
         * Non-explicit constructor to allow implicit conversion.
         * @param value A value from _Domain to initialize this instance with.
         */
        Byte(_Domain value)
            : value(value)
        {}

        /**
         * The byte value.
         */
        value_type const value;
    };
}

#endif  // __LIBS101_BYTE_HPP

