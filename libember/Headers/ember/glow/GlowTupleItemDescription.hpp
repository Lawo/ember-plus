/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP
#define __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP

#include "GlowContainer.hpp"
#include "ParameterType.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowTupleItemDescription contains a ber type information and a name.
     */
    class LIBEMBER_API GlowTupleItemDescription : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes an enumeration entry with the application tag specified.
             * @param tag Application tag.
             */
            explicit GlowTupleItemDescription(ber::Tag const& tag);

            /**
             * Initializes a new GlowTupleItemDescripiton with a known type and name.
             * @param type The type to initialize this instance with.
             * @param name The name to initialize this instance with.
             */
            GlowTupleItemDescription(ParameterType const& type, std::string const& name);

            /**
             * Returns the type stored in this instance. If the property is not available,
             * ber::Type::None will be returned.
             * @return The type stored in this instance.
             */
            ParameterType type() const;

            /**
             * Returns the name of this item description. If no name is set, an empty
             * string will be returned.
             * @return The name of this item description.
             */
            std::string name() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowTupleItemDescription.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP
