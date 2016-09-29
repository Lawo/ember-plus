/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/*
 * Explicitly undefine the macro and include the implementation file manually afterwards.
 * This is required in order to avoid multiply defined symbols when linking because of the
 * definition being transitively set in headers indirectly included.
 */
#ifdef LIBEMBER_HEADER_ONLY
#  undef LIBEMBER_HEADER_ONLY
#endif
#include "ember/glow/GlowLabel.hpp"
#include "ember/glow/impl/GlowLabel.ipp"
