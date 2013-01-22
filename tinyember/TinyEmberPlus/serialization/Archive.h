#ifndef __TINYEMBER_SERIALIZATION_ARCHIVE_H
#define __TINYEMBER_SERIALIZATION_ARCHIVE_H

#include "../Types.h"

namespace gadget
{
    class Node;
}

namespace serialization
{
    class Archive
    {
        public:
            void serialize(gadget::Node const* root, String const& filename) const;

            gadget::Node* deserialize(String const& filename) const;
    };
}

#endif//__TINYEMBER_SERIALIZATION_ARCHIVE_H
