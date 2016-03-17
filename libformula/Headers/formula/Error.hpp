#ifndef __LIBFORMULA_ERROR_HPP
#define __LIBFORMULA_ERROR_HPP

#include "ErrorType.hpp"
#include "Symbol.hpp"
#include "util/Util.hpp"

namespace libformula
{
    /**
     * Describes an error that occurred while scanning or parsing a term.
     */
    struct Error
    {
        public:
            /**
             * Initializes a new error.
             * @param type The error type.
             * @param symbol The symbol that identifies the location where the error occurred.
             * @param message Textual error description.
             */
            template<typename InputIterator>
            Error(ErrorType const& type, Symbol<InputIterator> const& symbol, std::string const& message)
                : m_type(type)
                , m_message(message)
                , m_token(symbol.token())
                , m_symbol(util::symbol_tostring(symbol))
            {}

            /**
             * Initializes a new error.
             * @param type The error type.
             * @param message Textual error description.
             */
            Error(ErrorType const& type, std::string const& message)
                : m_type(type)
                , m_message(message)
                , m_token(0)
            {}

            /**
             * Returns the error type.
             * @return The error type.
             */
            ErrorType const& type() const
            {
                return m_type;
            }

            /**
             * Returns the error message.
             * @return The error message.
             */
            std::string const& message() const
            {
                return m_message;
            }

            /**
             * Returns the symbol index.
             * @return The symbol index.
             */
            std::size_t token() const
            {
                return m_token;
            }

            /**
             * Returns the string representation of the symbol, if provided at construction.
             * @return The string representation of the symbol.
             */
            std::string const& symbol() const
            {
                return m_symbol;
            }

        private:
            ErrorType m_type;
            std::size_t m_token;
            std::string m_message;
            std::string m_symbol;
    };

}

#endif  // __LIBFORMULA_ERROR_HPP
