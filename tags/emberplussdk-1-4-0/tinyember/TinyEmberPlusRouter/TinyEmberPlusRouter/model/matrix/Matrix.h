#ifndef __TINYEMBERROUTER_MODEL_MATRIX_MATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_MATRIX_H

#include "../Element.h"
#include "Signal.h"
#include "../NotificationSink.h"
#include "../../util/Collection.h"
#include "../../util/Types.h"

namespace model { namespace matrix
{
   /**
     * Abstract base class for all matrix classes.
     */
   class Matrix : public Element
   {
   public:
      /**
        * Creates a new instance of DynamicNToNLinearMatrix.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink The object to notify of parameter value changes
        *     and matrix connections.
        */
      Matrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink);

      /**
        * Destructor.
        */
      virtual ~Matrix();

      /**
        * Returns a constant collection of the targets owned by the matrix.
        * @return A constant collection of the targets owned by the matrix.
        */
      inline Signal::Vector const& targets() const { return m_targets; }

      /**
        * Returns a constant collection of the sources owned by the matrix.
        * @return A constant collection of the sources owned by the matrix.
        */
      inline Signal::Vector const& sources() const { return m_sources; }

      /**
        * Returns a modifiable collection of the targets owned by the matrix.
        * @return A modifiable collection of the targets owned by the matrix.
        */
      inline Signal::Vector& targets() { return m_targets; }

      /**
        * Returns a modifiable collection of the sources owned by the matrix.
        * @return A modifiable collection of the sources owned by the matrix.
        */
      inline Signal::Vector& sources() { return m_sources; }

      /**
        * Returns a pointer to the object to notify of parameter value changes and matrix connections.
        * @return A pointer to the object to notify of parameter value changes and matrix connections.
        */
      inline NotificationSink* notificationSink() const { return m_notificationSink; }

      /**
        * Returns the path to the Node containing signal labels.
        * @return The path to the Node containing signal labels.
        */
      inline util::Oid labelsPath() const { return m_labelsPath; }

      /**
        * Sets the path to the Node containing signal labels.
        * @param value The path to the Node containing signal labels.
        */
      inline void setLabelsPath(util::Oid const& value)
      {
         m_labelsPath = value;
      }

      /**
        * Issues a connect operation on the specified @p target.
        * @param target Pointer to the target to connect to.
        *     Must be contained in the collection returned by targets().
        * @param firstSource Iterator pointing to the first source to connect.
        * @param lastSource Iterator pointing behind the last source to connect.
        * @param state Caller-defined state to be passed through.
        * @param operation The connect operation to issue.
        */
      template<typename InputIterator>
      void connect(Signal* target, InputIterator firstSource, InputIterator lastSource, void* state, util::ConnectOperation const& operation);

      /**
        * Issues a connect operation on the specified @p target with
        * default operation "absolute".
        * @param target Pointer to the target to connect to.
        *     Must be contained in the collection returned by targets().
        * @param firstSource Iterator pointing to the first source to connect.
        * @param lastSource Iterator pointing behind the last source to connect.
        * @param state Caller-defined state to be passed through.
        */
      template<typename InputIterator>
      void connect(Signal* target, InputIterator firstSource, InputIterator lastSource, void* state);

      /**
        * Returns the number of targets owned by the matrix.
        * @return The number of targets owned by the matrix.
        */
      virtual int targetCount() const = 0;

      /**
        * Returns the number of sources owned by the matrix.
        * @return The number of sources owned by the matrix.
        */
      virtual int sourceCount() const = 0;

      /**
        * Returns a pointer to the target with the specified number.
        * @param number The number of the target to get.
        * @return A pointer to the target with the specified number.
        */
      virtual Signal* getTarget(int number) const = 0;

      /**
        * Returns a pointer to the source with the specified number.
        * @param number The number of the source to get.
        * @return A pointer to the source with the specified number.
        */
      virtual Signal* getSource(int number) const = 0;

   protected:
      /**
        * Implement this method in a derived class to issue connects
        * according to specific connection semantics.
        * @param target Pointer to the target to connect to.
        * @param sources A collection of the sources to connect.
        * @param state Caller-defined state to be passed through.
        * @param operation The connect operation to issue.
        * @return True if the connected sources of @p target were updated.
        */
      virtual bool connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation) = 0;

   private:
      Signal::Vector m_targets;
      Signal::Vector m_sources;
      NotificationSink* m_notificationSink;
      util::Oid m_labelsPath;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   template<typename InputIterator>
   inline void Matrix::connect(Signal* target, InputIterator firstSource, InputIterator lastSource, void* state, util::ConnectOperation const& operation)
   {
      if(util::contains(m_targets.begin(), m_targets.end(), target) == false)
         throw std::runtime_error("target");

      if(firstSource != lastSource)
      {
         if(util::contains(m_sources.begin(), m_sources.end(), *firstSource) == false)
            throw std::runtime_error("sources");
      }

      auto sources = Signal::Vector(firstSource, lastSource);

      if(connectOverride(target, sources, state, operation))
         m_notificationSink->notifyMatrixConnection(this, target, state);
   }

   template<typename InputIterator>
   inline void Matrix::connect(Signal* target, InputIterator firstSource, InputIterator lastSource, void* state)
   {
      connect(target, firstSource, lastSource, state, util::ConnectOperation::Absolute);
   }
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_MATRIX_H
