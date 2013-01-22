#ifndef ENTITYWRITER_H
#define ENTITYWRITER_H

#include <algorithm>
#include "../Entity.h"
#include "../EntityTypeVisitor.h"
#include "../Node.h"
#include "../Parameter.h"
#include "../states/EnumState.h"
#include "../states/NumericState.h"
#include "../states/StringState.h"
#include "EntitySerialization.h"

namespace gadgets { namespace serialization
{
    /**
     * Writes an entity and all its children to a stream
     */
    template<typename StreamType>
    class EntityWriter : EntityTypeVisitor
    {
    public:
        typedef StreamType stream_type;

    public:
        /**
         * Constructor
         * @param stream Stream to serialize the data to
         * @param node Node to serialize
         */
        EntityWriter(stream_type& stream, Entity* node);

    private:
        /**
         * Serializes a single entity
         * @param entity Entity whos properties shall be serialized
         */
        void save(Entity* entity);

        /**
         * Serializes the common properties of an Entity
         * @param entity Entity type
         */
        template<EntityType::_Domain Type, EntityVersion::_Domain Version>
        void saveHeader(Entity const& entity);

        /**
         * Writes the node's properties and its child nodes
         */
        virtual void visit(Node& node);

        /**
         * Writes the properties of an integer parameter
         * @see EntityTypeVisitor
         */
        virtual void visit(Parameter& parameter, states::NumericState<int>& state);

        /**
         * Writes the properties of a double parameter
         * @see EntityTypeVisitor
         */
        virtual void visit(Parameter& parameter, states::NumericState<double>& state);

        /**
         * Writes the properties of an enumeration parameter
         * @see EntityTypeVisitor
         */
        virtual void visit(Parameter& parameter, states::EnumState& state);

        /**
         * Writes the properties of a string parameter
         * @see EntityTypeVisitor
         */
        virtual void visit(Parameter& parameter, states::StringState& state);

    private:
        stream_type& m_stream;
    };

    /**************************************************************************
     * Mandatory Inline implementation                                        *
     **************************************************************************/

    template<typename StreamType>
    inline EntityWriter<StreamType>::EntityWriter(stream_type& stream, Entity* node)
        : m_stream(stream)
    {
        save(node);
    }

    template<typename StreamType>
    inline void EntityWriter<StreamType>::save(Entity* entity)
    {
        entity->visit(*this);
    }

    template<typename StreamType>
    template<EntityType::_Domain Type, EntityVersion::_Domain Version>
    inline void EntityWriter<StreamType>::saveHeader(Entity const& entity)
    {
        m_stream << Type << Version << entity.identifier() << entity.description();
    }

    template<typename StreamType> //virtual
    inline void EntityWriter<StreamType>::visit(Node& node)
    {
        saveHeader<EntityType::Node, EntityVersion::V1>(node);
        Entity::iterator first = node.begin();
        Entity::iterator const last = node.end();

        m_stream << node.size();

        for( /** Nothing **/ ; first != last; ++first)
            save(*first);
    }

    template<typename StreamType> //virtual
    inline void EntityWriter<StreamType>::visit(Parameter& parameter, states::NumericState<int>& state)
    {
        saveHeader<EntityType::IntParam, EntityVersion::V1>(parameter);

        m_stream << state.isCommand() << state.isWriteable() << state.max() << state.min() << state.value() << state.format();
    }

    template<typename StreamType> //virtual
    inline void EntityWriter<StreamType>::visit(Parameter& parameter, states::NumericState<double>& state)
    {
        saveHeader<EntityType::RealParam, EntityVersion::V1>(parameter);

        m_stream << state.isCommand() << state.isWriteable() << state.max() << state.min() << state.value() << state.format();
    }

    template<typename StreamType> //virtual
    inline void EntityWriter<StreamType>::visit(Parameter& parameter, states::EnumState& state)
    {
        saveHeader<EntityType::EnumParam, EntityVersion::V1>(parameter);
        states::EnumState::const_iterator first = state.begin();
        states::EnumState::const_iterator const last = state.end();

        m_stream << state.isCommand() << state.isWriteable() << state.index() << state.size();


        for(/** Nothing **/; first != last; ++first)
            m_stream << *first;
    }

    template<typename StreamType> //virtual
    inline void EntityWriter<StreamType>::visit(Parameter& parameter, states::StringState& state)
    {
        saveHeader<EntityType::StringParam, EntityVersion::V1>(parameter);

        m_stream << state.isCommand() << state.isWriteable() << state.maxLength() << state.value();
    }
}
}

#endif // ENTITYWRITER_H
