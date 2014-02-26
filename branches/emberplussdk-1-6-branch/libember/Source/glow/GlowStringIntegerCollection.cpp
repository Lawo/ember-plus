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

/*
 * Explicitly undefine the macro and include the implementation file manually afterwards.
 * This is required in order to avoid multiply defined symbols when linking because of the
 * definition being transitively set in headers indirectly included.
 */
#ifdef LIBEMBER_HEADER_ONLY
#  undef LIBEMBER_HEADER_ONLY
#endif
#include "ember/glow/GlowStringIntegerCollection.hpp"
#include "ember/glow/impl/GlowStringIntegerCollection.ipp"
