#ifndef __TINYEMBERROUTER_MODEL_FUNCTION_H
#define __TINYEMBERROUTER_MODEL_FUNCTION_H

#include "Element.h"

namespace model
{
   class ElementVisitor;

   /**
     * Represents an Ember+ RPC Function.
     * Translates to EmberPlus-Glow.Function or EmberPlus-Glow.QualifiedFunction.
     */
   class Function : public Element
   {
   public:
      
      /**
        * Encapsulates the function call executed when an Ember+ function is invoked.
        */
      class Delegate
      {
      public:
         virtual void invoke(util::VariantValueVector const& arguments,
                             util::VariantValueVector& result) = 0;
      };

   public:
      typedef std::vector<util::TupleItem> TupleItemVector;

      /**
        * Creates a new instance of Function.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param delegate Points to the function target. Will be deleted in th
        *     Function destructor.
        * @param firstArgument Iterator pointing to the first argument description.
        * @param lastArgument Iterator pointing behind the last argument description.
        * @param firstResult Iterator pointing to the first result field description.
        * @param lastResult Iterator pointing behind the last result field description.
        */
      template<typename TupleItemIterator>
      Function(int number,
               Element* parent,
               std::string const& identifier,
               Delegate* delegate,
               TupleItemIterator firstArgument, TupleItemIterator lastArgument,
               TupleItemIterator firstResult, TupleItemIterator lastResult);

      /**
        * Creates a new instance of Function that does not have a return value.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param delegate Points to the function target. Will be deleted in th
        *     Function destructor.
        * @param firstArgument Iterator pointing to the first argument description.
        * @param lastArgument Iterator pointing behind the last argument description.
        */
      template<typename TupleItemIterator>
      Function(int number,
               Element* parent,
               std::string const& identifier,
               Delegate* delegate,
               TupleItemIterator firstArgument, TupleItemIterator lastArgument);

      ~Function();

      /**
        * Returns a constant collection of the function's argument descriptions.
        * @return A constant collection of the function's argument descriptions.
        */
      inline TupleItemVector const& arguments() const { return m_arguments; }

      /**
        * Returns a constant collection of the function's result field descriptions.
        * @return A constant collection of the function's result field descriptions.
        */
      inline TupleItemVector const& result() const { return m_result; }

      /**
        * Invokes the function.
        * @param firstArgumentValue Iterator pointing to the first argument value.
        * @param lastArgumentValue Iterator pointing behind the last argument value.
        */
      template<typename VariantValueIterator>
      bool invoke(VariantValueIterator firstArgumentValue, VariantValueIterator lastArgumentValue,
                  util::VariantValueVector& result) const;

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);

   private:
      Delegate* m_delegate;
      TupleItemVector m_arguments;
      TupleItemVector m_result;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   template<typename TupleItemIterator>
   Function::Function(int number,
                      Element* parent,
                      std::string const& identifier,
                      Function::Delegate* delegate,
                      TupleItemIterator firstArgument, TupleItemIterator lastArgument,
                      TupleItemIterator firstResult, TupleItemIterator lastResult)
      : Element(number, parent, identifier)
      , m_delegate(delegate)
      , m_arguments(firstArgument, lastArgument)
      , m_result(firstResult, lastResult)
   {
   }

   template<typename TupleItemIterator>
   Function::Function(int number,
                      Element* parent,
                      std::string const& identifier,
                      Function::Delegate* delegate,
                      TupleItemIterator firstArgument, TupleItemIterator lastArgument)
      : Element(number, parent, identifier)
      , m_delegate(delegate)
      , m_arguments(firstArgument, lastArgument)
   {
   }

   template<typename VariantValueIterator>
   bool Function::invoke(VariantValueIterator firstArgumentValue, VariantValueIterator lastArgumentValue,
                         util::VariantValueVector& result) const
   {
      auto argumentValues = util::VariantValueVector(firstArgumentValue, lastArgumentValue);

      if(argumentValues.size() != m_arguments.size())
         return false;

      for(size_type index = 0; index < argumentValues.size(); index++)
      {
         if(argumentValues[index].type().value() != m_arguments[index].type().value())
            return false;
      }

      m_delegate->invoke(argumentValues, result);

      if(result.size() != m_result.size())
         return false;

      for(size_type index = 0; index < result.size(); index++)
      {
         if(result[index].type().value() != m_result[index].type().value())
            return false;
      }

      return true;
   }
}

#endif//__TINYEMBERROUTER_MODEL_FUNCTION_H
