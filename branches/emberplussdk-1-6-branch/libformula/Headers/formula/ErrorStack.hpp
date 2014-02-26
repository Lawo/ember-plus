#ifndef __LIBFORMULA_ERRORSTACK_HPP
#define __LIBFORMULA_ERRORSTACK_HPP

#include <vector>
#include "Error.hpp"

namespace libformula
{
    /**
     * This class contains all error that are reported by the scanner or the parser.
     */
    class ErrorStack
    {
        typedef std::vector<Error> ErrorCollection;
        public:
            typedef ErrorCollection::size_type size_type;
            typedef ErrorCollection::const_iterator const_iterator;

            /** 
             * Initializes a new ErrorStack instance.
             */
            ErrorStack();

            /**
             * Returns an iterator pointing to the first error that has been reported.
             * @return An iterator pointing to the first error that has been reported.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator pointing to the first item beyond the error collection.
             * @return An iterator pointing to the first item beyond the error collection.
             */
            const_iterator end() const;

            /**
             * Returns the number of errors this stack contains.
             * @return The number of errors.
             */
            size_type size() const;

            /**
             * Returns true if the stack is empty.
             * @return true if the stack is empty.
             */
            bool empty() const;

            /**
             * Returns true if the stack contains at least one item.
             * @return true if the stack contains at least one item.
             */
            bool any() const;

            /**
             * Resets the error stack
             */
            void clear();

            /**
             * Inserts the provided error description into the error stack.
             * @param error The error to insert.
             */
            void push(Error const& error);

            /**
             * Creates a new error description and inserts it into the error stack.
             * @param error The error type.
             * @param symbol The symbol that determines where the error occurred.
             * @param message Additional error description.
             */
            template<typename InputIterator>
            void push(ErrorType const& error, Symbol<InputIterator> const& symbol, std::string const& message);

            /**
             * Creates a new error description from the provided information and inserts it into the stack.
             * @param error The error type.
             * @param message A textual error description.
             */
            void push(ErrorType const& error, std::string const& message);

        protected:
            /**
             * This method is called whenever a new error is inserted. It can be used for additional logging
             * or any other purpose. The default implementation is empty.
             * @param error The error item that has been added.
             */
            virtual void pushOverride(Error const& error);

        private:
            ErrorCollection m_errors;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline ErrorStack::ErrorStack()
    {}

    inline ErrorStack::const_iterator ErrorStack::begin() const
    {
        return m_errors.begin();
    }

    inline ErrorStack::const_iterator ErrorStack::end() const
    {
        return m_errors.end();
    }

    inline ErrorStack::size_type ErrorStack::size() const
    {
        return m_errors.size();
    }

    inline bool ErrorStack::empty() const
    {
        return size() == 0;
    }

    inline bool ErrorStack::any() const
    {
        return size() > 0;
    }

    inline void ErrorStack::push(Error const& error)
    {
        m_errors.push_back(error);
        pushOverride(error);
    }

    template<typename InputIterator>
    inline void ErrorStack::push(ErrorType const& error, Symbol<InputIterator> const& symbol, std::string const& message)
    {
        push(Error(error, symbol, message));
    }

    inline void ErrorStack::push(ErrorType const& error, std::string const& message)
    {
        push(Error(error, message));
    }

    inline void ErrorStack::clear()
    {
        m_errors.clear();
    }

    inline void ErrorStack::pushOverride(Error const& error)
    {}
}

#endif  // __LIBFORMULA_ERRORSTACK_HPP
