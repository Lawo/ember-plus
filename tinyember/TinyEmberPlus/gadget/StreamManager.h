#ifndef __TINYEMBER_STREAMMANAGER_H
#define __TINYEMBER_STREAMMANAGER_H

#include <vector>
#include "ParameterTypeVisitor.h"

namespace gadget
{
    class Parameter;

    class StreamManager
    {
        friend class Parameter;
        typedef std::vector<Parameter*> ParameterCollection;
        public:
            typedef ParameterCollection::const_iterator const_iterator;
            typedef ParameterCollection::size_type size_type;

            static StreamManager& instance();

            const_iterator begin() const;

            const_iterator end() const;

            size_type size() const;

            void updateValues();

            bool isParameterTransmittedViaStream(Parameter const* parameter) const;

        private:
            void registerParameter(Parameter* parameter);

            void unregisterParameter(Parameter* parameter);

        private:
            ParameterCollection m_parameters;

            class RandomValueGenerator : public ParameterTypeVisitor
            {
                public:
                    virtual void visit(EnumParameter* parameter);

                    virtual void visit(StringParameter* parameter);

                    virtual void visit(IntegerParameter* parameter);

                    virtual void visit(RealParameter* parameter);
            };
    };
}

#endif//__TINYEMBER_STREAMMANAGER_H
