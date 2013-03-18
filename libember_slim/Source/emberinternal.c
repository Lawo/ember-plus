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
