#ifndef __TINYEMBER_UTIL_STRINGCONVERTER_H
#define __TINYEMBER_UTIL_STRINGCONVERTER_H

#include "../Types.h"
#include <qstring.h>

namespace util
{
    /**
     * The string converter is able to convert an utf8 std::string into a QString
     * and vice versa.
     */
    class StringConverter
    {
        public:
            /**
             * Converts a std::string into a QString.
             * @param str The std string to convert.
             * @return The QString instance.
             */
            static QString toUtf8QString(String const& str);

            /**
             * Converts a QString into an utf8 encoded std::string.
             * @param str The qstring to convert.
             * @return The utf8 encoded std::string.
             */
            static String toUtf8StdString(QString const& str);
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

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
