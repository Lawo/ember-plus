/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "ember.h"
#include "emberinternal.h"

// ====================================================================
//
// Internals
//
// ====================================================================

throwError_t _pThrowError = NULL;
failAssertion_t _pFailAssertion = NULL;
allocMemory_t _pAllocMemory = NULL;
freeMemory_t _pFreeMemory = NULL;

void throwError(int error, pcstr pMessage)
{
   if(_pThrowError != NULL)
      _pThrowError(error, pMessage);
}

void failAssertion(pcstr pFileName, int lineNumber)
{
   if(_pFailAssertion != NULL)
      _pFailAssertion(pFileName, lineNumber);
}

void *allocMemory(size_t size)
{
   void *pMemory;

   if(_pAllocMemory != NULL)
   {
      pMemory = _pAllocMemory(size);

      if(pMemory == NULL)
         throwError(-1, "Out of memory!");

      return pMemory;
   }
   else
   {
      throwError(-1, "No memory allocation function set!");
      return NULL;
   }
}

void freeMemory(void *pMemory)
{
   if(_pFreeMemory != NULL)
      _pFreeMemory(pMemory);
   else
      throwError(-1, "No memory de-allocator function set!");
}
