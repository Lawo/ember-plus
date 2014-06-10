/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_UTIL_VALIDATION_HPP
#define __LIBEMBER_GLOW_UTIL_VALIDATION_HPP

#include <locale>
#include <stdexcept>
#include <string>

namespace libember { namespace glow { namespace util
{
    /**
     * This class provides several evaluation methods.
     */
    class Validation
    {
        public:
            /**
             * Tests whether a identifier string contains any invalid characters. The identifier 
             * must not be empty and must not start with a digit. Additionally, it may not contain
             * the '/' character.
             * @param identifier The identifier to evaluate.
             * @return True if the identifier is valid, otherwise false.
             */
            static bool isIdentifierValid(std::string const& identifier);

            /**
             * Tests whether a identifier string contains any invalid characters. The identifier 
             * must not be empty and must not start with a digit. Additionally, it may not contain
             * the '/' character.
             * @param first A reference to the first character of the identifier string.
             * @param last A reference to the termination character, which is the first item 
             *      beyond the string.
             * @return True if the identifier is valid, otherwise false.
             */
            template<typename InputIterator>
            static bool isIdentifierValid(InputIterator first, InputIterator last);

            /**
             * Evaluates the passed identifier and throws a std::runtime_exception when
             * the identifier is of an invalid format.
             * @param identifier The identifier to evaluate.
             * @throw std::runtime_error when the provided identifier contains an invalid
             *      character.
             */
            static void assertIdentifierValid(std::string const& identifier);

            /**
             * Evaluates the passed identifier and throws a std::runtime_exception when
             * the identifier is of an invalid format.
             * @param first A reference to the first character of the identifier string to evaluate.
             * @param last A reference to the termination character, which is the first item 
             *      beyond the string.
             * @throw std::runtime_error when the provided identifier contains an invalid
             *      character.
             */
            template<typename InputIterator>
            static void assertIdentifierValid(InputIterator first, InputIterator last);

        private:
            /**
             * Tests whether the first character of an identifier is valid. An identifier
             * must start with a character from a to z or A to Z, or with an underscore.
             * @param character The character to evaluate.
             * @return true if the character is valid, otherwise false.
             */
            static bool isFirstCharacterValid(char character);
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline bool Validation::isIdentifierValid(std::string const& identifier)
    {
        return isIdentifierValid(identifier.begin(), identifier.end());
    }

    template<typename InputIterator>
    inline bool Validation::isIdentifierValid(InputIterator first, InputIterator last)
    {
        bool const isEmpty = first == last;
        if (isEmpty == false && isFirstCharacterValid(*first))
        {
            for ( ; first != last; ++first)
            {
                if (*first == '/')
                    return false;
            }
            return true;
        }
        return false;
    }

    inline void Validation::assertIdentifierValid(std::string const& identifier)
    {
        assertIdentifierValid(identifier.begin(), identifier.end());
    }

    template<typename InputIterator>
    inline void Validation::assertIdentifierValid(InputIterator first, InputIterator last)
    {
        bool const isEmpty = first == last;
        if (isEmpty)
            throw std::runtime_error("Identifier must not be empty");

        if (isFirstCharacterValid(*first) == false)
            throw std::runtime_error("Identifier must begin with a letter or the '_'");

        for ( /* Nothing */ ; first != last; ++first)
        {
            if (*first == '/')
                throw std::runtime_error("Identifier must not contain the '/' character");
        }
    }

    inline bool Validation::isFirstCharacterValid(char character)
    {
        return (character >= 'a' && character <= 'z')
            || (character >= 'A' && character <= 'Z')
            || (character == '_');
    }
}
}
}

#endif  // __LIBEMBER_GLOW_UTIL_VALIDATION_HPP
