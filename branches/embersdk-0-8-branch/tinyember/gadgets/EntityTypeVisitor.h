#ifndef ENTITYTYPEVISITOR_H
#define ENTITYTYPEVISITOR_H

#include "Entity.h"

namespace gadgets
{
    /** Forward declarations **/
    namespace states
    {
        template<typename ValueType>
        class NumericState;
        class EnumState;
        class StringState;
    }

    class Node;
    class Parameter;

    /**
     * The type visitor is used to receive the concrete type of an entity. Additionally, if the entity is
     * a parameter, it will also report its state information
     */
    class EntityTypeVisitor
    {
    public:
        /**
         * Destructor
         */
        virtual ~EntityTypeVisitor();

        /**
         * Called by the entity when its a node
         * @param node The node that is being visited
         */
        virtual void visit(Node& node) = 0;

        /**
         * Called by an integer parameter
         * @param parameter The parameter being visited
         * @param state The parameter's state information
         */
        virtual void visit(Parameter& parameter, states::NumericState<int>& state) = 0;

        /**
         * Called by an double parameter
         * @param parameter The parameter being visited
         * @param state The parameter's state information
         */
        virtual void visit(Parameter& parameter, states::NumericState<double>& state) = 0;

        /**
         * Called by an enumeration parameter
         * @param parameter The parameter being visited
         * @param state The parameter's state information
         */
        virtual void visit(Parameter& parameter, states::EnumState& state) = 0;

        /**
         * Called by an string parameter
         * @param parameter The parameter being visited
         * @param state The parameter's state information
         */
        virtual void visit(Parameter& parameter, states::StringState& state) = 0;

    protected:
        /**
         * Constructor
         */
        EntityTypeVisitor();
    };
}

#endif // ENTITYTYPEVISITOR_H
