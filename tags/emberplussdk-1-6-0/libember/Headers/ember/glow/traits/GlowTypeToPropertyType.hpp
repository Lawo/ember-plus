/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_TRAITS_GLOWTYPETOPROPERTYTYPE_HPP
#define __LIBEMBER_GLOW_TRAITS_GLOWTYPETOPROPERTYTYPE_HPP

#include "../GlowNodeBase.hpp"
#include "../GlowNode.hpp"
#include "../GlowQualifiedNode.hpp"
#include "../NodeProperty.hpp"

#include "../GlowParameterBase.hpp"
#include "../GlowParameter.hpp"
#include "../GlowQualifiedParameter.hpp"
#include "../ParameterProperty.hpp"

#include "../GlowFunctionBase.hpp"
#include "../GlowFunction.hpp"
#include "../GlowQualifiedFunction.hpp"
#include "../FunctionProperty.hpp"

#include "../GlowMatrixBase.hpp"
#include "../GlowMatrix.hpp"
#include "../GlowQualifiedMatrix.hpp"
#include "../MatrixProperty.hpp"

namespace libember { namespace glow { namespace traits 
{
    /** 
     * Structure used to determine the corresponding property type of a glow type.
     */
    template<typename GlowType>
    struct GlowTypeToPropertyType;

    #define GLOWTYPETOPROPERTYTYPE(TYPE)                                \
    template <>                                                         \
    struct GlowTypeToPropertyType<glow::Glow##TYPE##Base>               \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct GlowTypeToPropertyType<glow::Glow##TYPE>                     \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct GlowTypeToPropertyType<glow::GlowQualified##TYPE>            \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct GlowTypeToPropertyType<const glow::Glow##TYPE##Base>         \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct GlowTypeToPropertyType<const glow::Glow##TYPE>               \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct GlowTypeToPropertyType<const glow::GlowQualified##TYPE>      \
    {                                                                   \
        typedef glow::TYPE##Property Property;                          \
    };                                                                  \


    GLOWTYPETOPROPERTYTYPE(Node)
    GLOWTYPETOPROPERTYTYPE(Parameter)
    GLOWTYPETOPROPERTYTYPE(Function)
    GLOWTYPETOPROPERTYTYPE(Matrix)
}
}
}

#endif  // __LIBEMBER_GLOW_TRAITS_GLOWTYPETOPROPERTYTYPE_HPP
