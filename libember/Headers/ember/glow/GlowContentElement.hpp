#ifndef __LIBEMBER_GLOW_CONTENTELEMENT_HPP
#define __LIBEMBER_GLOW_CONTENTELEMENT_HPP

#include "../dom/Set.hpp"
#include "GlowElement.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    struct GlowProperty;

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
            typedef std::size_t flag_type;

            typedef dom::Set::iterator iterator;
            typedef dom::Set::const_iterator const_iterator;

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
             * Inserts a child node into the set.
             * @param where the location where the node shall be inserted.
             * @param child The node to insert.
             */
            void insert(iterator where, dom::Node* child);

            /**
             * Checks if the passed property exists in the content set.
             * @property Property to look for.
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

        private:
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


    /******************************************************
     * Inline Implementation                              *
     ******************************************************/
    template<typename PropertyType>
    inline bool Contents::contains(PropertyType const& property) const
    {
        assureContainer();

        bool const result = (m_propertyFlags & (1 << property.value)) != 0;
        return result;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowContentElement.ipp"
#endif

#endif  // __LIBEMBER_GLOW_CONTENTELEMENT_HPP
