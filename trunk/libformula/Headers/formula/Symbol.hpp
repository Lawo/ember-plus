#ifndef __LIBFORMULA_SYMBOL_HPP
#define __LIBFORMULA_SYMBOL_HPP

#include "Types.hpp"
#include "SymbolType.hpp"
#include "traits/StringStreamConverter.h"

namespace libformula
{
    /**
     * Represents a symbol identified by the scanner. 
     */
    template<typename InputIterator>
    struct Symbol
    {
        public:
            typedef typename InputIterator const_iterator;
            typedef std::size_t index_type;

            /**
             * Initializes a new symbol
             * @param type The symbol type.
             * @param token The index of the symbol.
             * @param first Points to the first character of the term string that belongs
             *      to this token.
             * @param last Points to the first character that does not belong to this token.
             */
            Symbol(SymbolType const& type, index_type token, InputIterator first, InputIterator last);

            /**
             * Returns the symbol type.
             * @return The symbol type.
             */
            SymbolType const& type() const;

            /**
             * Returns the symbol index.
             * @return The symbol index.
             */
            index_type token() const;

            /**
             * Converts the provided substring into a value of the specified type.
             * This method should only be called when the symbol type is IntegerValue
             * or RealValue, this method does not perform any type or security checks!
             * @return Returns the converted value.
             */
            template<typename ValueType>
            ValueType toValueType() const;

            /**
             * Returns the first item of the substring identifying this symbol.
             * @return The first item of the substring identifying this symbol.
             */
            const_iterator begin() const;

            /**
             * Returns the first item beyond the substring identifying this symbol.
             * @return The first item beyond the substring identifying this symbol.
             */
            const_iterator end() const;

        private:
            SymbolType m_type;
            int m_token;
            const_iterator m_begin;
            const_iterator m_end;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline Symbol<InputIterator>::Symbol(SymbolType const& type, index_type token, InputIterator first, InputIterator last)
        : m_type(type)
        , m_token(token)
        , m_begin(first)
        , m_end(last)
    {
    }

    template<typename InputIterator>
    inline SymbolType const& Symbol<InputIterator>::type() const
    {
        return m_type;
    }

    template<typename InputIterator>
    inline typename Symbol<InputIterator>::index_type Symbol<InputIterator>::token() const
    {
        return m_token;
    }

    template<typename InputIterator>
    inline typename Symbol<InputIterator>::const_iterator Symbol<InputIterator>::begin() const
    {
        return m_begin;
    }

    template<typename InputIterator>
    inline typename Symbol<InputIterator>::const_iterator Symbol<InputIterator>::end() const
    {
        return m_end;
    }

    template<typename InputIterator>
    template<typename ValueType>
    ValueType Symbol<InputIterator>::toValueType() const
    {
        auto const first = std::begin(*this);
        auto const last = std::end(*this);
        auto const result = traits::StringStreamConverter<ValueType>::parse(first, last);
        return result;
    }
}

#endif  // __LIBFORMULA_SYMBOL_HPP
