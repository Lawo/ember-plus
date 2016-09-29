/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
#define FAIL()    failAssertion(__FILE__, __LINE__)
#else
#define ASSERT(x) ((void)0)
#define FAIL()    ((void)0)
#endif

#endif

#endif
