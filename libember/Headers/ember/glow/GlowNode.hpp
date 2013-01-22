#ifndef __LIBEMBER_GLOW_GLOWNODE_HPP
#define __LIBEMBER_GLOW_GLOWNODE_HPP

#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"

namespace libember { namespace glow
{
    /** Forward declaration **/
    class GlowNodeFactory;

    /**
     * Class representing a single node.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowNode : public GlowContentElement
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Constructor, initializes a Glow node with an application tag.
             * @note The application tag will be set to ContextSpecific-0.
             * @param number The internal node number.
             */
            explicit GlowNode(int number);

            /**
             * Constructor, initializes a Glow node with an application tag.
             * @param number The internal node number.
             * @param tag Application tag.
             */
            GlowNode(int number, ber::Tag const& tag);

            /**
             * Tests if the node contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(GlowProperty const& property) const;

            /**
             * Sets the description string.
             * @param description The description string to set.
             */
            void setDescription(std::string const& description);

            /**
             * Sets the identifir string.
             * @param identifier The identifier string.
             */
            void setIdentifier(std::string const& identifier);

            /**
             * Returns a modifiable element collection that contains the children.
             * The element will be inserted if it doesn't already exist.
             * @return The element collection.
             */
            GlowElementCollection* children();

            /**
             * Returns the number of this node.
             * @return The node number.
             */
            int number() const;

            /**
             * Returns the description string.
             * @return The description string.
             */
            std::string description() const;

            /**
             * Returns the identifier string.
             * @return The identifier string.
             */
            std::string identifier() const;

            /**
             * Returns the constant element collection. If no children are attached,
             * this method returns null.
             * @return Element collection containing the children of this node.
             */
            GlowElementCollection const* children() const;

        private:
            /**
             * This constructor initializes a node without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowNode(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowNode.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWNODE_HPP

