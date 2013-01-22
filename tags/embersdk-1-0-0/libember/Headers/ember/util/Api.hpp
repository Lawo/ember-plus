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

#ifndef __LIBEMBER_UTIL_API_HPP
#define __LIBEMBER_UTIL_API_HPP

/*
 * Preprocessor definitions for generic symbol import/export support
 * Adapted from http://gcc.gnu.org/wiki/Visibility
 */

#if defined _WIN32 || defined __CYGWIN__
#  define LIBEMBER_HELPER_DLL_IMPORT __declspec(dllimport)
#  define LIBEMBER_HELPER_DLL_EXPORT __declspec(dllexport)
#  define LIBEMBER_HELPER_DLL_LOCAL
#else
#  define LIBEMBER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#  define LIBEMBER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#  define LIBEMBER_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#if defined LIBEMBER_DLL                // Define this if libember is compiled or used as a DLL/shared object
#  ifdef LIBEMBER_HEADER_ONLY
#    error "LIBEMBER_DLL may not be used in conjunction with LIBEMBER_HEADER_ONLY"
#  endif
#  ifdef LIBEMBER_DLL_EXPORTS           // Defined libember is being built (instead of using it)
#    define LIBEMBER_API LIBEMBER_HELPER_DLL_EXPORT
#  else
#    define LIBEMBER_API LIBEMBER_HELPER_DLL_IMPORT
#  endif
#  define LIBEMBER_LOCAL LIBEMBER_HELPER_DLL_LOCAL
#else                                   // If LIBEMBER_DLL is not defined this means that libember is used as a static or header only library.
#  define LIBEMBER_API
#  define LIBEMBER_LOCAL
#endif

#endif  // __LIBEMBER_UTIL_API_HPP

