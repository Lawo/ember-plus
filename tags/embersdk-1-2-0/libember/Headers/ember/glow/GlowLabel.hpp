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
