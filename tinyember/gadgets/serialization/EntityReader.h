#ifndef ENTITYREADER_H
#define ENTITYREADER_H

#include <algorithm>
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
     * The EntityReader can be used to load an entity tree from a stream
     */
    template<typename StreamType>
    class EntityReader : EntityTypeVisitor
    {
    public:
        typedef StreamType stream_type;

    public:
        /**
         * Constructor, immediately loads all entities from the passed stream_type
         * @param stream Stream to load the entities from
         */
        EntityReader(stream_type& stream);

        /**
         * Returns the loaded root node.
         */
        Node* root() const;

    private:
        /**
         * Reads the next entity header
         * @param current Current node
         */
        Entity* read(Node* current);

        /**
         * Creates a new entity depending on the provided arguments and loads its properties
         * @param current The current node
         */
        Entity* read(Node* current, EntityType const& type, EntityVersion const& version, String const& identifier, String const& description);

    private:
        /**
         * Loads the node's propertis from a stream and starts loading child nodes and parameters
         */
        virtual void visit(Node& node);

        /**
         * Loads an integer parameter
         */
        virtual void visit(Parameter& parameter, states::NumericState<int>& state);

        /**
         * Loads a double parameter
         */
        virtual void visit(Parameter& parameter, states::NumericState<double>& state);

        /**
         * Loads an enumeration parameter
         */
        virtual void visit(Parameter& parameter, states::EnumState& state);

        /**
         * Loads a string parameter
         */
        virtual void visit(Parameter& parameter, states::StringState& state);

    private:
        stream_type& m_stream;
        Node* m_root;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename StreamType>
    inline EntityReader<StreamType>::EntityReader(stream_type &stream)
        : m_stream(stream)
        , m_root(0)
    {
        read(0);
    }

    template<typename StreamType>
    inline Node* EntityReader<StreamType>::root() const
    {
        return m_root;
    }

    template<typename StreamType>
    inline Entity* EntityReader<StreamType>::read(Node *current)
    {
        EntityType::_Domain type;
        EntityVersion::_Domain version;
        String identifier;
        String description;

        m_stream >> ((EntityType::value_type&) type) >> ((EntityVersion::value_type&) version) >> identifier >> description;

        return read(current, type, version, identifier, description);
    }

    template<typename StreamType>
    inline Entity* EntityReader<StreamType>::read(Node* current, EntityType const& type, EntityVersion const&, String const& identifier, String const& description)
    {
        Entity* next = 0;

        switch(type.value)
        {
        case EntityType::EnumParam:
            next = current->insert(StateType::Enumeration, identifier, description);
            next->visit(*this);
            break;

        case EntityType::IntParam:
            next = current->insert(StateType::Integer, identifier, description);
            next->visit(*this);
            break;

        case EntityType::RealParam:
            next = current->insert(StateType::Real, identifier, description);
            next->visit(*this);
            break;

        case EntityType::StringParam:
            next = current->insert(StateType::String, identifier, description);
            next->visit(*this);
            break;

        case EntityType::Node:
            if (current == 0)
            {
                m_root = Node::create(identifier, description);
                m_root->visit(*this);
            }
            else
            {
                next = current->insert(identifier, description);
                next->visit(*this);
            }

            break;
        }

        return next;
    }

    template<typename StreamType>
    inline void EntityReader<StreamType>::visit(Node& node)
    {
        Node::size_type size;

        m_stream >> size;

        while(size-- > 0)
            read(&node);
    }

    template<typename StreamType>
    inline void EntityReader<StreamType>::visit(Parameter&, states::NumericState<int>& state)
    {
        bool command;
        bool writeable;
        int max;
        int min;
        int value;
        String format;

        m_stream >> command >> writeable >> max >> min >> value >> format;

        state.setCommand(command);
        state.setWriteable(writeable);
        state.setMax(max);
        state.setMin(min);
        state.setValue(value);
        state.setFormat(format);
    }

    template<typename StreamType>
    inline void EntityReader<StreamType>::visit(Parameter&, states::NumericState<double>& state)
    {
        bool command;
        bool writeable;
        double max;
        double min;
        double value;
        String format;

        m_stream >> command >> writeable >> max >> min >> value >> format;

        state.setCommand(command);
        state.setWriteable(writeable);
        state.setMax(max);
        state.setMin(min);
        state.setValue(value);
        state.setFormat(format);
    }

    template<typename StreamType>
    inline void EntityReader<StreamType>::visit(Parameter&, states::EnumState& state)
    {
        bool command;
        bool writeable;
        states::EnumState::value_type entry;
        states::EnumState::index_type index;
        states::EnumState::size_type size;
        states::EnumState::EnumList list;

        m_stream >> command >> writeable >> index >> size;

        while(size-- > 0)
        {
            m_stream >> entry;

            list.push_back(entry);
        }

        state.assign(list.begin(), list.end());
        state.setIndex(index);
    }

    template<typename StreamType>
    inline void EntityReader<StreamType>::visit(Parameter&, states::StringState& state)
    {
        bool command;
        bool writeable;
        int max;
        String value;

        m_stream >> command >> writeable >> max >> value;

        state.setCommand(command);
        state.setWriteable(writeable);
        state.setMaxLength(max);
        state.setValue(value);
    }
}
}

#endif // ENTITYREADER_H
