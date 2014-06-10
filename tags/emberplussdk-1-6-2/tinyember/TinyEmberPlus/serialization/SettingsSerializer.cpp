#include "SettingsSerializer.h"
#include <qfile.h>
#include <qtextstream.h>

namespace serialization
{
    SettingsSerializer::SettingsSerializer(std::string const& filename)
        : m_document("Settings")
        , m_filename(QString::fromStdString(filename))
    {
        QFile file(m_filename);
        if (file.open(QIODevice::ReadOnly))
        {
            m_document.setContent(&file);
        }
        file.close();

        auto root = m_document.documentElement();
        if (root.isNull())
        {
            root = m_document.createElement("Settings");
            m_document.appendChild(root);
        }
    }

    SettingsSerializer::~SettingsSerializer()
    {
        save();
    }
            
    void SettingsSerializer::save()
    {
        QFile file(m_filename);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            m_document.save(stream, QDomNode::EncodingFromDocument);
            
            file.close();
        }
    }

    void SettingsSerializer::setOption(QString const& option, QString const& value)
    {
        auto root = m_document.documentElement();
        if (root.isNull() == false)
        {
            auto node = root.firstChild();
            while(node.isNull() == false)
            {
                auto element = node.toElement();
                if (element.isNull() == false)
                {
                    if (element.tagName() == option)
                    {
                        element.setAttribute("value", value);
                        return;
                    }
                }

                node = node.nextSibling();
            }

            auto element = m_document.createElement(option);
            element.setAttribute("value", value);
            root.appendChild(element);
        }
    }

    QString SettingsSerializer::getOption(QString const& option)
    {
        auto root = m_document.documentElement();
        if (root.isNull() == false)
        {
            auto name = root.tagName();
            auto node = root.firstChild();
            while (node.isNull() == false)
            {
                auto element = node.toElement();
                if (element.isNull() == false)
                {
                    if (element.tagName() == option)
                    {
                        auto value = element.attribute("value");
                        return value;
                    }
                }

                node = node.nextSibling();
            }
        }
        return "";
    }

    int SettingsSerializer::getOption(QString const& option, int fallback)
    {
        auto value = getOption(option);
        if (value.isEmpty())
        {
            return fallback;
        }
        else
        {
            return value.toInt();
        }
    }
}
