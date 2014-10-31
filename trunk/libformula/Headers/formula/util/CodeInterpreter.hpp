#ifndef __LIBFORMULA_UTIL_CODEINTERPRETER_HPP
#define __LIBFORMULA_UTIL_CODEINTERPRETER_HPP

#include <cmath>
#include <vector>
#include "ValueStack.hpp"
#include "../CodeEmitter.hpp"
#include "../Types.hpp"

namespace libformula { namespace util
{
    /**
     * Implementation of the CodeEmitter interface, that generates the code
     * in an intermediate language which is able to execute the term operation.
     */
    class CodeInterpreter : public CodeEmitter
    {
        typedef libformula::long_type long_type;
        typedef libformula::real_type real_type;
        public:
            /**
             * Initializes a new code interpreter instance.
             */
            CodeInterpreter();

            /**
             * Computes the term with the provided value used for the $ variable.
             * @param value Value of the variable.
             * @return Returns the result of the term computation.
             */
            template<typename ValueType>
            ValueType compute(ValueType value) const;

        public:
            /** @see CodeEmitter */
            virtual void emitPushLong(long value);

            /** @see CodeEmitter */
            virtual void emitPushReal(double value);

            /** @see CodeEmitter */
            virtual void emitPushIt();

            /** @see CodeEmitter */
            virtual void emitAdd();

            /** @see CodeEmitter */
            virtual void emitSubtract();

            /** @see CodeEmitter */
            virtual void emitMultiply();

            /** @see CodeEmitter */
            virtual void emitDivide();

            /** @see CodeEmitter */
            virtual void emitLongDivide();

            /** @see CodeEmitter */
            virtual void emitModulo();

            /** @see CodeEmitter */
            virtual void emitCall(FunctionType const& function, int argcount);

            /** @see CodeEmitter */
            virtual void emitNegate();

        private:
            /**
             * Structure representing a single opcode.
             */
            struct OpCode
            {
                /**
                 * Enumeration containingng the available opcodes.
                 */
                enum _Domain
                {
                    PushLong,
                    PushReal,
                    PushIt,
                    Add,
                    Sub,
                    Mul,
                    Div,
                    Idiv,
                    Mod,
                    Call,
                    Negate
                };

                /**
                 * Initializes a new OpCode with a long value.
                 * @param code OpCode.
                 * @param value Value associated with that opcode.
                 */
                OpCode(_Domain code, long_type value)
                    : m_code(code)
                {
                    m_long = value;
                }

                /**
                 * Initializes a new OpCode with a real value.
                 * @param code OpCode.
                 * @param value Value associated with that opcode.
                 */
                OpCode(_Domain code, real_type value)
                    : m_code(code)
                {
                    m_real = value;
                }

                /**
                 * Initializes a new OpCode with a function type.
                 * @param code OpCode.
                 * @param type Function associated with that opcode.
                 * @param argcount number of arguments to expect.
                 */
                OpCode(_Domain code, FunctionType const& type, int argcount)
                    : m_code(code)
                {
                    m_long = type.value() | (argcount << 24);
                }

                /**
                 * Initializes a new OpCode which doesn't require any additional data.
                 * @param code OpCode.
                 */
                OpCode(_Domain code)
                    : m_code(code)
                {
                }

                /**
                 * Converts the stored value into a function type.
                 * @return Returns the converted function type.
                 */
                FunctionType const toFunction() const
                {
                    return FunctionType(static_cast<FunctionType::_Domain>(m_long & 0x0000FFFF));
                }

                /**
                 * Returns the stored number of arguments.
                 * @return The stored number of arguments.
                 */
                int argcount() const
                {
                    return ((m_long >> 24) & 0xFF);
                }

                union
                {
                    real_type m_real;
                    long_type m_long; 
                };
                _Domain m_code;
            };

            typedef std::vector<OpCode> OpCodeCollection;

        private:
            OpCodeCollection m_code;
    };

    inline CodeInterpreter::CodeInterpreter()
    {}

    inline void CodeInterpreter::emitPushLong(long value)
    {
        m_code.push_back(OpCode(OpCode::PushLong, value));
    }

    inline void CodeInterpreter::emitPushReal(double value)
    {
        m_code.push_back(OpCode(OpCode::PushReal, value));
    }

    inline void CodeInterpreter::emitPushIt()
    {
        m_code.push_back(OpCode(OpCode::PushIt));
    }

    inline void CodeInterpreter::emitAdd()
    {
        m_code.push_back(OpCode(OpCode::Add));
    }

    inline void CodeInterpreter::emitSubtract()
    {
        m_code.push_back(OpCode(OpCode::Sub));
    }

    inline void CodeInterpreter::emitMultiply()
    {
        m_code.push_back(OpCode(OpCode::Mul));
    }

    inline void CodeInterpreter::emitDivide()
    {
        m_code.push_back(OpCode(OpCode::Div));
    }

    inline void CodeInterpreter::emitLongDivide()
    {
        m_code.push_back(OpCode(OpCode::Idiv));
    }

    inline void CodeInterpreter::emitModulo()
    {
        m_code.push_back(OpCode(OpCode::Mod));
    }

    inline void CodeInterpreter::emitCall(FunctionType const& function, int argcount)
    {
        m_code.push_back(OpCode(OpCode::Call, function, argcount));
    }

    inline void CodeInterpreter::emitNegate()
    {
        m_code.push_back(OpCode(OpCode::Negate));
    }

    template<typename ValueType>
    inline ValueType CodeInterpreter::compute(ValueType value) const
    {
        ValueStack::value_type it = value;
        ValueStack stack;

        auto first = std::begin(m_code);
        auto const last = std::end(m_code);
        for (; first != last; ++first)
        {
            auto& code = *first;
            switch(code.m_code)
            {
                case OpCode::Add:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = x + y;
                    stack.push(r);
                    break;
                }
                case OpCode::Sub:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = x - y;
                    stack.push(r);
                    break;
                }
                case OpCode::Mul:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = x * y;
                    stack.push(r);
                    break;
                }
                case OpCode::Div:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = x / y;
                    stack.push(r);
                    break;
                }
                case OpCode::Idiv:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = detail::ValueStackItemOperation::idiv(x, y);
                    stack.push(r);
                    break;
                }
                case OpCode::Mod:
                {
                    auto const y = stack.pop();
                    auto const x = stack.pop();
                    auto const r = x % y;
                    stack.push(r);
                    break;
                }
                case OpCode::PushIt:
                    stack.push(it);
                    break;
                case OpCode::PushLong:
                    stack.push(code.m_long);
                    break;
                case OpCode::PushReal:
                    stack.push(code.m_real);
                    break;
                case OpCode::Negate:
                {
                    auto const x = stack.pop();
                    auto const r = -x;
                    stack.push(r);
                    break;
                }
                case OpCode::Call:
                {
                    auto const type = code.toFunction();
                    auto const args = code.argcount();
                    switch(type.value())
                    {
                        case FunctionType::Exp:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::exp(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Pow:
                        {
                            auto const y = stack.pop<real_type>();
                            auto const x = stack.pop<real_type>();
                            auto const r = std::pow(x, y);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Cos:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::cos(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Sin:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::sin(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Tan:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::tan(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Acos:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::acos(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Asin:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::asin(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Atan:
                        {
                            if (args == 1)
                            {
                                auto const x = stack.pop<real_type>();
                                auto const r = std::atan(x);
                                stack.push(r);
                            }
                            else if (args == 2)
                            {
                                auto const y = stack.pop<real_type>();
                                auto const x = stack.pop<real_type>();
                                auto const r = std::atan2(y, x);
                                stack.push(r);
                            }

                            break;
                        }
                        case FunctionType::Cosh:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::cosh(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Sinh:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::sinh(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Tanh:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::tanh(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Int:
                        {
                            auto const x = stack.pop<long_type>();
                            stack.push(x);
                            break;
                        }
                        case FunctionType::Float:
                        {
                            auto const x = stack.pop<real_type>();
                            stack.push(x);
                            break;
                        }
                        case FunctionType::Log:
                        {
                            if (args == 1)
                            {
                                auto const x = stack.pop<real_type>();
                                auto const r = std::log(x);
                                stack.push(r);
                            }
                            else if (args == 2)
                            {
                                auto const y = stack.pop<real_type>();
                                auto const x = stack.pop<real_type>();
                                auto const a = std::log(x);
                                auto const b = std::log(y);
                                auto const r = a / b;
                                stack.push(r);
                            }

                            break;
                        }
                        case FunctionType::Ln:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::log(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Round:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::floor(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Ceil:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::ceil(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Sqrt:
                        {
                            auto const x = stack.pop<real_type>();
                            auto const r = std::sqrt(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Abs:
                        {
                            auto const x = stack.pop();
                            auto const r = detail::ValueStackItemOperation::abs(x);
                            stack.push(r);
                            break;
                        }
                        case FunctionType::Sgn:
                        {
                            auto const x = stack.pop();
                            auto const r = detail::ValueStackItemOperation::sgn(x);
                            stack.push(r);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        if (stack.empty())
        {
            return it.toValueType<ValueType>();
        }
        else
        {
            auto const result = stack.pop();
            return result.toValueType<ValueType>();
        }
    }
}
}

#endif  // __LIBFORMULA_UTIL_CODEINTERPRETER_HPP
