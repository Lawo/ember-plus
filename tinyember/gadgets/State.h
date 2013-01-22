#ifndef PARAMETERSTATE_H
#define PARAMETERSTATE_H

#include "Entity.h"

/** Forward declarations **/
namespace libember { namespace glow
{
    class Value;
}
}


namespace gadgets
{
    /** Forward declarations **/
    class EntityStateVisitor;
    class EntityTypeVisitor;
    class Parameter;

    /**
     * A state describes the value-type of a parameter
     */
    struct StateType
    {
        enum _Domain
        {
            None,
            Integer,
            Real,
            Enumeration,
            String
        };

        typedef std::size_t value_type;

        /**
         * Constructor
         * @param State type
         */
        StateType(value_type const value)
            : value(value)
        {}

        value_type const value;
    };


    /**
     * Base class for parameter states. The state defines the value type
     * of a parameter and it's properties.
     */
    class State
    {
    public:
        /**
         * Creates a new state
         * @param type Type
         * @param owner The entity owning this state, usually a parameter
         * @param writeable Writeable flag
         * @return Returns the created parameter state
         */
        static State *create(StateType const type, Entity const& owner, bool writeable);

    public:
        /**
         * Destructor
         */
        virtual ~State();

        /**
         * Tries to set a value. The implementation has to try to convert it to
         * its preferred type.
         * @param value The value to assign to this parameter state.
         * @return Returns true if the value could be assigned, otherwise false.
         */
        virtual bool trySetValue(libember::glow::Value const& value) = 0;

        /**
         * Creates a copy of this instance, but with a new owner.
         * @param owner The owner of the copy.
         * @return Returns the new instance.
         */
        virtual State* clone(Entity const& owner) const = 0;

        /**
         * This visitor is being used to determine the concrete type of the state
         * @param visitor Visitor
         * @param parameter Parameter
         */
        void visit(EntityTypeVisitor& visitor, Parameter& parameter);

        /**
         * This visitor is used to access the dirty properties of the state.
         * @param visitor Visitor
         * @param parameter The parameter calling this visit method.
         * @param state Contains the properties that shall be reported to the visitor.
         */
        void visit(EntityStateVisitor& visitor, Parameter const& parameter, PropertyState const& state) const;

        /**
         * Changes the command flag
         * @param command New value
         */
        void setCommand(bool command);

        /**
         * Changes the writeable flag
         * @param writeable New value
         */
        void setWriteable(bool writeable);

        /**
         * Returns the current command state
         * @return Returns true if this parameter is a command, otherwise false.
         */
        bool isCommand() const;

        /**
         * Returns the current writeable state
         * @return Returns true if the parameter is writeable, otherwise false.
         */
        bool isWriteable() const;

    protected:
        /**
         * Constructor
         * @param owner The entity owning this state
         * @param writeable Initial writeable flag
         * @param command Initial command flag
         */
        State(Entity const& owner, bool writeable = true, bool command = false);

        /**
         * Sets a property to dirty, calls the owner's markDirty method
         * @param property Property to mark as dirty
         */
        void markDirty(Property const property) const;

        /**
         * Called by the visit method. The implementation is responsible of adding all properties requested.
         * @param visitor Visitor
         * @param parameter Parameter
         * @param state The properties that shall be reported to the visitor
         */
        virtual void visitImpl(EntityStateVisitor& visitor, Parameter const& parameter, PropertyState const& state) const = 0;

        /**
         * Called by the visit method. The implementation is used to resolve the concrete state type
         * @param visitor Type Visitor
         * @param parameter Parameter calling this state's visit method
         * @param identifier The parameter's identifier
         * @param number The parameter's number
         */
        virtual void visitImpl(EntityTypeVisitor& visitor, Parameter& parameter) = 0;

    private:
        Entity const& m_owner;
        bool m_isCommand;
        bool m_isWriteable;
    };

    /**************************************************************************
     * Inline Implementation                                                  *
     **************************************************************************/

    inline void State::markDirty(Property const property) const
    {
        m_owner.markDirty(property);
    }

    inline bool State::isCommand() const
    {
        return m_isCommand;
    }

    inline bool State::isWriteable() const
    {
        return m_isWriteable;
    }

    inline void State::setCommand(bool command)
    {
        if (m_isCommand != command)
        {
            m_isCommand = command;

            markDirty(Property::IsCommand);
        }
    }

    inline void State::setWriteable(bool writeable)
    {
        if (m_isWriteable != writeable)
        {
            m_isWriteable = writeable;

            markDirty(Property::IsWriteable);
        }
    }
}

#endif // PARAMETERSTATE_H
