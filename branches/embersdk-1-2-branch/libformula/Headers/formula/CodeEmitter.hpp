#ifndef __LIBFORMULA_CODEEMITTER_HPP
#define __LIBFORMULA_CODEEMITTER_HPP

#include "FunctionType.hpp"
#include "Types.hpp"

namespace libformula
{
    /**
     * This interface is used by the parser to generate the IL OpCodes required to execute
     * the provided term.
     */
    class CodeEmitter
    {
        public:
            /** Destructor */
            virtual ~CodeEmitter()
            {}

            /**
             * Called to emit a push operation
             * @param value The integer value to push onto the stack.
             */
            virtual void emitPushLong(long_type value) = 0;

            /**
             * Called to emit a push operation
             * @param value The real value to push onto the stack.
             */
            virtual void emitPushReal(real_type value) = 0;

            /**
             * Called to emit a push operation, where the value input variable must be pushed onto the stack.
             */
            virtual void emitPushIt() = 0;

            /**
             * Called to emit an add operation, which is supposed to use the two items on top of the stack.
             */
            virtual void emitAdd() = 0;

            /**
             * Called to emit a subtract operation, which is supposed to use the two items on top of the stack.
             */
            virtual void emitSubtract() = 0;

            /**
             * Called to emit a multiply operation, which is supposed to use the two items on top of the stack.
             */
            virtual void emitMultiply() = 0;

            /**
             * Called to emit a divide operation, which is supposed to use the two items on top of the stack.
             * @note The result of this operation should always be double.
             */
            virtual void emitDivide() = 0;

            /**
             * Called to emit a modulo operation, which is supposed to use the two items on top of the stack.
             * @note The result of this operation should always be integer.
             */
            virtual void emitModulo() = 0;

            /**
             * Called to emit an integer divide operation, which is supposed to use the two items on top of the stack.
             * @note The result of this operation should always be integer.
             */
            virtual void emitLongDivide() = 0;

            /**
             * Called to emit a function call. The arguments are pushed onto the stack before calling the function.
             * @param function The function type to invoke.
             * @param number of arguments provided.
             */
            virtual void emitCall(FunctionType const& function, int argcount) = 0;

            /**
             * Called to negate the item that is currently on top of the stack.
             */
            virtual void emitNegate() = 0;
    };
}

#endif  // __LIBFORMULA_CODEEMITTER_HPP
