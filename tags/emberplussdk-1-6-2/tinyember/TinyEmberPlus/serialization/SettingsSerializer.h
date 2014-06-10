#ifndef __TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H
#define __TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H

#include <string>
#include <qdom.h>

namespace serialization
{
    /**
     * This helper class is used to load or save common settings by using
     * an xml document.
     */
    class SettingsSerializer
    {
        public:
            /**
             * Initializes a new SettingsSerializer.
             * @param filename The name of the file that stores the settings.
             */
            explicit SettingsSerializer(std::string const& filename);

            /** Destructor */
            ~SettingsSerializer();

            /**
             * Adds or updates an option.
             * @param The name of the option.
             * @param The value of the option.
             */
            void setOption(QString const& option, QString const& value);

            /**
             * Gets an option value from the current xml document.
             * @param option The name of the option to read.
             * @return The option value or an empty string, if the option doesn't exist.
             */
            QString getOption(QString const& option);

            /**
             * Gets an option and returns its integer value.
             * @param option The name of the option to read.
             * @param fallback The value to return when the option doesn't exist.
             * @return The option value or the provided fallback, if the option doesn't exist.
             */
            int getOption(QString const& option, int fallback);
            
            /**
             * Writes the current document to a file.
             */
            void save();

        private:
            QDomDocument m_document;
            QString m_filename;
    };
}

#endif//__TINYEMBER_SERIALIZATION_SETTINGSSERIALIZER_H
