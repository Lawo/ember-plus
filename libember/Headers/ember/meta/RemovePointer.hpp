/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_REMOVEPOINTER_HPP
#define __LIBEMBER_META_REMOVEPOINTER_HPP

namespace libember { namespace meta
{
    /**
     * An unary meta-function that removes a possible pointer declaration from
     * the passed type.
     */
    template<typename T>
    struct RemovePointer
    {
        typedef T type;
    };


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/


    template<typename T>
    struct RemovePointer<T*>
    {
        typedef T type;
    };
}
}

#endif  // __LIBEMBER_META_REMOVEPOINTER_HPP

