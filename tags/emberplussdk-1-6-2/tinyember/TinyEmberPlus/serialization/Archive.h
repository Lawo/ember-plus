#ifndef __TINYEMBER_SERIALIZATION_ARCHIVE_H
#define __TINYEMBER_SERIALIZATION_ARCHIVE_H

#include "../Types.h"

/** Forward declaration */
namespace gadget
{
    class Node;
}

namespace serialization
{
    /**
     * The Archive provides method to store a gadget tree to a file or load it. The trees
     * are converted into their glow representation and are then ber encoded and written
     * to disk.
     */
    class Archive
    {
        public:
            /**
             * Writes the gadget tree to a file.
             * @param root The root node to serialize.
             * @param filename The name of the file where the encoded data will be written.
             */
            void serialize(gadget::Node const* root, String const& filename) const;

            /**
             * Loads a gadget tree from a file.
             * @param filename The name of the file containing the data.
             * @return The deserialized gadget tree.
             */
            gadget::Node* deserialize(String const& filename) const;
    };
}

#endif//__TINYEMBER_SERIALIZATION_ARCHIVE_H
