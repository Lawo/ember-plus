/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __TINYEMBERROUTER_MODEL_MATRIX_NTONLINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_NTONLINEARMATRIX_H

#include "LinearMatrix.h"

namespace model { namespace matrix
{
   /**
     * Matrix with N:N connection semantics and linear addressing mode.
     */
   class NToNLinearMatrix : public LinearMatrix
   {
   public:
      NToNLinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount);

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);

   protected:
      /**
        * Overridden to execute connects according to N:N connection semantics.
        */
      virtual bool connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);
   };
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_NTONLINEARMATRIX_H
