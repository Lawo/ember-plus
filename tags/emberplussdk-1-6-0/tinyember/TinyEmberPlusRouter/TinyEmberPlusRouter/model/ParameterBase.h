#ifndef __TINYEMBERROUTER_MODEL_PARAMETERBASE_H
#define __TINYEMBERROUTER_MODEL_PARAMETERBASE_H

#include "Element.h"
#include "NotificationSink.h"

namespace model
{
   /**
     * Abstract base class for parameters.
     */
   class ParameterBase : public Element
   {
   public:
      /**
        * Creates a new instance of ParameterBase.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink Pointer to the object receiving value change
        *     notifications.
        */
      ParameterBase(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink);

      /**
        * Returns a pointer to the object receiving value change notifications.
        * @return A pointer to the object receiving value change notifications.
        */
      inline NotificationSink* notificationSink() const { return m_notificationSink; }

      /**
        * Returns true if the parameter cannot be set via Ember+.
        * @return True if the parameter cannot be set via Ember+.
        */
      inline bool isReadOnly() const { return m_isReadOnly; }

      /**
        * Sets a value indicating whether the parameter can be set via Ember+.
        * @param value A value indicating whether the parameter can be set via Ember+.
        */
      inline void setReadOnly(bool value)
      {
         m_isReadOnly = value;
      }

   private:
      NotificationSink* m_notificationSink;
      bool m_isReadOnly;
   };
}

#endif//__TINYEMBERROUTER_MODEL_PARAMETERBASE_H
