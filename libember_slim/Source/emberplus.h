/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_EMBERPLUS_H
#define __LIBEMBER_SLIM_EMBERPLUS_H

/**
  * The version of the library implementation as an uint16.
  * The upper byte is the major version number.
  * The lower byte is the minor version number.
  */
#define EMBER_LIBRARY_VERSION (0x0150) //1.80

#include "api.h"
#include "glowtx.h"
#include "glowrx.h"

/**
  * Initializes internal parameters of the ember library.
  * @param throwError the callback to invoke when a fatal runtime
  *      error occurs.
  * @param failAssertion the callback to invoke when a runtime
  *      assertion fails. Only used if the _DEBUG preprocessor
  *      flag is defined.
  * @param allocMemory the callback to invoke for memory allocation.
  * @param freeMemory the callback to invoke to free memory.
  */
LIBEMBER_API void ember_init(throwError_t throwError,
                failAssertion_t failAssertion,
                allocMemory_t allocMemory,
                freeMemory_t freeMemory);

#endif
