/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWNODEBASE_HPP
#define __LIBEMBER_GLOW_GLOWNODEBASE_HPP

#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "NodeProperty.hpp"
#include "util/CompliesWithSchema.hpp"

namespace libember { namespace glow
{
    /**
     * Class containing the common properties of a node.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowNodeBase : public GlowContentElement
    {
        public:
            /**
             * Tests if the node contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(NodeProperty const& property) const;

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
             * Sets the string containing the schema identifiers. The identifiers
             * are separated by the linefeed character (0x0A, '\n')
             * @param identifiers The names of the schema identifiers this node uses.
             */
            void setSchemaIdentifiers(std::string const& identifiers);

            /**
             * Sets the root property. This property may be used if a provider acts as proxy
             * who collects the data of several sub-devices. It may be required to mark these
             * sub-devices' nodes as root-node.
             * @param isRoot True to mark this node as root, otherwise false.
             */
            void setRoot(bool isRoot);

            /**
             * Adds the online state of the node.
             * @param isOnline The current online state of the node.
             */
            void setIsOnline(bool isOnline);
            
            /**
             * Inserts the object identifier to the template describing the structure
             * of this element.
             */
            void setTemplateReference(ber::ObjectIdentifier const& path);

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
             * Returns the string containing the schema identifiers. The identifiers
             * are separated with the line feed character (0x0A, '\n').
             * @return The string containing the schema identifiers.
             */
            std::string schemaIdentifiers() const;

            /**
             * Tests if the node complies with the specified schema.
             * @param schemaIdentifier The identifier of the schema to test.
             * @return true, if the schema is supported. Otherwise, this method
             *      return false.
             */
            bool compliesWithSchema(std::string const& schemaIdentifier) const;

            /**
             * Returns the root node flag
             * @return The root node flag
             */
            bool isRoot() const;

            /**
             * Returns the online state of the node, if available.
             * @return The online state of the node. If the property is not set, true 
             *      is being returned.
             */
            bool isOnline() const;

            /**
             * Returns the constant element collection. If no children are attached,
             * this method returns null.
             * @return Element collection containing the children of this node.
             */
            GlowElementCollection const* children() const;

            /**
             * Returns the object identifier of the template reference. If not present,
             * an empty oid is being returned.
             */
            ber::ObjectIdentifier templateReference() const;

        protected:
            /**
             * This constructor initializes a node with the specified content- and children-tags.
             * @param type Object type.
             * @param tag Application tag.
             * @param contentsTag The tag to use for the contents set.
             * @param childrenTag The tag to use for the children.
             */
            GlowNodeBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag);

        private:
            ber::Tag m_childrenTag;
    };


    /******************************************************
     * Inline implementation                              *
     ******************************************************/


    inline bool GlowNodeBase::compliesWithSchema(std::string const& schemaIdentifier) const
    {
        return util::complies_with_schema(schemaIdentifiers(), schemaIdentifier);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowNodeBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWNODEBASE_HPP

