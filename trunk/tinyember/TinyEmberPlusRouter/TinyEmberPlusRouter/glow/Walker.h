#ifndef __TINYEMBERROUTER_GLOW_WALKER_H
#define __TINYEMBERROUTER_GLOW_WALKER_H

#include <list>
#include <ember/Ember.hpp>

namespace glow
{
   /**
     * This class is used to walk a glow tree, invoking abstract methods for each
     * encountered GlowElement.
     * This makes the differences between Parameter and QualifiedParameter etc.
     * transparent, since every GlowContentElement is treated like its
     * qualified version.
     */
   class Walker
   {
   public:
      /**
        * Walks a glow tree, invoking abstract methods for each
        * encountered GlowElement.
        * @param glow The root of the Glow tree to walk.
        */
      void walk(libember::glow::GlowContainer const* glow);

   protected:
      /**
        * Override this method to process GlowCommand objects.
        * @param glow Pointer to the GlowCommand object encountered.
        * @param path The path to the GlowCommand's parent element
        *     as defined by e.g. EmberPlus-Glow.QualifiedNode.path.
        */
      virtual void handleCommand(libember::glow::GlowCommand const* glow, libember::ber::ObjectIdentifier const& path) = 0;

      /**
        * Override this method to process GlowParameter objects.
        * @param glow Pointer to the GlowParameter object encountered.
        * @param path The path to the parameter as defined by e.g.
        *     EmberPlus-Glow.QualifiedParameter.path.
        */
      virtual void handleParameter(libember::glow::GlowParameterBase const* glow, libember::ber::ObjectIdentifier const& path) = 0;

      /**
        * Override this method to process GlowMatrix objects.
        * @param glow Pointer to the GlowMatrix object encountered.
        * @param path The path to the matrix as defined by e.g.
        *     EmberPlus-Glow.QualifiedMatrix.path.
        */
      virtual void handleMatrix(libember::glow::GlowMatrixBase const* glow, libember::ber::ObjectIdentifier const& path) = 0;

      /**
        * Override this method to process GlowNode objects.
        * @param glow Pointer to the GlowNode object encountered.
        * @param path The path to the node as defined by e.g.
        *     EmberPlus-Glow.QualifiedNOde.path.
        * @note Only significant for consumer applications.
        */
      virtual void handleNode(libember::glow::GlowNodeBase const* glow, libember::ber::ObjectIdentifier const& path);

      /**
        * Override this method to process GlowFunction objects.
        * @param glow Pointer to the GlowFunction object encountered.
        * @param path The path to the function as defined by e.g.
        *     EmberPlus-Glow.QualifiedFunction.path.
        * @note Only significant for consumer applications.
        */
      virtual void handleFunction(libember::glow::GlowFunctionBase const* glow, libember::ber::ObjectIdentifier const& path);

      /**
        * Override this method to process GlowStreamEntry objects.
        * @param glow Pointer to the GlowStreamEntry object encountered.
        * @note Only significant for consumer applications.
        */
      virtual void handleStreamEntry(libember::glow::GlowStreamEntry const* glow);

      /**
        * Override this method to process GlowInvocationResult objects.
        * @param glow Pointer to the GlowInvocationResult object encountered.
        * @note Only significant for consumer applications.
        */
      virtual void handleInvocationResult(libember::glow::GlowInvocationResult const* glow);

   private:
      inline libember::ber::ObjectIdentifier pathToOid() const { return libember::ber::ObjectIdentifier(m_path.begin(), m_path.end()); }

      void walkNode(libember::glow::GlowNode const* glow);
      void walkParameter(libember::glow::GlowParameter const* glow);
      void walkMatrix(libember::glow::GlowMatrix const* glow);
      void walkFunction(libember::glow::GlowFunction const* glow);
      void walkQualifiedNode(libember::glow::GlowQualifiedNode const* glow);
      void walkQualifiedParameter(libember::glow::GlowQualifiedParameter const* glow);
      void walkQualifiedMatrix(libember::glow::GlowQualifiedMatrix const* glow);
      void walkQualifiedFunction(libember::glow::GlowQualifiedFunction const* glow);
      void walkStreamCollection(libember::glow::GlowStreamCollection const* glow);

      template<typename InputIterator>
      void walkElements(InputIterator first, InputIterator last);

   private:
      std::list<int> m_path;
   };

   template<typename InputIterator>
   inline void Walker::walkElements(InputIterator first, InputIterator last)
   {
      for( ; first != last; first++)
      {
         auto glow = dynamic_cast<libember::glow::GlowContainer const*>(std::addressof(*first));

         if(glow != nullptr)
            walk(glow);
      }
   }
}

#endif//__TINYEMBERROUTER_GLOW_WALKER_H
