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

#ifndef __LIBEMBER_IMPL_VERSION_IPP
#define __LIBEMBER_IMPL_VERSION_IPP

#define _LIBEMBER_MAKE_VERSION(MAJ, MIN, REV)       _LIBEMBER_MAKE_VERSION_IMPL(MAJ, MIN, REV)
#define _LIBEMBER_MAKE_VERSION_IMPL(MAJ, MIN, REV)  ("v" #MAJ "." #MIN "." #REV)            

namespace libember
{
    LIBEMBER_EXTERN
    char const* const versionString = _LIBEMBER_MAKE_VERSION(LIBEMBER_VERSION_MAJOR, LIBEMBER_VERSION_MINOR, LIBEMBER_VERSION_REVISION);
}

#undef _LIBEMBER_MAKE_VERSION
#undef _LIBEMBER_MAKE_VERSION_IMPL

#endif  // __LIBEMBER_IMPL_VERSION_IPP

