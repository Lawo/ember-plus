/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWFUNCTIONBASE_HPP
#define __LIBEMBER_GLOW_GLOWFUNCTIONBASE_HPP

#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "GlowTupleItemDescription.hpp"
#include "FunctionProperty.hpp"

namespace libember { namespace glow
{
    /**
     * Class containing the common properties of a function.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowFunctionBase : public GlowContentElement
    {
        public:
            /**
             * Tests if the node contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(FunctionProperty const& property) const;

            /**
             * Sets the identifier string.
             * @param identifier The identifier string.
             */
            void setIdentifier(std::string const& identifier);

            /**
             * Sets the description string.
             * @param description The description string to set.
             */
            void setDescription(std::string const& description);

            /**
             * Inserts the object identifier to the template describing the structure
             * of this element.
             */
            void setTemplateReference(ber::ObjectIdentifier const& path);

            /**
             * Returns the sequence containing the arguments. If the sequence does not
             * yet exist, it will be created. 
             * @return The sequence containing the arguments.
             */
            dom::Sequence* arguments();

            /**
             * Returns the sequence containing the result types. If the sequence does not
             * yet exist, it will be created. 
             * @return The sequence containing the result types.
             */
            dom::Sequence* result();

            /**
             * Returns a modifiable element collection that contains the children.
             * The element will be inserted if it doesn't already exist.
             * @return The element collection.
             */
            GlowElementCollection* children();

            /**
             * Returns the identifier string.
             * @return The identifier string.
             */
            std::string identifier() const;

            /**
             * Returns the description string.
             * @return The description string.
             */
            std::string description() const;

            /**
             * Returns the constant element collection. If no children are attached,
             * this method returns null.
             * @return Element collection containing the children of this node.
             */
            GlowElementCollection const* children() const;

            /**
             * Returns a sequence which contains the arguments this function has. If no arguments
             * are set, this method returns null.
             * @return A sequence containing the arguments of this function.
             */
            dom::Sequence const* arguments() const;

            /**
             * Copies all arguments that are of type GlowTupleItemDescription into the
             * passed output iterator.
             * @param dest The iterator to copy the valid arguments to.
             */
            template<typename OutputIterator>
            size_type typedArguments(OutputIterator dest) const;

            /**
             * Returns a sequence which contains the result types this function has. If no result types
             * are set, this method returns null.
             * @return A sequence containing the result types of this function.
             */
            dom::Sequence const* result() const;

            /**
             * Copies all result types that are of type GlowTupleItemDescription into the
             * passed output iterator.
             * @param dest The iterator to copy the valid result types to.
             */
            template<typename OutputIterator>
            size_type typedResult(OutputIterator dest) const;

            /**
             * Returns the object identifier of the template reference. If not present,
             * an empty oid is being returned.
             */
            ber::ObjectIdentifier templateReference() const;

        protected:
            /**
             * This constructor initializes a function with the specified content- and children-tags.
             * @param type Object type.
             * @param tag Application tag.
             * @param contentsTag The tag to use for the contents set.
             * @param childrenTag The tag to use for the children.
             */
            GlowFunctionBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag);

        private:
            ber::Tag m_childrenTag;
    };

    template<typename OutputIterator>
    inline GlowFunctionBase::size_type GlowFunctionBase::typedArguments(OutputIterator dest) const
    {
        dom::Sequence const* container = arguments();

        return container != 0
            ? util::TypeFilter<GlowTupleItemDescription>::collect(container->begin(), container->end(), dest)
            : 0;
    }

    template<typename OutputIterator>
    inline GlowFunctionBase::size_type GlowFunctionBase::typedResult(OutputIterator dest) const
    {
        dom::Sequence const* container = result();

        return container != 0
            ? util::TypeFilter<GlowTupleItemDescription>::collect(container->begin(), container->end(), dest)
            : 0;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowFunctionBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWFUNCTIONBASE_HPP

