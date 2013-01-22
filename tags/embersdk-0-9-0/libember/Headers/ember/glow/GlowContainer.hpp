#ifndef __LIBEMBER_GLOW_CONTAINER_HPP
#define __LIBEMBER_GLOW_CONTAINER_HPP

#include "../dom/Sequence.hpp"
#include "detail/CompareNodeTag.hpp"
#include "traits/ValueTypeToBerType.hpp"
#include "GlowTags.hpp"
#include "GlowType.hpp"

namespace libember { namespace glow
{
    /**
     * Base class for all glow types.
     */
    class LIBEMBER_API GlowContainer : public dom::Sequence
    {
        protected:
            /**
             * Initializes a new container with a glow type and application tag.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @param tag The application tag.
             */
            GlowContainer(GlowType const& type, ber::Tag const& tag);

            /**
             * Initializes a new container only with a type.
             * @param type The glow type to set. This type will be converted into a universal tag.
             * @note The application tag will be set to ContextSpecific-0.
             */
            explicit GlowContainer(GlowType const& type);

            /**
             * Returns the universal tag of this container.
             * @return The universal tag of this container.
             */
            virtual ber::Tag typeTagImpl() const;

            /**
             * Searches for a node with the specified application tag and tries 
             * to cast it to the specified type.
             * @param first Start element of the container to traverse.
             * @param last End element of the container to traverse
             * @param tag Tag to look for
             * @return If the node exists, this method returns a pointer to the node. Otherwise, it 
             *      returns null.
             */
            template<typename GlowType>
            GlowType* find(iterator first, iterator last, ber::Tag const& tag);

            /**
             * Searches for a node with the specified application tag and tries 
             * to cast it to the specified type.
             * @param first Start element of the container to traverse.
             * @param last End element of the container to traverse
             * @param tag Tag to look for
             * @return If the node exists, this method returns a pointer to the node. Otherwise, it 
             *      returns null.
             */
            template<typename GlowType>
            GlowType const* find(const_iterator first, const_iterator last, ber::Tag const& tag) const;

        private:
            /** Prohibit assignment */
            GlowContainer& operator=(GlowContainer const&);

        private:
            ber::Tag const m_universalTag;
    };

    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename GlowType>
    inline GlowType* GlowContainer::find(iterator first, iterator last, ber::Tag const& tag)
    {
        iterator const it = std::find_if(first, last, detail::CompareNodeTag(tag));
        return it != last ? &dynamic_cast<GlowType&>(*it) : 0;
    }

    template<typename GlowType>
    inline GlowType const* GlowContainer::find(const_iterator first, const_iterator last, ber::Tag const& tag) const
    {
        const_iterator const it = std::find_if(first, last, detail::CompareNodeTag(tag));
        return it != last ? &dynamic_cast<GlowType const&>(*it) : 0;
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowContainer.ipp"
#endif

#endif  // __LIBEMBER_GLOW_CONTAINER_HPP


