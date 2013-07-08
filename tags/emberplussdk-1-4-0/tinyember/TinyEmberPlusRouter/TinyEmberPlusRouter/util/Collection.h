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
