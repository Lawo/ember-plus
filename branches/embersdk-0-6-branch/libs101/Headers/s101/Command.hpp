#ifndef __LIBS101_COMMAND_HPP
#define __LIBS101_COMMAND_HPP

namespace libs101
{
    /**
     * Struct enumerating the commands available for the S101 messages.
     */ 
    struct Command
    {
        enum _Domain
        {
            EmBER = 0x10
        };


        typedef unsigned char value_type;

        /**
         * Non-explicit constructor to allow implicit conversion.
         * @param value A value from _Domain to initialize this instance with.
         */
        Command(_Domain value)
            : value(value)
        {}

        /**
         * Command value.
         */
        value_type const value;
    };
}

#endif  // __LIBS101_COMMAND_HPP

