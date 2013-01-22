#ifndef __LIBFORMULA_TERMCOMPILER_HPP
#define __LIBFORMULA_TERMCOMPILER_HPP

#include "Term.hpp"

namespace libformula
{
    /**
     * TermCompiler, this class is used the create an executable term from a term-string.
     */
    class TermCompiler
    {
        public:
            /**
             * Compiles a term and returns the compilaton result.
             * @param first Pointer to the first character of the term string.
             * @param last Points to the first item beyond the term string.
             * @param error Error stack, must not be nullptr.
             */
            template<typename InputIterator>
            static CompiledTerm compile(InputIterator first, InputIterator last, ErrorStack* error)
            {
                auto const term = CompiledTerm(first, last, error);
                return term;
            }

            /**
             * Compiles a term and returns the compilaton result.
             * @param first Pointer to the first character of the term string.
             * @param last Points to the first item beyond the term string.
             */
            template<typename InputIterator>
            static CompiledTerm compile(InputIterator first, InputIterator last)
            {
                auto error = ErrorStack();
                auto const term = compile(first, last, &error);
                return term;
            }

            /**
             * Compiles a term and returns the compilaton result.
             * @param term Termstring to compile.
             * @param error Error stack, must not be nullptr.
             */
            static CompiledTerm compile(std::string const& term, ErrorStack* error)
            {
                auto const first = std::begin(term);
                auto const last = std::end(term);
                return compile(first, last, error);
            }

            /**
             * Compiles a term and returns the compilaton result.
             * @param term Termstring to compile.
             */
            static CompiledTerm compile(std::string const& term)
            {
                auto error = ErrorStack();
                auto const first = std::begin(term);
                auto const last = std::end(term);
                return compile(first, last, &error);
            }
    };
}

#endif  // __LIBFORMULA_TERMCOMPILER_HPP
