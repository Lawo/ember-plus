#ifndef __TINYEMBERROUTER_GLOW_DISPATCHER_H
#define __TINYEMBERROUTER_GLOW_DISPATCHER_H

#include <QtCore>
#include "../model/NotificationSink.h"
#include "../net/TcpClientFactory.h"
#include "../net/TcpServer.h"
#include "Walker.h"
#include "../model/Element.h"
#include "../model/ElementVisitor.h"

namespace glow
{
   /**
     * The application's Ember+ front-end.
     * Aggregates net::TcpServer to listen for inbound connections from consumers.
     * Handles inbound Ember+ packages.
     * Creates and dispatches spontaneous Ember+ updates to consumers.
     * Implements two interfaces: model::NotificationSink and net::TcpClientFactory.
     */
   class Dispatcher : public model::NotificationSink, public net::TcpClientFactory
   {
      friend class Consumer;
      friend class Walker;


   // ========================================================
   //
   // Dispatcher::GlowWalker Declaration
   //
   // ========================================================

   private:
      /**
        * Handles inbound Glow packages.
        */
      class GlowWalker : public glow::Walker
      {
      public:
         /**
           * Creates a new instance of Dispatcher::GlowWalker.
           * @param dispatcher Pointer to the owner Dispatcher object.
           * @param source Pointer to the consumer that issued the
           *     Glow tree to walk.
           */
         GlowWalker(Dispatcher* dispatcher, Consumer* source);

      protected:
         /**
           * Overridden to respond to GetDirectory commands.
           */
         virtual void handleCommand(libember::glow::GlowCommand const* glow, libember::ber::ObjectIdentifier const& path);

         /**
           * Overridden to handle parameter value changes.
           */
         virtual void handleParameter(libember::glow::GlowParameterBase const* glow, libember::ber::ObjectIdentifier const& path);

         /**
           * Overridden to handle inbound matrix connects.
           */
         virtual void handleMatrix(libember::glow::GlowMatrixBase const* glow, libember::ber::ObjectIdentifier const& path);

      private:
         Dispatcher* m_dispatcher;
         Consumer* m_source;
      };


   // ========================================================
   //
   // Dispatcher::ElementToGlowConverter Declaration
   //
   // ========================================================

   private:
      /**
        * Implements interface ElementVisitor to create Glow objects
        * from Element objects.
        * Used by method Dispatcher::elementToGlow.
        * After visiting the Element object, the created Glow object
        * must be fetched by calling detachResult().
        */
      class ElementToGlowConverter : public model::ElementVisitor
      {
      public:
         /**
           * Creates a new instance of ElementToGlowConverter.
           * @param dirFieldMask The EmberPlus-Glow.FieldFlags values
           *     indicating the fields to set on the Glow object.
           * @param isCompleteMatrixEnquired True if the Glow object is
           *     to be created in response to a GetDirectory command
           *     on a matrix element.
           *     If true, GlowMatrix objects will be rendered including
           *     The "connections" and (if applicable) the "targets" and
           *     "sources" fields.
           */
         ElementToGlowConverter(int dirFieldMask, bool isCompleteMatrixEnquired);

         /**
           * Fetches the Glow object previously created by one of the
           * visit() overloads and detaches the 
           * @return the Glow object previously created by one of the
           *      visit() overloads. The caller is responsible for
           *      deleting the returned object.
           */
         libember::glow::GlowElement* detachResult();

      public:
         /**
           * Overriden to create GlowQualifiedNode objects from model::Node objects.
           */
         virtual void visit(model::Node* element);

         /**
           * Overriden to create GlowQualifiedParameter objects from model::Parameter objects.
           */
         virtual void visit(model::IntegerParameter* element);

         /**
           * Overriden to create GlowQualifiedParameter objects from model::Parameter objects.
           */
         virtual void visit(model::StringParameter* element);

         /**
           * Overriden to create GlowQualifiedFunction objects from model::Function objects.
           */
         virtual void visit(model::Function* element);

         /**
           * Overriden to create GlowQualifiedMatrix objects from model::Matrix objects.
           */
         virtual void visit(model::matrix::OneToNLinearMatrix* element);

         /**
           * Overriden to create GlowQualifiedMatrix objects from model::Matrix objects.
           */
         virtual void visit(model::matrix::NToNLinearMatrix* element);

         /**
           * Overriden to create GlowQualifiedMatrix objects from model::Matrix objects.
           */
         virtual void visit(model::matrix::NToNNonlinearMatrix* element);

         /**
           * Overriden to create GlowQualifiedMatrix objects from model::Matrix objects.
           */
         virtual void visit(model::matrix::DynamicNToNLinearMatrix* element);

      private:
         inline bool hasDirField(int value) const { return (m_dirFieldMask & value) == value; }
         libember::glow::GlowMatrixBase* MatrixToGlow(model::matrix::Matrix const* element);

         template<typename GlowElementType>
         inline GlowElementType* store(GlowElementType* glow)
         {
            if(m_result != nullptr)
               throw std::runtime_error("Result of previous operation has not been detached!");

            m_result = glow;
            return glow;
         }

      private:
         int m_dirFieldMask;
         bool m_isCompleteMatrixEnquired;
         libember::glow::GlowElement* m_result;
      };


   // ========================================================
   //
   // Dispatcher Declaration
   //
   // ========================================================

   public:
      /**
        * Creates a new instance of Dispatcher, starting a TcpServer
        * on the passed @p port.
        * @param parent The Qt object to parent the aggregated TcpServer.
        * @param port The port the aggregated TcpServer should listen on.
        */
      Dispatcher(QObject* parent, int port);

      // --------------------- properties
      /**
        * Returns the root of the DOM tree.
        * @return The root of the DOM tree.
        */
      inline model::Element* root() const { return m_root; }

      /**
        * Sets the root of the DOM tree.
        * @param value The root of the DOM tree.
        */
      inline void setRoot(model::Element* value)
      {
         m_root = value;
      }


      // --------------------- model::NotificationSink implementation
      /**
        * Implemented to send GlowConnection objects to all connected consumers
        * when a matrix connection has changed in the DOM.
        */
      virtual void notifyMatrixConnection(model::matrix::Matrix* matrix, model::matrix::Signal* target, void* state);

      /**
        * Implemented to send GlowQualifiedParameter objects to all connected consumers
        * when a parameter value has changed in the DOM.
        */
      virtual void notifyParameterValueChanged(util::Oid const& parameterPath, int value);

      /**
        * Implemented to send GlowQualifiedParameter objects to all connected consumers
        * when a parameter value has changed in the DOM.
        */
      virtual void notifyParameterValueChanged(util::Oid const& parameterPath, std::string const& value);


      // --------------------- net::TcpClientFactory implementation
      /**
        * Implemented to create Consumer objects when a new TcpClient has connected.
        */
      virtual net::TcpClient* create(QTcpSocket* socket);

   private:
      void receiveGlow(libember::glow::GlowContainer const* glow, Consumer* source);
      libember::glow::GlowElement* elementToGlow(model::Element* element, int dirFieldMask, bool isCompleteMatrixEnquired) const;

      void writeGlow(libember::glow::GlowContainer const* glow);

   private:
      net::TcpServer m_server;
      model::Element* m_root;
   };
}

#endif//__TINYEMBERROUTER_GLOW_DISPATCHER_H
