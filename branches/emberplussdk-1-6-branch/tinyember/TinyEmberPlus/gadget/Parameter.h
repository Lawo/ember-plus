#ifndef __TINYEMBER_GADGET_PARAMETER_H
#define __TINYEMBER_GADGET_PARAMETER_H

#include <list>
#include <memory>
#include "../Types.h"
#include "Access.h"
#include "DirtyStateListener.h"
#include "Formula.h"
#include "ParameterField.h"
#include "ParameterType.h"
#include "StreamDescriptor.h"
#include "Subscriber.h"

namespace gadget
{
    /**
     * Forward declarations
     */
    class Node;
    class ParameterTypeVisitor;
    class ParameterTypeVisitorConst;

    /**
     * Base class for all parameter types. This class derives from the Subscribable interface
     * which manages subscription requests.
     */
    class Parameter : public Subscribable
    {
        friend class Node;
        public:
            typedef DirtyStateListener<ParameterFieldState::flag_type, Parameter const*> DirtyStateListener;

            /** Destructor */
            virtual ~Parameter();

            /**
             * This method is used to resolve the concrete parameter type.
             * @param visitor The visitor which is interested in the concrete
             *      parameter type.
             */
            virtual void accept(ParameterTypeVisitor& visitor) = 0;

            /**
             * This method is used to resolve the concrete parameter type.
             * @param visitor The visitor which is interested in the concrete
             *      parameter type.
             */
            virtual void accept(ParameterTypeVisitorConst const& visitor) const = 0;

            /**
             * Returns a formatted string that contains the current parameter value
             * as it would be displayed by a user interface.
             * @return A formatted string that contains the current parameter value.
             */
            virtual std::string toDisplayValue() const = 0;

            /**
             * Returns the number identifying this parameter.
             * @return The number identifying this parameter.
             */
            int number() const;

            /**
             * Returns the parameter's stream identifier. If not set, -1 will be returned.
             * @return The parameter's stream identifier.
             */
            int streamIdentifier() const;

            /**
             * Returns the string identifier of this parameter.
             * @return The string identifier of this parameter.
             */
            String const& identifier() const;

            /**
             * Returns the description or display string of this parameter. This string may
             * be empty.
             * @return The parameter's description string.
             */
            String const& description() const;

            /**
             * Returns the current schema identifier. If no schema is specified, an empty
             *      string is being returned.
             * @return The current schema identifier.
             */
            String const& schema() const;

            /**
             * Returns the parameter's type.
             * @return The parameter's type.
             */
            ParameterType const type() const;

            /**
             * Returns the parameter's parent node.
             * @return The parameter's parent node.
             */
            Node const* parent() const;

            /**
             * Returns the current access to the parameter value.
             * @return The current access to the parameter value.
             */
            Access access() const;

            /**
             * Returns the parameter's formula. The formula contains the two terms
             * used to tranform a provider value into a consumer value and vice versa.
             * If no terms are set, empty strings will be returned for the terms.
             * @return The parameter's formula.
             */
            Formula const& formula() const;

            /**
             * Returns the current dirty state of the parameter.
             * @return The current dirty state of the parameter.
             */
            ParameterFieldState const& dirtyState() const;

            /**
             * Returns a const pointer to the parameter's stream descriptor. If no descriptor is
             * set, null is being returned.
             * @return The parameter's stream descriptor or null, if no descriptor is set.
             */
            StreamDescriptor const* streamDescriptor() const;

            /**
             * Returns true if at least one property is marked dirty.
             * @return true if at least one property is marked dirty.
             */
            bool isDirty() const;

            /**
             * Resets the dirty state of this parameter.
             */
            void clearDirtyState();

            /**
             * Updates the parameter description string.
             * @param value The new description string.
             */
            void setDescription(String const& value);

            /**
             * Changes the value access for this parameter.
             * @param value The new access to assume.
             */
            void setAccess(Access const& value);

            /**
             * Updates the parameter's formula.
             * @param formula The new formula to copy.
             */
            void setFormula(Formula const& formula);

            /**
             * Sets a new stream identifier.
             * @param value The new stream identifier to set. Set to -1 if the parameter
             *      has no stream identifier.
             */
            void setStreamIdentifier(int value);

            /**
             * Sets a new schema identifier. To remove the identifier,
             * simply pass an empty string.
             * @param value The new schema identifier.
             */
            void setSchema(String const& value);

            /**
             * Updates the parameter's stream descriptor or removes it.
             * @param The new stream descriptor. To remove the descriptor, pass null.
             */
            void setStreamDescriptor(std::shared_ptr<StreamDescriptor> descriptor = nullptr);

            /**
             * Sets a new stream descriptor.
             * @param format The value format.
             * @param offset The value offset within the stream.
             */
            void setStreamDescriptor(StreamFormat const& format, unsigned offset);

            /**
             * Registers a new dirty state listener for this parameter.
             * @param listener The listener instance to add to the collection of listeners.
             */
            void registerListener(DirtyStateListener* listener);

            /**
             * Removes an existing dirty state listener from this parameter.
             * @param listener The listener to remove.
             */
            void unregisterListener(DirtyStateListener* listener);

            /**
             * Returns true if a valid stream identifier is set.
             * @return true if a stream identifier is set, otherwise false.
             */
            bool hasStreamIdentifier() const;

            /**
             * Returns true if a stream descriptor exists.
             * @return true if a stream descriptor exists, otherwise false.
             */
            bool hasStreamDescriptor() const;

        protected:
            /**
             * Initializes a new Parameter.
             * @param type The value type of this parameter.
             * @param parent The parent node of this parameter.
             * @param identifier The parameter's identifier string.
             * @param number The parameter's number.
             */
            Parameter(ParameterType const& type, Node* parent, String const& identifier, int number);

            /**
             * Marks one or more properties as dirty.
             * @param field The fields to mark dirty.
             * @param notify If set to true, the registered listeners will be informed
             *      about the state change.
             */
            void markDirty(ParameterField const& field, bool notify);

            /**
             * Informs all registered state listeners about the current dirty state of this parameter.
             */
            void notify() const;

        protected:
            /**
             * If there is at least one subscription and a valid stream identifier, the
             * parameter registers itself to the stream manager.
             */
            virtual void subscribed();

            /**
             * When there are no more active subscriptions, the parameter unregisters itself
             * from the stream manager.
             */
            virtual void unsubscribed();

            /**
             * When the subscription count changes, the registered dirty state listeners will
             * be informed about that update.
             * @param newCount Contains the current number of subscribers.
             */
            virtual void subscriptionCountChanged(size_type newCount);

        private:
            ParameterType const m_type;
            int const m_number;
            int m_streamIdentifier;
            String const m_identifier;
            String m_description;
            String m_schema;
            Node* m_parent;
            Formula m_formula;
            ParameterFieldState m_state;
            Access::value_type m_access;
            std::list<DirtyStateListener*> m_listeners;
            std::shared_ptr<StreamDescriptor> m_streamDescriptor;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETER_H
