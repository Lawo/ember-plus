#ifndef __LIBFORMULA_PARSER_HPP
#define __LIBFORMULA_PARSER_HPP

#include <cmath>
#include <sstream>
#include "CodeEmitter.hpp"
#include "ErrorStack.hpp"
#include "Scanner.hpp"

namespace libformula
{
    /**
     * The parser processes all provided symbols and emits the code required
     * to compute the mathematical term, as long as it is parsed correctly.
     */
    template<typename InputIterator>
    class Parser
    {
        typedef Scanner<InputIterator> scanner_type;
        typedef typename scanner_type::const_iterator const_iterator;
        public:
            /**
             * Initializes a new Parser instance which tries to generate the code required
             * to execute the term computation.
             * @param scanner The scanner containing the indentifier symbols.
             * @param code Reference to the code emitter interface.
             * @param error Reference to the error stack, where alls scanner and parser errors 
             *      will be logged.
             */
            Parser(scanner_type const& scanner, CodeEmitter* code, ErrorStack* error);

        private:
            /**
             * Parses a term, which may be two products that will be added or subtracted, or a single product.
             * @param code Code emitter.
             * @param error Error stack.
             */
            void term(CodeEmitter* code, ErrorStack* error);

            /**
             * Parses a product, which may be two expressions that will be multiplied or divided, or a single expression.
             * @param code Code emitter.
             * @param error Error stack.
             */
            void product(CodeEmitter* code, ErrorStack* error);

            /** 
             * Parses an expression, which is a single atom or an atom, followed by the pow operator and another expression.
             * @param code Code emitter.
             * @param error Error stack.
             */
            void expression(CodeEmitter* code, ErrorStack* error);

            /**
             * Parses an atom, which may be the variable (it), a number, a function call or a term put into parentheses.
             * @param code Code emitter.
             * @param error Error stack.
             */
            void atom(CodeEmitter* code, ErrorStack* error);

            /**
             * Parses a function, which contains an argument list.
             * @param code Code emitter.
             * @param error Error stack.
             */
            void func(CodeEmitter* code, ErrorStack* error, FunctionType const& type);

            /**
             * If the next symbol is a plus or minus character, this method will move to the next
             * symbol and return the sign of the operator, which is '-1' for '-' and +1 for any other
             * symbol.
             * @return Returns the sign of the current symbol, -1 for '-', +1 for any other symbol.
             */
            long sign();

        private:
            /**
             * Returns the next symbol.
             * @return The next symbol.
             */
            const_iterator next() const;

            /**
             * Tests whether another symbol is availble or if the parser already reached the end of the stream.
             * @return True when at least one more valid symbol is available, false otherwise.
             */
            bool peek() const;

            /**
             * Moves to the next symbol.
             */
            void move();

        private:
            const_iterator const m_last;
            const_iterator const m_first;
            scanner_type const& m_scanner;
            const_iterator m_current;
            bool m_error;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline Parser<InputIterator>::Parser(scanner_type const& scanner, CodeEmitter* code, ErrorStack* error)
        : m_last(scanner.end())
        , m_first(scanner.begin())
        , m_current(scanner.end())
        , m_scanner(scanner)
    {
        term(code, error);

        m_error = error->any();
    }

    template<typename InputIterator>
    inline typename Parser<InputIterator>::const_iterator Parser<InputIterator>::next() const
    {
        if (m_current != m_last)
        {
            return m_current + 1;
        }
        else
        {
            return m_first;
        }
    }

    template<typename InputIterator>
    inline bool Parser<InputIterator>::peek() const
    {
        auto const next = m_current != m_last
            ? m_current + 1
            : m_first;

        return next != m_last && next->type().value() != SymbolType::EndOfFile;
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::move()
    {
        if (m_current == m_last)
        {
            m_current = m_first;
        }
        else
        {
            ++m_current;
        }
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::term(CodeEmitter* code, ErrorStack* error)
    {
        product(code, error);

        while (peek())
        {
            auto const& symbol = *next();
            auto const type = symbol.type();
            if (type.value() == SymbolType::Plus)
            {
                move();
                product(code, error);
                code->emitAdd();
            }
            else if (type.value() == SymbolType::Minus)
            {
                move();
                product(code, error);
                code->emitSubtract();
            }
            else if (type.value() == SymbolType::Modulo)
            {
                move();
                product(code, error);
                code->emitModulo();
            }
            else
            {
                break;
            }
        }
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::product(CodeEmitter* code, ErrorStack* error)
    {
        expression(code, error);

        while (peek())
        {
            auto const& symbol = *next();
            auto const type = symbol.type();
            if (type.value() == SymbolType::Multiply)
            {
                move();
                expression(code, error);
                code->emitMultiply();
            }
            else if (type.value() == SymbolType::Divide)
            {
                move();
                expression(code, error);
                code->emitDivide();
            }
            else if (type.value() == SymbolType::LongDivide)
            {
                move();
                expression(code, error);
                code->emitLongDivide();
            }
            else
            {
                break;
            }
        }
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::expression(CodeEmitter* code, ErrorStack* error)
    {
        atom(code, error);
        while (peek())
        {
            auto const& symbol = *next();
            auto const type = symbol.type();
            if (type.value() == SymbolType::Pow)
            {
                move();
                atom(code, error);
                code->emitCall(FunctionType::Pow, 2);
            }
            else
            {
                break;
            }
        }
    }

    template<typename InputIterator>
    inline long Parser<InputIterator>::sign()
    {
        auto const& symbol = *next();
        auto const type = symbol.type();

        if(type.value() == SymbolType::Minus)
        {
            move();
            return -1;
        }
        else if (type.value() == SymbolType::Plus)
        {
            move();
        }

        return 1;
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::atom(CodeEmitter* code, ErrorStack* error)
    {
        if (peek())
        {
            auto const sign = this->sign();
            auto const& symbol = *next();
            auto const type = symbol.type();

            if (type.value() == SymbolType::IntegerValue)
            {
                auto const value = symbol.template toValueType<long_type>() * sign;
                code->emitPushLong(value);
                move();
            }
            else if (type.value() == SymbolType::RealValue)
            {
                auto const value = symbol.template toValueType<real_type>() * sign;
                code->emitPushReal(value);
                move();
            }
            else if (type.value() == SymbolType::It)
            {
                code->emitPushIt();
                move();

                if (sign < 0)
                    code->emitNegate();
            }
            else if (type.value() == SymbolType::Pi)
            {
                auto const pi = 3.14159265358979323846 * sign;
                code->emitPushReal(pi);
                move();
            }
            else if (type.value() == SymbolType::E)
            {
                auto const e =  2.71828182845904523536 * sign;
                code->emitPushReal(e);
                move();
            }
            else if (type.value() == SymbolType::LParen)
            {
                move();
                term(code, error);
                if (peek())
                {
                    auto const& rparen = *next();
                    if (rparen.type().value() == SymbolType::RParen)
                    {
                        move();

                        if (sign < 0)
                            code->emitNegate();
                        return;
                    }
                    else
                    {
                        error->push(ErrorType::UnexpectedToken, rparen, "Expected ')'");
                        return;
                    }
                }
            }
            else if(FunctionType::isFunction(symbol.type()))
            {
                auto const function = FunctionType::fromSymbol(symbol.type());
                move();
                func(code, error, function);

                if (sign < 0)
                    code->emitNegate();
            }
        }
    }

    template<typename InputIterator>
    inline void Parser<InputIterator>::func(CodeEmitter* code, ErrorStack* error, FunctionType const& type)
    {
        auto const minargs = type.minargs();
        auto const maxargs = type.maxargs();
        auto args = 0;

        if (peek())
        {
            auto const& lparen = *next();
            if (lparen.type().value() == SymbolType::LParen)
            {
                while(args < maxargs && peek())
                {
                    move();
                    term(code, error);
                    ++args;

                    if (peek())
                    {
                        auto const& symbol = *next();
                        if (symbol.type().value() == SymbolType::Comma)
                        {
                            if (args == maxargs)
                            {
                                // Invalid argcount
                                std::stringstream stream;
                                stream << "Invalid argument count, only " << maxargs << " args allowed";
                                error->push(ErrorType::InvalidArgumentCount, symbol, stream.str());
                                return;
                            }
                        }
                        else if (symbol.type().value() == SymbolType::RParen)
                        {
                            if (args >= minargs && args <= maxargs)
                            {
                                code->emitCall(type, args);
                                move();
                            }
                            else
                            {
                                std::stringstream stream;
                                if (minargs != maxargs)
                                {
                                    stream << "Invalid argument count, function requires " << minargs << " to " << maxargs << "arguments";
                                }
                                else
                                {
                                    stream << "Invalid argument count, function requires " << maxargs << " arguments";
                                }
                                error->push(ErrorType::InvalidArgumentCount, symbol, stream.str());
                            }

                            return;
                        }
                        else
                        {
                            error->push(ErrorType::UnexpectedToken, symbol, "Expected ',' or ')'");
                            return;
                        }
                    }
                    else
                    {
                        if (args < maxargs)
                        {
                            error->push(ErrorType::InvalidArgumentCount, lparen, "End of file reached, are you missing a ')'?");
                        }
                        else
                        {
                            error->push(ErrorType::UnexpectedEndOfFile, lparen, "End of file reached");
                        }
                        return;
                    }
                }
            }
            else
            {
                error->push(ErrorType::UnexpectedToken, lparen, "Expected '('");
                return;
            }
        }
    }
}

#endif  // __LIBFORMULA_PARSER_HPP
