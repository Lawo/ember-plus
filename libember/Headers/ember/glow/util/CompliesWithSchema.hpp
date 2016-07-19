/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

