#ifndef __LIBFORMULA_TERM_HPP
#define __LIBFORMULA_TERM_HPP

#include "CodeEmitter.hpp"
#include "ErrorStack.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"
#include "util/CodeInterpreter.hpp"
#include <memory>

namespace libformula
{
    /**
     * Represents a mathematical expression, which is constructed from
     * a provided string.
     * @note The template parameter must be a class implementing the 
     *      CodeEmitter interface. Additionaly, it must provided
     *      a templated function with the following name and
     *      signature: 
     *          template<typename DestType>
     *          DestType compute(DestType);
     */
    template<typename CodeEmitterType>
    class Term
    {
        public:
            /**
             * Initializes a new term by parsing and compiling the provided
             * term string.
             * @param first Points to the first item in the buffer containing
             *      the term to compile.
             * @param last Points to the first item beyond the buffer.
             * @param error A pointer to the error stack which collects errors
             *      that occur during scanning or parsing the term.
             */
            template<typename InputIterator>
            Term(InputIterator first, InputIterator last, ErrorStack* error);

            /**
             * Computes the term and returns the result.
             * @param value The value to use for the '$' symbol.
             * @return Returns the result of the computation.
             */
            template<typename ValueType>
            ValueType compute(ValueType value) const;

        private:
            std::shared_ptr<CodeEmitterType> m_emitter;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename CodeEmitterType>
    template<typename InputIterator>
    inline Term<CodeEmitterType>::Term(InputIterator first, InputIterator last, ErrorStack* error)
        : m_emitter(std::make_shared<CodeEmitterType>())
    {
        auto const scanner = Scanner<InputIterator>(first, last, error);
        if (error->empty())
        {
            auto emitter = m_emitter.get();
            auto const parser = Parser<InputIterator>(scanner, emitter, error);
        }
    }

    template<typename CodeEmitterType>
    template<typename ValueType>
    ValueType Term<CodeEmitterType>::compute(ValueType value) const
    {
        auto const result = m_emitter->compute(value);
        return result;
    }

    typedef Term<util::CodeInterpreter> CompiledTerm;
}

#endif  // __LIBFORMULA_TERM_HPP
