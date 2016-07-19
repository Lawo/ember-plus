/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWLABEL_HPP
#define __LIBEMBER_GLOW_GLOWLABEL_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;

    /**
     * GlowLabel describes a signal label as used in a matrix.
     */
    class LIBEMBER_API GlowLabel : public GlowContainer
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a new instance of GlowLabel with the default tag.
             * @param basePath The path to the node containing label parameters.
             * @param description The label description.
             */
            GlowLabel(ber::ObjectIdentifier const& basePath, std::string description);

            /**
             * Initializes a new instance of GlowLabel
             * @param basePath The path to the node containing label parameters.
             * @param description The label description.
             * @param tag The application tag to set
             */
            GlowLabel(ber::ObjectIdentifier const& basePath, std::string description, ber::Tag const& tag);

            /**
             * Returns the path to the node containing label parameters.
             * @return The path to the node containing label parameters.
             */
            ber::ObjectIdentifier basePath() const;

            /**
             * Returns the label description.
             * @return The label description.
             */
            std::string description() const;

        private:
            /**
             * Initializes a new instance of GlowLabel
             * @param tag The application tag to set
             */
            explicit GlowLabel(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowLabel.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWLABEL_HPP
