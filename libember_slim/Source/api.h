/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_API_H
#define __LIBEMBER_API_H

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

#endif  // __LIBEMBER_API_H

