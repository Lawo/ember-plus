/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_TAG_HPP
#define __LIBEMBER_BER_TAG_HPP

#include "../util/Api.hpp"
#include "Class.hpp"

namespace libember { namespace ber
{
    /**
     * A type representing a BER tag separated into its class and number.
     */
    class LIBEMBER_API Tag
    {
        public:
            typedef unsigned long Number;
            typedef unsigned char Preamble;

        public:
            /**
             * Default constructor. Initializes the tag in a singular state.
             */
            Tag();

            /**
             * Constructor that initializes the tag with the specified class
             * and number.
             * @param berClass the class of the tag.
             * @param tagNumber the raw tag number within the class specified
             *      by @p berClass.
             */
            Tag(Class berClass, Number tagNumber);

            /**
             * Raw constructor that initializes the tag with the specified
             * preamble and number.
             * @param preamble the raw preamble of the tag that, in addition
             *      to the tags class, may contain a set container bit.
             * @param tagNumber the raw tag number within the class specified
             *      by @p preamble.
             */
            Tag(Preamble preamble, Number tagNumber);

            /**
             * Return the raw preamble of this tag.
             * @return The raw preamble of this tag.
             */
            Preamble preamble() const;

            /**
             * Return the number of this tag.
             * @return The raw number of this tag.
             */
            Number number() const;

            /**
             * Return whether or not this tag is marking a container
             * frame.
             * @return True if this tag is currently marking a container
             *      frame, otherwise false.
             */
            bool isContainer() const;

            /**
             * Set whether or not this tag should mark a container frame.
             * @param value a flag indicating whether or not the tag should
             *      mark a container frame.
             */
            void setContainer(bool value);

            /**
             * Create a copy of this tag with the container bit set.
             * @return A copy of this tag with the container bit set.
             */
            Tag toContainer() const;

            /**
             * Return the class of this tag extracted from its preamble.
             * @return The class of this tag extracted from its preamble.
             */
            Class getClass() const;

            /**
             * Set the class of this tag to @p berClass, leaving the other
             * bits stored within the pramble untouched.
             */
            void setClass(ber::Class berClass);

        private:
            Preamble m_preamble;
            Number m_number;
    };

    /**
     * Generator function to make generating a tag more syntactically
     * convenient.
     */
    LIBEMBER_API
    Tag make_tag(Tag::Preamble preamble, Tag::Number number);

    /**
     * Overloaded equality comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs and @p rhs are equal, otherwise false.
     */
    LIBEMBER_API
    bool operator==(Tag const& lhs, Tag const& rhs);

    /**
     * Overloaded inequality comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs and @p rhs are not equal, otherwise false.
     */
    LIBEMBER_API
    bool operator!=(Tag const& lhs, Tag const& rhs);

    /**
     * Overloaded less than comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs is less than @p rhs, otherwise false.
     */
    LIBEMBER_API
    bool operator<(Tag const& lhs, Tag const& rhs);

    /**
     * Overloaded less than or equal to comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs is less than or equal to @p rhs, otherwise false.
     */
    LIBEMBER_API
    bool operator<=(Tag const& lhs, Tag const& rhs);

    /**
     * Overloaded greater than comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs is greater than @p rhs, otherwise false.
     */
    LIBEMBER_API
    bool operator>(Tag const& lhs, Tag const& rhs);

    /**
     * Overloaded greater than or equal to comparison operator.
     * @param lhs a constant reference a tag to compared.
     * @param rhs a constant reference a tag to compared.
     * @return True if @p lhs is greater than or equal to @p rhs, otherwise false.
     */
    LIBEMBER_API
    bool operator>=(Tag const& lhs, Tag const& rhs);
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Tag.ipp"
#endif

#endif  // __LIBEMBER_BER_TAG_HPP
