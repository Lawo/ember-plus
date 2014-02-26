#ifndef __TINYEMBERROUTER_MODEL_MATRIX_DYNAMICNTONLINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_DYNAMICNTONLINEARMATRIX_H

#include "LinearMatrix.h"
#include "../Node.h"
#include "../IntegerParameter.h"
#include "../DynamicElementEmitter.h"

namespace model { namespace matrix
{
   /**
     * Matrix with N:N connection semantics, linear addressing mode
     * and dynamic parameter generation.
     * Only provides parameters for crosspoints.
     */
   class DynamicNToNLinearMatrix : public LinearMatrix, public DynamicElementEmitter
   {
   // ========================================================
   // DynamicNode
   // ========================================================
   private:
      /**
        * Type used for nodes generated on-the-fly.
        */
      class DynamicNode : public Node
      {
         friend class DynamicNToNLinearMatrix;

         DynamicNode(util::Oid const& path);

         /**
           * Overridden to return m_path.
           */
         virtual util::Oid path() const { return m_path; }

         util::Oid m_path;
      };


   // ========================================================
   // DynamicIntegerParameter
   // ========================================================
   private:
      /**
        * Type used for integer parameters generated on-the-fly.
        */
      class DynamicIntegerParameter : public IntegerParameter
      {
         friend class DynamicNToNLinearMatrix;

         DynamicIntegerParameter(util::Oid const& path,
                                 Element* parent,
                                 std::string identifier,
                                 DynamicNToNLinearMatrix* owner,
                                 int value,
                                 int minimum, int maximum);

         /**
           * Overridden to notify owner of inbound parameter change.
           */
         virtual void onValueChanged();

         /**
           * Overridden to return m_path.
           */
         virtual util::Oid path() const { return m_path; }

         DynamicNToNLinearMatrix* m_owner;
         util::Oid m_path;
      };


   // ========================================================
   // DynamicNToNLinearMatrix
   // ========================================================
   public:
      /**
        * Creates a new instance of DynamicNToNLinearMatrix.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink The object to notify of parameter value changes
        *     and matrix connections.
        * @param targetCount The number of targets to create.
        * @param sourceCount The number of sources to create.
        */
      DynamicNToNLinearMatrix(int number,
                              Element* parent,
                              std::string const& identifier,
                              NotificationSink* notificationSink,
                              int targetCount, int sourceCount);

      /**
        * Destructor
        */
      virtual ~DynamicNToNLinearMatrix();

      /**
        * Returns the sub-id used to address dynamically generated descendants.
        * @return The sub-id used to address dynamically generated descendants.
        */
      inline int parametersSubid() const { return 0; }

      /**
        * Returns the minimum value of each crosspoint's "gain" parameter
        * @return The minimum value of each crosspoint's "gain" parameter
        */
      inline int minimumGain() const { return -128; }

      /**
        * Returns the maximum value of each crosspoint's "gain" parameter
        * @return The maximum value of each crosspoint's "gain" parameter
        */
      inline int maximumGain() const { return 15; }

      /**
        * Returns the number of each crosspoint's "gain" parameter.
        * @return The number of each crosspoint's "gain" parameter.
        */
      inline int gainParameterNumber() const { return 1; }

      /**
        * Overridden to call the appropriate visit() overload.
        */
      virtual void accept(ElementVisitor* visitor);

   protected:
      /**
        * Overridden to execute connects according to N:N connection semantics.
        */
      virtual bool connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);

   private:
      /**
        * Invoked by DynamicIntegerParameter on inbound value changes.
        */
      bool onParameterValueChanged(util::Oid const& path, int value);

   private:
      int* m_xpointGains;

   // ========================================================
   // DynamicElementEmitter Members
   // ========================================================
   public:
      /**
        * Implemented to emit descendant Elements on-the-fly.
        */
      virtual Element* emitDescendant(util::Oid const& path);
   };
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_DYNAMICNTONLINEARMATRIX_H
