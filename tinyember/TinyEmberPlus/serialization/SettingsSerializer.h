#ifndef __TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H
#define __TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H

#include <string>
#include <qdom.h>

namespace serialization
{
    class SettingsSerializer
    {
        public:
            explicit SettingsSerializer(std::string const& filename);

            ~SettingsSerializer();

            void setOption(QString const& option, QString const& value);

            QString getOption(QString const& option);

            int getOption(QString const& option, int fallback);
            
            void save();

        private:
            QDomDocument m_document;
            QString m_filename;
    };
}

#endif//__TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H
