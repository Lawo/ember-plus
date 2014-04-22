/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_UTIL_COMPLIESWITHSCHEMA_HPP
#define __LIBEMBER_GLOW_UTIL_COMPLIESWITHSCHEMA_HPP

#include <string>

namespace libember { namespace glow { namespace util
{
    /**
     * Tests whether the string containing a collection of schema identifiers contains
     * a specified schema.
     * @param schemaIdentifiers A string containing a collection of schema identifiers.
     * @param schemaIdentifier The schema identifier to find.
     * @return true, if the schema identifier is part of the collection. Otherwise, this
     *      method returns false.
     */
    inline bool complies_with_schema(std::string const& schemaIdentifiers, std::string const& schemaIdentifier)
    {
        return schemaIdentifiers.find(schemaIdentifier) != std::string::npos;
    }
}
}
}

#endif // __LIBEMBER_GLOW_UTIL_COMPLIESWITHSCHEMA_HPP

