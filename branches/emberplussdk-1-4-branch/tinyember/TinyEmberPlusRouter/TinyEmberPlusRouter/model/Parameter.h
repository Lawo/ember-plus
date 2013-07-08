#ifndef __TINYEMBERROUTER_MODEL_PARAMETER_H
#define __TINYEMBERROUTER_MODEL_PARAMETER_H

#include "ParameterBase.h"

namespace model
{
   /**
     * Templated parameter base class.
     */
   template<typename TValue>
   class Parameter : public ParameterBase
   {
   public:
      /**
        * Creates a new instance of Parameter.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink Pointer to the object receiving value change
        *     notifications.
        */
      Parameter(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink);

      /**
        * Returns the current parameter value.
        * @return The current parameter value.
        */
      inline TValue value() const { return m_value; }

      /**
        * Sets the parameter value.
        * @param value the new parameter value.
        * @note Invokes the abstract method onValueChanged.
        */
      inline void setValue(TValue value)
      {
         if(value != m_value)
         {
            m_value = value;
            onValueChanged();
         }
      }

   protected:
      /**
        * Implement this method in derived classes to issue value change notifications.
        */
      virtual void onValueChanged() = 0;

      /**
        * Sets the parameter value without invoking the onValueChanged method.
        * @param value the new parameter value.
        */
      inline void setValueSilent(TValue value)
      {
         m_value = value;
      }

   private:
      TValue m_value;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   template<typename TValue>
   inline Parameter<TValue>::Parameter(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : ParameterBase(number, parent, identifier, notificationSink)
   {}
}

#endif//__TINYEMBERROUTER_MODEL_PARAMETER_H
