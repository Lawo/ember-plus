#ifndef __TINYEMBER_UTIL_STRINGCONVERTER_H
#define __TINYEMBER_UTIL_STRINGCONVERTER_H

#include "../Types.h"
#include <qstring.h>

namespace util
{
    class StringConverter
    {
        public:
            static QString toUtf8QString(String const& str);

            static String toUtf8StdString(QString const& str);
    };


    inline QString StringConverter::toUtf8QString(String const& str)
    {
        auto const buffer = str.c_str();
        auto const length = str.length();
        return QString::fromUtf8(buffer, length);
    }

    inline String StringConverter::toUtf8StdString(QString const& str)
    {
        auto const bytes = str.toUtf8();
        return String(bytes.begin(), bytes.end());
    }

}

#endif//__TINYEMBER_UTIL_STRINGCONVERTER_H
