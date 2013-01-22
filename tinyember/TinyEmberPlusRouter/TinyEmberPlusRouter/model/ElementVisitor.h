#ifndef __TINYEMBERROUTER_MODEL_ELEMENTVISITOR_H
#define __TINYEMBERROUTER_MODEL_ELEMENTVISITOR_H

namespace model
{
   class Node;
   class IntegerParameter;
   class StringParameter;

   namespace matrix
   {
      class OneToNLinearMatrix;
      class NToNLinearMatrix;
      class NToNNonlinearMatrix;
      class DynamicNToNLinearMatrix;
   }

   /**
     * Visitor Interface accepted by Element::accept().
     */
   class ElementVisitor
   {
   public:
      virtual void visit(Node* element) = 0;
      virtual void visit(IntegerParameter* element) = 0;
      virtual void visit(StringParameter* element) = 0;
      virtual void visit(matrix::OneToNLinearMatrix* element) = 0;
      virtual void visit(matrix::NToNLinearMatrix* element) = 0;
      virtual void visit(matrix::NToNNonlinearMatrix* element) = 0;
      virtual void visit(matrix::DynamicNToNLinearMatrix* element) = 0;
   };
}

#endif//__TINYEMBERROUTER_MODEL_ELEMENTVISITOR_H
