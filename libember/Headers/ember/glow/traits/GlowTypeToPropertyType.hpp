/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
