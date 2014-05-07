#ifndef __LIBFORMULA_UTIL_CODEDUMP_HPP
#define __LIBFORMULA_UTIL_CODEDUMP_HPP

#include "../CodeEmitter.hpp"
#include "Util.hpp"
#include <cstdio>
#include <iostream>

namespace libformula { namespace util
{
    /**
     * Implementation of the CodeEmitter interface which simply writes the emitted opcodes to the console.
     */
    class CodeDump : public libformula::CodeEmitter
    {
        public:
            /** @see CodeEmitter */
            virtual void emitPushLong(long value) 
            {
                std::cout << "Long(" << value << ")" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitPushReal(double value) 
            {
                std::cout << "Real(" << value << ")" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitPushIt() 
            {
                std::cout << "It" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitAdd() 
            {
                std::cout << "+" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitSubtract() 
            {
                std::cout << "-" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitMultiply() 
            {
                std::cout << "*" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitDivide() 
            {
                std::cout << "/" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitLongDivide() 
            {
                std::cout << "\\" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitModulo() 
            {
                std::cout << "%" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitCall(libformula::FunctionType const& function, int argcount) 
            {
                auto const name = functiontype_tostring(function);
                if (argcount == 1)
                    std::cout << name << " (1 arg)" << std::endl;
                else
                    std::cout << name << " (" << argcount << " args)" << std::endl;
            }

            /** @see CodeEmitter */
            virtual void emitNegate()
            {
                std::cout << "negate" << std::endl;
            }
    };
}
}

#endif  // __LIBFORMULA_UTIL_CODEDUMP_HPP
