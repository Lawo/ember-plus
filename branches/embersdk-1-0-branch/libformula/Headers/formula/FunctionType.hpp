#ifndef __LIBFORMULA_FUNCTIONTYPE_HPP
#define __LIBFORMULA_FUNCTIONTYPE_HPP

#include "SymbolType.hpp"

namespace libformula
{
    /**
     * Represents the type of a function.
     */
    struct FunctionType
    {
        public:
            enum _Domain
            {
                Invalid,
                Exp,
                Cos,
                Sin,
                Tan,
                Acos,
                Asin,
                Atan,
                Cosh,
                Sinh,
                Tanh,
                Int,
                Float,
                Log,
                Ln,
                Round,
                Ceil,
                Sqrt,
                Pow,
            };

            typedef unsigned int value_type;

            /**
             * Initializes a new FunctionType instance.
             * @param value The function to initialize this instance with.
             */
            FunctionType(_Domain value)
                : m_value(value)
            {}

            /**
             * Returns the function value.
             * @return The function value.
             */
            value_type value() const
            {
                return m_value;
            }

            /**
             * Evaluates whether the provided symbol is a function or not.
             * @return True when the symbol is a function, otherwise false.
             */
            static bool isFunction(SymbolType const& type)
            {
                auto const value = type.value();
                switch(value)
                {
                    case SymbolType::Sin:
                    case SymbolType::Cos:
                    case SymbolType::Tan:
                    case SymbolType::Sinh:
                    case SymbolType::Cosh:
                    case SymbolType::Tanh:
                    case SymbolType::Asin:
                    case SymbolType::Acos:
                    case SymbolType::Atan:
                    case SymbolType::Sqrt:
                    case SymbolType::Log:
                    case SymbolType::Ln:
                    case SymbolType::Round:
                    case SymbolType::Ceil:
                    case SymbolType::Int:
                    case SymbolType::Float:
                    case SymbolType::Exp:
                    case SymbolType::Powf:
                        return true;

                    default:
                        return false;
                }
            }

            /**
             * Initializes a new FunctionType instance from the provided symbol type.
             * @param type The symbol type to create a function from.
             * @return Returns the new FunctionType instance. If the symbol doesn't represent a function,
             *      the type is set to 'Invalid'.
             */
            static FunctionType fromSymbol(SymbolType const& type)
            {
                auto const value = type.value();
                switch(value)
                {
                    case SymbolType::Sin:
                        return FunctionType::Sin;
                    case SymbolType::Cos:
                        return FunctionType::Cos;
                    case SymbolType::Tan:
                        return FunctionType::Tan;
                    case SymbolType::Sinh:
                        return FunctionType::Sinh;
                    case SymbolType::Cosh:
                        return FunctionType::Cosh;
                    case SymbolType::Tanh:
                        return FunctionType::Tanh;
                    case SymbolType::Asin:
                        return FunctionType::Asin;
                    case SymbolType::Acos:
                        return FunctionType::Acos;
                    case SymbolType::Atan:
                        return FunctionType::Atan;
                    case SymbolType::Sqrt:
                        return FunctionType::Sqrt;
                    case SymbolType::Log:
                        return FunctionType::Log;
                    case SymbolType::Ln:
                        return FunctionType::Ln;
                    case SymbolType::Round:
                        return FunctionType::Round;
                    case SymbolType::Ceil:
                        return FunctionType::Ceil;
                    case SymbolType::Int:
                        return FunctionType::Int;
                    case SymbolType::Float:
                        return FunctionType::Float;
                    case SymbolType::Exp:
                        return FunctionType::Exp;
                    case SymbolType::Powf:
                        return FunctionType::Pow;
                    default:
                        return FunctionType::Invalid;
                }
            }

            /**
             * Returns the number of arguments the function requires.
             * @return The number of arguments the function requires.
             */
            int minargs() const
            {
                switch(m_value)
                {
                    case FunctionType::Sin:
                    case FunctionType::Cos:
                    case FunctionType::Tan:
                    case FunctionType::Sinh:
                    case FunctionType::Cosh:
                    case FunctionType::Tanh:
                    case FunctionType::Asin:
                    case FunctionType::Acos:
                    case FunctionType::Atan:
                    case FunctionType::Sqrt:
                    case FunctionType::Log:
                    case FunctionType::Ln:
                    case FunctionType::Round:
                    case FunctionType::Ceil:
                    case FunctionType::Int:
                    case FunctionType::Float:
                    case FunctionType::Exp:
                        return 1;
                    case FunctionType::Pow:
                        return 2;
                    default:
                        return 0;
                }
            }

            /**
             * Returns the number of arguments the function requires.
             * @return The number of arguments the function requires.
             */
            int maxargs() const
            {
                switch(m_value)
                {
                    case FunctionType::Sin:
                    case FunctionType::Cos:
                    case FunctionType::Tan:
                    case FunctionType::Sinh:
                    case FunctionType::Cosh:
                    case FunctionType::Tanh:
                    case FunctionType::Asin:
                    case FunctionType::Acos:
                    case FunctionType::Sqrt:
                    case FunctionType::Ln:
                    case FunctionType::Round:
                    case FunctionType::Ceil:
                    case FunctionType::Int:
                    case FunctionType::Float:
                    case FunctionType::Exp:
                        return 1;
                    case FunctionType::Log:
                    case FunctionType::Pow:
                    case FunctionType::Atan:
                        return 2;
                    default:
                        return 0;
                }
            }

        private:
            value_type m_value;
    };
}

#endif//__LIBFORMULA_FUNCTIONTYPE_HPP
