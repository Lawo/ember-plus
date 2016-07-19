/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __TINYEMBERROUTER_UTIL_COLLECTION_H
#define __TINYEMBERROUTER_UTIL_COLLECTION_H

namespace util
{
   template<typename InputIterator, typename ValueType>
   InputIterator find(InputIterator first, InputIterator last, ValueType const& value)
   {
      for( ; first != last; first++)
      {
         if(*first == value)
            return first;
      }

      return last;
   }

   template<typename InputIterator, typename ValueType>
   bool contains(InputIterator first, InputIterator last, ValueType const& value)
   {
      return ::util::find<InputIterator, ValueType>(first, last, value) != last;
   }
}

#endif//__TINYEMBERROUTER_UTIL_COLLECTION_H
