#ifndef ENTITYSERIALIZATION_H
#define ENTITYSERIALIZATION_H

namespace gadgets { namespace serialization
{
    /**
     * Contains the entity type, used only for serialization
     */
    struct EntityType
    {
        enum _Domain
        {
            Unknown,
            Node,
            IntParam,
            EnumParam,
            RealParam,
            StringParam,

            _Size = 0xFFFFFFFF
        };

        typedef std::size_t value_type;

        /**
         * Non-explicit constructor.
         * @param value Entity type.
         */
        EntityType(_Domain const value)
            : value(value)
        {}

        /**
         * Contains the entity type.
         */
        value_type const value;
    };


    /**
     * Contains the entity version, used only for serialization
     */
    struct EntityVersion
    {
        enum _Domain
        {
            V1 = 1
        };

        typedef std::size_t value_type;

        /**
         * Non-explicit constructor
         * @param value Entity version.
         */
        EntityVersion(_Domain const value)
            : value(value)
        {}

        /**
         * Entity value
         */
        value_type const value;
    };
}
}

#endif // ENTITYSERIALIZATION_H
