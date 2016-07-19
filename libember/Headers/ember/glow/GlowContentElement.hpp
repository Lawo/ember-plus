/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_CONTENTELEMENT_HPP
#define __LIBEMBER_GLOW_CONTENTELEMENT_HPP

#include "../ber/Value.hpp"
#include "../dom/Set.hpp"
#include "../dom/VariantLeaf.hpp"
#include "util/Find.hpp"
#include "GlowElement.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowContentElement;

    /**
     * Helper class that initializes a content element when it is being accessed.
     * The content element is a set which contains optional properties of a node 
     * or a parameter.
     */
    class LIBEMBER_API Contents 
    {
        friend class GlowContentElement;
        public:
            typedef unsigned int flag_type;

            typedef dom::Set::iterator iterator;
            typedef dom::Set::const_iterator const_iterator;
            typedef dom::Set::size_type size_type;

            /**
             * Returns the number of nodes this container stores.
             * @return The number of nodes this container stores.
             */
            size_type size() const;

            /**
             * Returns the first element of the content set.
             * @return The first content element.
             */
            iterator begin();

            /**
             * Returns the end of the content set.
             * @return The end of the content element.
             */
            iterator end();

            /**
             * Returns the first element of the content set.
             * @return The first content element.
             */
            const_iterator begin() const;

            /**
             * Returns the end of the content set.
             * @return The end of the content element.
             */
            const_iterator end() const;

            /**
             * Adds an Ember Container to the content set.
             * @param value the ember element to add.
             */
            void set(dom::Container* value);

            /**
             * Adds or changes the leaf node with the provided application tag and value.
             * @param tag The application tag of the leaf to add or update.
             * @param value The value to set.
             */
            template<typename ValueType>
            void set(ber::Tag const& tag, ValueType value);

            /**
             * Searches for a VariantLeaf with the specified application tag and returns its value.
             * @param tag The tag of the node to get the value from.
             * @return The node's value if found, otherwise a Value in an irregular state will be returned.
             */
            ber::Value get(ber::Tag const& tag) const;

            /**
             * Checks if the passed property exists in the content set.
             * @param property Property to look for.
             * @return True if the property exists, otherwise false.
             * @note The PropertyType must have a field named value.
             */
            template<typename PropertyType>
            bool contains(PropertyType const& property) const;

        private:
            /**
             * Initializes a new Content instance.
             * @param parent The parent node where the content set will be
             *      added to if required.
             * @param contentTag The tag of the set containing the optional contents
             *      of this container.
             */
            Contents(GlowContentElement& parent, ber::Tag const& contentTag);

            /**
             * Assures that the content set exists.
             */
            void assureContainer() const;

            /**
             * Scans the collection for available properties and returns flags
             * indicating which properties exist.
             * The flag for each property is constructed by left-shifting 1 by the value of the property.
             * @param first First element of the container to traverse.
             * @param last End of the container.
             * @return Returns a mask indicating the available properties.
             */
            flag_type generatePropertyFlags(const_iterator first, const_iterator last) const;

            /** Prohibit assignment */
            Contents& operator=(Contents const&);

        private:
            GlowContentElement& m_parent;
            ber::Tag m_contentTag;
            mutable flag_type m_propertyFlags;
            mutable dom::Set* m_container;
    };


    /**
     * Base class for types that require an additional content set with optional
     * properties, like Node and Parameter.
     */
    class LIBEMBER_API GlowContentElement : public GlowElement
    {
        protected:
            /**
             * Initializes a new Content element with the specified glow type and 
             * application tag.
             * @param type The glow type to set.
             * @param tag The application tag.
             * @param contentTag The application tag the content set shall be created with.
             */
            GlowContentElement(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentTag);

            /**
             * Returns the content element which contains optional properties. 
             * The content set will be created automatically if it doesn't already exist.
             * @return The element's content.
             */
            Contents& contents();

            /**
             * Returns the content element which contains optional properties. 
             * The content set will be created automatically if it doesn't already exist.
             * @return The element's content.
             */
            const Contents& contents() const;

        private:
            Contents m_contents;
    };


    /**************************************************************************
     * Inline Implementation                                                  *
     **************************************************************************/

    template<typename PropertyType>
    inline bool Contents::contains(PropertyType const& property) const
    {
        assureContainer();
        bool const result = (m_propertyFlags & (1 << property.value())) != 0;
        return result;
    }

    inline Contents::size_type Contents::size() const
    {
        assureContainer();
        return m_container->size();
    }

    template<typename ValueType>
    inline void Contents::set(ber::Tag const& tag, ValueType value)
    {
        iterator const first = begin();
        iterator const last = end();
        iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            dom::VariantLeaf* node = dynamic_cast<dom::VariantLeaf*>(&*result);
            if (node != 0)
                node->setValue(value);
            else
                return;
        }
        else
        {
            if (m_container != 0)
                m_container->insert(last, new dom::VariantLeaf(tag, value));
            else
                return;
        }

        m_propertyFlags |= (tag.getClass() == ber::Class::ContextSpecific ? (1 << tag.number()) : 0);
    }

    inline void Contents::set(dom::Container* value)
    {
        ber::Tag const tag = value->applicationTag();
        iterator const where = end();
        if (m_container != 0)
        {
            m_container->insert(where, value);
            m_propertyFlags |= (tag.getClass() == ber::Class::ContextSpecific ? (1 << tag.number()) : 0);
        }
    }

    inline ber::Value Contents::get(ber::Tag const& tag) const
    {
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            dom::VariantLeaf const* node = dynamic_cast<dom::VariantLeaf const*>(&*result);
            if (node != 0)
            {
                return node->value();
            }
        }

        return ber::Value();
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowContentElement.ipp"
#endif

#endif  // __LIBEMBER_GLOW_CONTENTELEMENT_HPP
