#ifndef PARAMETER_H
#define PARAMETER_H

#include "Entity.h"
#include "State.h"

/** Forward declarations **/
namespace libember { namespace glow
{
    class Value;
}
}

namespace gadgets
{
    /** Forward declarations **/
    class Node;
    class State;


    /**
     * Parameter class. To create a parameter, call the Node's insert method.
     */
    class Parameter : public Entity
    {
        friend class Node;

    public:
        /**
         * Destructor
         */
        virtual ~Parameter();

        /**
         * Creates a copy of this parameter
         * @param parent The parent of the copy
         * @param identifier The identifier of the copy
         */
        virtual Entity* clone(Node* parent, String const& identifier) const;

        /**
         * Changes the internal parameter type
         * @param type New type
         */
        void setState(StateType const type);

        /**
         * Calls the state's trySetValue and returns its result.
         * @param value The value to try to set.
         * @return Returns true if the value could be set, otherwise false.
         * @see State::trySetValue
         */
        bool trySetValue(libember::glow::Value const& value);

    private:
        /**
         * Constructor
         * @param parent The parameter's parent
         * @param type The parameter type
         * @param identifier The parameter identifier
         * @param description The parameter description or display name
         */
        Parameter(Node* parent, StateType const type, String const& identifier, String const& description);

        /**
         * Visitor implementation
         * @see Entity
         */
        virtual void visitImpl(EntityStateVisitor& visitor, PropertyState const& properties) const;

        /**
         * Visitor implementation
         * @see Entity
         */
        virtual void visitImpl(EntityTypeVisitor& typeVisitor);

    private:
        State* m_type;
    };
}

#endif // PARAMETER_H
