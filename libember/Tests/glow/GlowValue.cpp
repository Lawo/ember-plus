/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ember/glow/GlowParameter.hpp"

#define THROW_TEST_EXCEPTION(message)                       \
            {                                               \
                std::ostringstream msgStream;               \
                msgStream << message ;                      \
                throw std::runtime_error(msgStream.str());  \
            }
    
int main(int, char const* const*)
{
    try
    {
        {
            libember::glow::GlowParameter p(0);
            p.setValue("HelloWorld");
            libember::glow::Value const value = p.value();
            if (value.type().value() != libember::glow::ParameterType::String)
            {
                THROW_TEST_EXCEPTION("Unexpected type coercion of char const* by GlowParameter::setValue(). Expected glow::ParameterType::String");
            }
        }
        {
            libember::glow::Value const v("HelloWorld");
            if (v.type().value() != libember::glow::ParameterType::String)
            {
                THROW_TEST_EXCEPTION("Unexpected type coercion of char const* by glow::Value constructor. Expected glow::ParameterType::String");
            }
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

