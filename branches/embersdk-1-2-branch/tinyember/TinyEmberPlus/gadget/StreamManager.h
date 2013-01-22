#ifndef __TINYEMBER_STREAMMANAGER_H
#define __TINYEMBER_STREAMMANAGER_H

#include <vector>
#include "ParameterTypeVisitor.h"

namespace gadget
{
    /** Forward declaration */
    class Parameter;

    /**
     * The StreamManager is a singleton which contains all parameters that have a valid streamIdentifier
     * set. The parameter register themseves as soon as there is at least one subscriber for a parameter.
     */
    class StreamManager
    {
        friend class Parameter;
        typedef std::vector<Parameter*> ParameterCollection;
        public:
            typedef ParameterCollection::const_iterator const_iterator;
            typedef ParameterCollection::size_type size_type;

            /**
             * Returns the singleton instance of the StreamManager.
             * @return The singleton instance of the StreamManager.
             */
            static StreamManager& instance();

            /**
             * Returns a reference to the first parameter that is registered to the manager.
             * @return A reference to the first parameter that is registered to the manager.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator that points one past the last parameter that is registered.
             * @return An iterator that points one past the last parameter that is registered.
             */
            const_iterator end() const;

            /**
             * Returns the number of streams that are currently registered.
             * @return The number of streams that are currently registered.
             */
            size_type size() const;

            /**
             * Generates a random value for all registered parameters.
             */
            void updateValues();

            /**
             * Tests whether the value of the passed parameter has to be transmitted via
             * a stream. This is the case when a parameter contains a stream identifier
             * or descriptor.
             * @param parameter The parameter to test.
             * @return True if the parameter value is transmitted as stream, false if
             *      is being transmitted in the usual way.
             */
            bool isParameterTransmittedViaStream(Parameter const* parameter) const;

        private:
            /**
             * Registers a parameter. This method is invoked by a parameter when
             * a consumer subscribes to it.
             * @param parameter The parameter to register.
             */
            void registerParameter(Parameter* parameter);

            /**
             * Unregisters a parameter. This method is invoked by a parameter when
             * it has no more subscribers and must therefore no longer be transmitted
             * via a stream.
             * @param parameter The parameter to unregister.
             */
            void unregisterParameter(Parameter* parameter);

        private:
            ParameterCollection m_parameters;

        private:
            /**
             * Simple random value generator for the supported parameter types.
             */
            class RandomValueGenerator : public ParameterTypeVisitor
            {
                public:
                    /**
                     * Generates a new index for the enumeration parameter.
                     * @param parameter The parameter to modify.
                     */
                    virtual void visit(EnumParameter* parameter);

                    /**
                     * Generates a new value for the string parameter.
                     * @param parameter The parameter to modify.
                     */
                    virtual void visit(StringParameter* parameter);

                    /**
                     * Generates a new value for the integer parameter.
                     * @param parameter The parameter to modify.
                     */
                    virtual void visit(IntegerParameter* parameter);

                    /**
                     * Generates a new value for the real parameter.
                     * @param parameter The parameter to modify.
                     */
                    virtual void visit(RealParameter* parameter);

                    /**
                     * Generates a new value for the boolean parameter.
                     * @param parameter The parameter to modify.
                     */
                    virtual void visit(BooleanParameter* parameter);
            };
    };
}

#endif//__TINYEMBER_STREAMMANAGER_H
