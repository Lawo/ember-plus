/*
    libember -- C++ 03 implementation of the Ember+ Protocol
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

