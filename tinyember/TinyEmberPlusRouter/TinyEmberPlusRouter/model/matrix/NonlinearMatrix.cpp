/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "NonlinearMatrix.h"

namespace model { namespace matrix
{
   // overrides

   int NonlinearMatrix::targetCount() const
   {
      return targets().size();
   }

   int NonlinearMatrix::sourceCount() const
   {
      return sources().size();
   }

   Signal* NonlinearMatrix::getTarget(int number) const
   {
      for(auto signal : targets())
      {
         if(signal->number() == number)
            return signal;
      }

      return nullptr;
   }

   Signal* NonlinearMatrix::getSource(int number) const
   {
      for(auto signal : sources())
      {
         if(signal->number() == number)
            return signal;
      }

      return nullptr;
   }
}}
