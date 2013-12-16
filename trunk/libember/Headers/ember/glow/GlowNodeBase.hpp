/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_GLOWNODEBASE_HPP
#define __LIBEMBER_GLOW_GLOWNODEBASE_HPP

#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "NodeProperty.hpp"

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
             * Sets the schema identifier string.
             * @param identifier The name of the schema this node belongs to.
             */
            void setSchemaIdentifier(std::string const& identifier);

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
             * Returns the schema identifier string.
             * @return The schema identifier string.
             */
            std::string schemaIdentifier() const;

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
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowNodeBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWNODEBASE_HPP

