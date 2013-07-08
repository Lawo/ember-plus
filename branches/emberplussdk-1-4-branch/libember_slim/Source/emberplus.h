/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol
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

#ifndef __LIBEMBER_SLIM_EMBERPLUS_H
#define __LIBEMBER_SLIM_EMBERPLUS_H

/**
  * The version of the library implementation as an uint16.
  * The upper byte is the major version number.
  * The lower byte is the minor version number.
  */
#define EMBER_LIBRARY_VERSION (0x0129) //1.41

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
void ember_init(throwError_t throwError,
                failAssertion_t failAssertion,
                allocMemory_t allocMemory,
                freeMemory_t freeMemory);

#endif
