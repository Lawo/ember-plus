#ifndef __LIBFORMULA_UTIL_UTIL_HPP
#define __LIBFORMULA_UTIL_UTIL_HPP

#include <sstream>
#include "../FunctionType.hpp"
#include "../Symbol.hpp"
#include "../Types.hpp"

namespace libformula { namespace util
{
    /**
     * Returns the string representation of the function type provided.
     * @param type The function type to get the string representation of.
     * @return The string representation of the function type.
     */
    inline std::string functiontype_tostring(FunctionType const& type)
    {
        auto const value = type.value();
        switch(value)
        {
            case FunctionType::Exp:
                return "exp";
            case FunctionType::Cos:
                return "cos";
            case FunctionType::Sin:
                return "sin";
            case FunctionType::Tan:
                return "tan";
            case FunctionType::Acos:
                return "acos";
            case FunctionType::Asin:
                return "asin";
            case FunctionType::Atan:
                return "atan";
            case FunctionType::Cosh:
                return "cosh";
            case FunctionType::Sinh:
                return "sinh";
            case FunctionType::Tanh:
                return "tanh";
            case FunctionType::Int:
                return "int";
            case FunctionType::Float:
                return "float";
            case FunctionType::Log:
                return "log";
            case FunctionType::Ln:
                return "ln";
            case FunctionType::Round:
                return "round";
            case FunctionType::Ceil:
                return "ceil";
            case FunctionType::Sqrt:
                return "sqrt";
            case FunctionType::Pow:
                return "pow";
            default:
                return "invalid";
        }
    }

    /**
     * Returns the string representation of the provided symbol.
     * @param The symbol to get the string representation from.
     * @return The string representation of the provided symbol.
     */
    template<typename InputIterator>
    inline std::string symbol_tostring(Symbol<InputIterator> const& symbol)
    {
        auto const& type = symbol.type();
        auto const value = type.value();
        if (FunctionType::isFunction(type))
        {
            auto const function = FunctionType::fromSymbol(type);
            auto const text = functiontype_tostring(function);
            return text;
        }
        else
        {
            switch(value)
            {
                case SymbolType::LParen:
                    return "(";
                case SymbolType::RParen:
                    return ")";
                case SymbolType::Assign:
                    return "=";
                case SymbolType::Plus:
                    return "+";
                case SymbolType::Minus:
                    return "-";
                case SymbolType::Multiply:
                    return "*";
                case SymbolType::Divide:
                    return "/";
                case SymbolType::LongDivide:
                    return "\\";
                case SymbolType::Modulo:
                    return "%";
                case SymbolType::Pow:
                    return "^";
                case SymbolType::Comma:
                    return ",";
                case SymbolType::It:
                    return "$";
                case SymbolType::Pi:
                    return "Pi";
                case SymbolType::E:
                    return "e";
                case SymbolType::IntegerValue:
                {
                    std::stringstream stream;
                    stream << "long " << symbol.toValueType<long_type>();
                    return stream.str();
                }
                case SymbolType::RealValue:
                {
                    std::stringstream stream;
                    stream << "real " << symbol.toValueType<real_type>();
                    return stream.str();
                }
                default:
                    return "unknown";
            }
        }
    }
}
}

#endif  // __LIBFORMULA_UTIL_UTIL_HPP
