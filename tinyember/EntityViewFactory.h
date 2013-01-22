#ifndef ENTITYVIEWFACTORY_H
#define ENTITYVIEWFACTORY_H

#include <QFrame>
#include "Types.h"
#include "Gadgets/EntityTypeVisitor.h"


/**
 * Factory class with creates a view item from an entity
 */
class EntityViewFactory : gadgets::EntityTypeVisitor
{
    typedef gadgets::Entity Entity;
    typedef gadgets::Node Node;
    typedef gadgets::Parameter Parameter;
    typedef gadgets::states::EnumState EnumState;
    typedef gadgets::states::NumericState<int> IntState;
    typedef gadgets::states::NumericState<double> DoubleState;
    typedef gadgets::states::StringState StringState;

public:
    /**
     * Creates a new view item for the specified entity
     * @param parent The parent widget of the view to create
     * @param entity The entity to create a view for
     * @return Returns the created view
     */
    static QFrame *createView(QWidget* parent, gadgets::Entity* entity);

protected:
    /**
     * Returns the created view.
     * @return Returns the view that has been created for the provided entity.
     */
    QFrame* view();

private:
    /**
     * Constructor
     * @param parent The parent window of the view.
     * @param entity The entity to create a view for.
     */
    EntityViewFactory(QWidget* parent, gadgets::Entity* entity);

    /**
     * Creates a view for a node.
     * @see EntityTypeVisitor.
     */
    virtual void visit(Node& node);

    /**
     * Creates a view for an integer parameter.
     * @see EntityTypeVisitor.
     */
    virtual void visit(Parameter& parameter, IntState& state);

    /**
     * Creates a view for a real parameter.
     * @see EntityTypeVisitor.
     */
    virtual void visit(Parameter& parameter, DoubleState& state);

    /**
     * Creates a view for a enumerated parameter.
     * @see EntityTypeVisitor.
     */
    virtual void visit(Parameter& parameter, EnumState& state);

    /**
     * Creates a view for a string parameter.
     * @see EntityTypeVisitor.
     */
    virtual void visit(Parameter& parameter, StringState& state);

private:
    QFrame *m_view;
    QWidget *const m_parent;
};

#endif // ENTITYVIEWFACTORY_H
