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

#ifndef __LIBEMBER_SLIM_BERTYPES_H
#define __LIBEMBER_SLIM_BERTYPES_H

// ======================================================
//
// General typedefs
//
// ======================================================

#ifdef INCLUDE_BERCUSTOM_H
// customize typedefs using this header
#include "bercustom.h"
#endif

#ifndef BYTE_DEFINED
/**
  * 8bit unsigned
  */
typedef unsigned char byte;
#endif

#ifndef DWORD_DEFINED
/**
  * 32bit unsigned
  */
typedef unsigned int dword;
#endif

#ifndef QWORD_DEFINED
/**
  * 64bit unsigned
  */
typedef unsigned long long qword;
#endif

#ifndef BERLONG_DEFINED
/**
  * 64bit signed
  */
typedef long long berlong;
#endif

#ifndef BERINT_DEFINED
/**
  * 32bit signed
  */
typedef int berint;
#endif

#ifndef PCSTR_DEFINED
/**
  * pointer to constant char
  */
typedef const char *pcstr;
#endif

#ifndef PSTR_DEFINED
/**
  * pointer to char
  */
typedef char *pstr;
#endif

#ifndef VOIDPTR_DEFINED
/**
  * either pointer type or integer type with pointer size
  */
typedef void *voidptr;
#endif

#if !defined(__cplusplus) && !defined(BOOL_DEFINED)
/**
  * true/false
  */
typedef int bool;

#ifndef false
#define false (0)
#endif

#ifndef true
#define true (!false)
#endif

#endif

#ifndef DOUBLE_LENGTH
/**
  * The length of the type double in bytes
  */
#define DOUBLE_LENGTH (8)
#endif

#ifndef TAGNUMBER_DEFINED
/**
  * Integer type to hold the number of a BerTag.
  * Usually the same size as berint.
  */
typedef unsigned int tagnumber;
#endif

#ifndef BERTYPE_DEFINED
/**
  * Integer type large enough to hold the local representation
  * of a ber type, including the BerType_ApplicationFlag flag.
  * Must be at least 16bit unsigned.
  * This is not an encoded/decoded type. Types in TLTLVs are
  * encoded as tags with universal or application class.
  */
typedef unsigned int bertype;
#endif


// ======================================================
//
// Utilities
//
// ======================================================

#ifndef bzero
#define bzero(item) (memset(&(item), 0, sizeof(item)))
#endif

#ifndef NULL
#define NULL (0)
#endif

#endif
