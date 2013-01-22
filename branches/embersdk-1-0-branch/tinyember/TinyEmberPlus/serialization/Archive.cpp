#include <algorithm>
#include <qfile.h>
#include <qdatastream.h>
#include "Archive.h"
#include "detail/GadgetTreeReader.h"
#include "detail/GadgetTreeWriter.h"

namespace serialization
{
    void Archive::serialize(gadget::Node const* root, String const& filename) const
    {
        auto const writer = detail::GadgetTreeWriter(root);
        auto& berstream = writer.m_stream;

        QFile file(QString::fromStdString(filename));
        if (file.open(QIODevice::WriteOnly))
        {
            std::for_each(std::begin(berstream), std::end(berstream), [&file](unsigned char byte)
            {
                file.putChar(byte);
            });

            file.flush();
            file.close();
        }
    }

    gadget::Node* Archive::deserialize(String const& filename) const
    {
        auto result = static_cast<gadget::Node*>(nullptr);
        if (QFile::exists(QString::fromStdString(filename)))
        {
            QFile file(QString::fromStdString(filename));
            if (file.open(QIODevice::ReadOnly))
            {
                auto bytearray = file.readAll();
                auto stream = libember::util::OctetStream(0);
                stream.append(bytearray.begin(), bytearray.end());

                auto reader = detail::GadgetTreeReader(stream);
                result = reader.m_root;
                file.close();
            }
        }

        return result;
    }
}
