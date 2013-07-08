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

#ifndef __LIBEMBER_SLIM_EMBERINTERNAL_H
#define __LIBEMBER_SLIM_EMBERINTERNAL_H

#include "bertypes.h"

// ====================================================================
//
// Definitions internal to the ember library.
//
// ====================================================================

void throwError(int error, pcstr pMessage);
void failAssertion(pcstr pFileName, int lineNumber);
void *allocMemory(size_t size);
void freeMemory(void *pMemory);

#define newobj(type) ((type *)allocMemory(sizeof(type)))
#define newarr(type, count) ((type *)allocMemory(sizeof(type) * (count)))

#ifndef ASSERT

#ifdef _DEBUG
#define ASSERT(x) if((x) == false) failAssertion(__FILE__, __LINE__)
#else
#define ASSERT(x) ((void)0)
#endif

#endif

#endif
