#ifndef __TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H
#define __TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H

namespace gadget
{
    class Parameter;
    class EnumParameter;
    class StringParameter;
    class IntegerParameter;
    class RealParameter;

    class ParameterTypeVisitor
    {
        public:
            virtual ~ParameterTypeVisitor()
            {}

            virtual void visit(EnumParameter* parameter) = 0;

            virtual void visit(StringParameter* parameter) = 0;

            virtual void visit(IntegerParameter* parameter) = 0;

            virtual void visit(RealParameter* parameter) = 0;
    };

    class ParameterTypeVisitorConst
    {
        public:
            virtual ~ParameterTypeVisitorConst()
            {}

            virtual void visit(EnumParameter const* parameter) const = 0;

            virtual void visit(StringParameter const* parameter) const = 0;

            virtual void visit(IntegerParameter const* parameter) const = 0;

            virtual void visit(RealParameter const* parameter) const = 0;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H
