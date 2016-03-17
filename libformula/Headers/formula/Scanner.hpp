#ifndef __LIBFORMULA_SCANNER_HPP
#define __LIBFORMULA_SCANNER_HPP

#include <string>
#include "ErrorStack.hpp"
#include "Symbol.hpp"

namespace libformula
{
    /**
     * The Scanner is iterating through the term string and identifies the single tokens which
     * are used for parsing in the second pass.
     */
    template<typename InputIterator>
    class Scanner
    {
        typedef std::vector<Symbol<InputIterator>> SymbolContainer;
        public:
            typedef typename SymbolContainer::value_type value_type;
            typedef typename SymbolContainer::size_type size_type;
            typedef typename SymbolContainer::const_iterator const_iterator;
            typedef typename SymbolContainer::reference reference;
            typedef typename SymbolContainer::const_reference const_reference;
            typedef typename InputIterator input_type;

            /**
             * Initializes a new scanner which immediately scans the provided term.
             * @param first Pointer to the first character in the term buffer.
             * @param last Points to the first item beyond the term to scan.
             * @param error Pointer to the error stack, where all detected errors are logged.
             * @note Note that the provided buffer must exist as long as the scanner instance
             *      is alive!
             */
            Scanner(InputIterator first, InputIterator last, ErrorStack *error);

            /**
             * Returns the number of tokens identified and created by the scanner.
             */
            size_type size() const;

            /**
             * Returns an iterator that points to the first symbol created by the scanner.
             * @return An iterator that points to the first symbol created by the scanner.
             */
            const_iterator begin() const;

            /**
             * Returns the iterator that points to the first location beyond the symbol buffer.
             * @return The iterator that points to the first location beyond the symbol buffer.
             */
            const_iterator end() const;

        private:
            /**
             * Inserts a symbol to the internal symbol buffer.
             * @param symbol The symbol to add.
             */
            void push(const_reference symbol);

            /**
             * Scans a single character symbol and adds the identifies symbol into the buffer.
             * @param first Current character.
             * @param last End of the provided term string.
             * @param error Error stack.
             */
            InputIterator scanSingleCharSymbol(InputIterator first, InputIterator last, ErrorStack *error);

            /**
             * Tries to identify a symbol that consists of several characters, like a number or a function call.
             * @param first Current character.
             * @param last End of the provided term string.
             * @param error Error stack.
             */
            InputIterator scanMultiCharSymbol(InputIterator first, InputIterator last, ErrorStack *error);

            /**
             * Tries to match the current input to a function and inserts the symbol to the buffer.
             * @param first Current character.
             * @param last End of the provided term string.
             * @param error Error stack.
             */
            InputIterator scanFunction(InputIterator first, InputIterator last, ErrorStack *error);

            /**
             * Parses a number.
             * @param first Current character.
             * @param last End of the provided term string.
             * @param error Error stack.
             */
            InputIterator scanNumber(InputIterator first, InputIterator last, ErrorStack *error);

        private:
            SymbolContainer m_symbols;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline Scanner<InputIterator>::Scanner(InputIterator first, InputIterator last, ErrorStack *error)
    {
        while(first != last && isspace(*first))
            ++first;

        for( ; first != last; )
        {
            switch(*first)
            {
                case SymbolType::It:
                case SymbolType::Divide:
                case SymbolType::LParen:
                case SymbolType::Minus:
                case SymbolType::Modulo:
                case SymbolType::Multiply:
                case SymbolType::Plus:
                case SymbolType::Pow:
                case SymbolType::RParen:
                case SymbolType::Comma:
                    first = scanSingleCharSymbol(first, last, error);
                    break;

                default:
                    first = scanMultiCharSymbol(first, last, error);
                    break;
            }

            while(first != last && isspace(*first))
                ++first;
        }

        push(value_type(SymbolType::EndOfFile, size(), last, last));
    }

    template<typename InputIterator>
    inline typename Scanner<InputIterator>::size_type Scanner<InputIterator>::size() const
    {
        return m_symbols.size();
    }

    template<typename InputIterator>
    inline typename Scanner<InputIterator>::const_iterator Scanner<InputIterator>::begin() const
    {
        return m_symbols.begin();
    }

    template<typename InputIterator>
    inline typename Scanner<InputIterator>::const_iterator Scanner<InputIterator>::end() const
    {
        return m_symbols.end();
    }

    template<typename InputIterator>
    inline InputIterator Scanner<InputIterator>::scanSingleCharSymbol(InputIterator first, InputIterator last, ErrorStack *error)
    {
        push(value_type(static_cast<SymbolType::_Domain>(*first), size(), first, first + 1));
        return ++first;
    }

    template<typename InputIterator>
    inline InputIterator Scanner<InputIterator>::scanMultiCharSymbol(InputIterator first, InputIterator last, ErrorStack *error)
    {
        if (::isalpha(*first))
        {
            return scanFunction(first, last, error);
        }
        else if (::isdigit(*first))
        {
            return scanNumber(first, last, error);
        }
        else
        {
            error->push(ErrorType::UnknownToken, "Unknown token");
            return last;
        }
    }

    template<typename InputIterator>
    inline InputIterator Scanner<InputIterator>::scanFunction(InputIterator first, InputIterator last, ErrorStack *error)
    {
        typedef typename std::iterator_traits<InputIterator>::value_type type;
        type buffer[32];
        auto cursor = &buffer[0];
        auto it = first;
        while(it != last && ::isalpha(*it))
        {
            *cursor++ = ::tolower(*it++);
        }

        *cursor = 0;

        if (std::strncmp(buffer, "sqrt", 4) == 0)
            push(value_type(SymbolType::Sqrt, size(), first, it));
        else if (std::strncmp(buffer, "log", 3) == 0)
            push(value_type(SymbolType::Log, size(), first, it));
        else if (std::strncmp(buffer, "ln", 2) == 0)
            push(value_type(SymbolType::Ln, size(), first, it));
        else if (std::strncmp(buffer, "round", 5) == 0)
            push(value_type(SymbolType::Round, size(), first, it));
        else if (std::strncmp(buffer, "ceil", 4) == 0)
            push(value_type(SymbolType::Ceil, size(), first, it));
        else if (std::strncmp(buffer, "int", 3) == 0)
            push(value_type(SymbolType::Int, size(), first, it));
        else if (std::strncmp(buffer, "float", 5) == 0)
            push(value_type(SymbolType::Float, size(), first, it));
        else if (std::strncmp(buffer, "exp", 3) == 0)
            push(value_type(SymbolType::Exp, size(), first, it));
        else if (std::strncmp(buffer, "sinh", 4) == 0)
            push(value_type(SymbolType::Sinh, size(), first, it));
        else if (std::strncmp(buffer, "sin", 3) == 0)
            push(value_type(SymbolType::Sin, size(), first, it));
        else if (std::strncmp(buffer, "cosh", 4) == 0)
            push(value_type(SymbolType::Cosh, size(), first, it));
        else if (std::strncmp(buffer, "cos", 3) == 0)
            push(value_type(SymbolType::Cos, size(), first, it));
        else if (std::strncmp(buffer, "tanh", 4) == 0)
            push(value_type(SymbolType::Tanh, size(), first, it));
        else if (std::strncmp(buffer, "tan", 3) == 0)
            push(value_type(SymbolType::Tan, size(), first, it));
        else if (std::strncmp(buffer, "atan", 4) == 0)
            push(value_type(SymbolType::Atan, size(), first, it));
        else if (std::strncmp(buffer, "acos", 4) == 0)
            push(value_type(SymbolType::Acos, size(), first, it));
        else if (std::strncmp(buffer, "asin", 4) == 0)
            push(value_type(SymbolType::Asin, size(), first, it));
        else if (std::strncmp(buffer, "pi", 2) == 0)
            push(value_type(SymbolType::Pi, size(), first, it));
        else if (std::strncmp(buffer, "e", 1) == 0)
            push(value_type(SymbolType::E, size(), first, it));
        else if (std::strncmp(buffer, "abs", 3) == 0)
            push(value_type(SymbolType::Abs, size(), first, it));
        else if (std::strncmp(buffer, "sgn", 3) == 0)
            push(value_type(SymbolType::Sgn, size(), first, it));
        else
        {
            error->push(ErrorType::UnknownFunction, buffer);
            return last;
        }

        return it;
    }

    template<typename InputIterator>
    inline InputIterator Scanner<InputIterator>::scanNumber(InputIterator first, InputIterator last, ErrorStack *error)
    {
        auto it = first;
        auto exponents = 0;
        auto operators = 0;
        auto dots = 0;

        while(it != last && exponents < 2 && operators < 2 && dots < 2)
        {
            if (*it == 'E' || *it == 'e')
                ++exponents;
            else if (dots > 0 && (*it == '+' || *it == '-'))
                ++operators;
            else if (*it == '.')
                ++dots;
            else if (::isdigit(*it) == 0)
                break;

            ++it;
        }

        auto const invalid = !(exponents < 2 && dots < 2 && operators < 2);
        if (invalid == false)
        {
            if (dots == 0 && exponents == 0 && operators == 0)
            {
                push(value_type(SymbolType::IntegerValue, size(), first, it));
            }
            else
            {
                push(value_type(SymbolType::RealValue, size(), first, it));
            }
            return it;
        }
        else
        {
            error->push(ErrorType::TokenIsNotAValidNumber, std::string(first, it));
            return last;
        }
    }

    template<typename InputIterator>
    inline void Scanner<InputIterator>::push(const_reference symbol)
    {
        m_symbols.push_back(symbol);
    }
}

#endif  // __LIBFORMULA_SCANNER_HPP
