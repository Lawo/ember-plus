#ifndef ENTITYARCHIVE_H
#define ENTITYARCHIVE_H

#include "../EntityTypeVisitor.h"
#include "EntityReader.h"
#include "EntityWriter.h"

namespace gadgets { namespace serialization
{
    /**
     * Provides the possibility to write an entity tree to a
     * stream or load it from a stream.
     */
    template<typename StreamType>
    class EntityArchive
    {
    public:
        typedef StreamType stream_type;
        typedef EntityReader<stream_type> reader_type;
        typedef EntityWriter<stream_type> writer_type;

    public:
        /**
         * Constructor
         */
        EntityArchive();

        /**
         * Stores the root node and all its children to the passed stream.
         * @note The stream must support the << operator
         * @param stream Target stream
         * @param root Node to serialize
         */
        void save(stream_type& stream, Entity* root) const;

        /**
         * Loads a node from a stream
         * @note The stream must support the >> operator
         * @return Returns the created node
         */
        Node* load(stream_type& stream) const;

    private:
        /**
         * Prohibit copy construction and assignment.
         */
        EntityArchive(EntityArchive const&);
        EntityArchive &operator=(EntityArchive const&);
    };


    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/


    template<typename StreamType>
    inline EntityArchive<StreamType>::EntityArchive()
    {
    }

    template<typename StreamType>
    inline void EntityArchive<StreamType>::save(stream_type& stream, Entity* root) const
    {
        writer_type(stream, root);
    }

    template<typename StreamType>
    inline Node* EntityArchive<StreamType>::load(stream_type& stream) const
    {
        reader_type const reader(stream);
        return reader.root();
    }
}
}

#endif // ENTITYARCHIVE_H
