#ifndef __TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H
#define __TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H

namespace gadget
{
    /** Forward declarations */
    class Parameter;
    class BooleanParameter;
    class EnumParameter;
    class StringParameter;
    class IntegerParameter;
    class RealParameter;

    /**
     * Interface for a parameter visitor.
     */
    class ParameterTypeVisitor
    {
        public:
            /** Destructor */
            virtual ~ParameterTypeVisitor()
            {}

            /**
             * This method is invoked when the parameter being visited is of type EnumParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(EnumParameter* parameter) = 0;

            /**
             * This method is invoked when the parameter being visited is of type StringParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(StringParameter* parameter) = 0;

            /**
             * This method is invoked when the parameter being visited is of type IntegerParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(IntegerParameter* parameter) = 0;

            /**
             * This method is invoked when the parameter being visited is of type RealParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(RealParameter* parameter) = 0;

            /**
             * This method is invoked when the parameter being visited is of type BooleanParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(BooleanParameter* parameter) = 0;
    };

    /**
     * Interface for a parameter type visitor, which can be used to visit constant
     * references.
     */
    class ParameterTypeVisitorConst
    {
        public:
            virtual ~ParameterTypeVisitorConst()
            {}

            /**
             * This method is invoked when the parameter being visited is of type EnumParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(EnumParameter const* parameter) const = 0;

            /**
             * This method is invoked when the parameter being visited is of type StringParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(StringParameter const* parameter) const = 0;

            /**
             * This method is invoked when the parameter being visited is of type IntegerParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(IntegerParameter const* parameter) const = 0;

            /**
             * This method is invoked when the parameter being visited is of type RealParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(RealParameter const* parameter) const = 0;

            /**
             * This method is invoked when the parameter being visited is of type BooleanParameter.
             * @param parameter The concrete parameter.
             */
            virtual void visit(BooleanParameter const* parameter) const = 0;
    };
}

#endif//__TINYEMBER_GADGET_PARAMETERTYPEVISITOR_H
