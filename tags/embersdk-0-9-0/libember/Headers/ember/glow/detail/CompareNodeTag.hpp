#ifndef __LIBEMBER_GLOW_DETAIL_COMPARENODETAG_HPP
#define __LIBEMBER_GLOW_DETAIL_COMPARENODETAG_HPP

#include "../../dom/Node.hpp"

namespace libember { namespace glow { namespace detail
{
    /** 
     * Struture that provides the () operator to compare a node's application tag with
     * another tag.
     */
    struct CompareNodeTag
    {
        /** 
         * Constructor, initializes the structure with the tag to compare 
         * a node's application tag with.
         * @param tag Application tag.
         */
        explicit CompareNodeTag(ber::Tag const& tag)
            : tag(tag)
        {}

        /** 
         * Compares the application tag of the node with the tag stored in this instance.
         * @param node Node to get the application tag from.
         * @return Returns true if the application tag equals the one that has been passed
         *      to the constructor, false otherwise.
         */
        bool operator()(dom::Node const& node) const
        {
            return tag == node.applicationTag();
        }
        
        ber::Tag tag;
    };
}
}
}

#endif  // __LIBEMBER_GLOW_DETAIL_COMPARENODETAG_HPP
