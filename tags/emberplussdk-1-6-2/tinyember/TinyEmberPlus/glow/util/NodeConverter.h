#ifndef __TINYEMBER_GLOW_UTIL_NODECONVERTER_H
#define __TINYEMBER_GLOW_UTIL_NODECONVERTER_H

#include "../../gadget/Node.h"

/** Forward declarations */
namespace libember { namespace glow 
{
    class GlowContainer;
    class GlowNodeBase;
    class GlowRootElementCollection;
}
}

namespace glow { namespace util 
{
    /**
     * Provides methods to create a GlowNode from a gadget node.
     */
    class NodeConverter
    {
        public:
            /**
             * Transforms a single node and appends it to the passed parent.
             * @param parent The parent container to attach the created GlowNode to.
             * @param node The gadget node to transform.
             * @param fields Flags containing the node properties to consider in the conversion.
             * @return The created GlowNode instance.
             */
            static libember::glow::GlowNodeBase* create(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

            /**
             * Transforms the passed node into a GlowNode and also creates the complete tree structure
             * required to reach the node from the provided root node.
             * This method assumes that only the node passed is being transferred, so it doesn't check
             * whether any of the parent nodes already exists.
             * @param root The root node to append the parents and the new node to.
             * @param node The gadget node to transform.
             * @param fields Flags containing the node properties to consider in the conversion.
             * @return The newly created node.
             */
            static libember::glow::GlowNodeBase* createStructured(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

            /**
             * Transforms the passed node into a QualifiedNode and appends it to the specified root element collection.
             * @param root The root element collection to append the node to.
             * @param node The gadget node to transform.
             * @param fields Flags containing the node properties to consider in the conversion.
             * @return The newly created node.
             */
            static libember::glow::GlowNodeBase* createQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node, gadget::NodeFieldState const& fields = gadget::NodeField::None);

        private:
            /**
             * This constructor transforms a single gadget node into a GlowNode.
             * @param parent The container to append the new node to.
             * @param node The node to transform.
             * @param fields Flags containing the node properties to consider in the conversion.
             * @param makeQualified If set to true, a QualifiedGlowNode will be created, otherwise the default GlowNode
             *      will be used.
             */
            NodeConverter(libember::glow::GlowContainer* parent, gadget::Node const* node, gadget::NodeFieldState const& fields, bool makeQualified);

        private:
            libember::glow::GlowNodeBase* m_node;
    };
}
}

#endif//__TINYEMBER_GLOW_UTIL_NODECONVERTER_H
