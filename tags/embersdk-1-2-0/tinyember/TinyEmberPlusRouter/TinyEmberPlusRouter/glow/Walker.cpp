#include "Walker.h"

namespace glow
{
   void Walker::walk(libember::glow::GlowContainer const* glow)
   {
      using libember::glow::GlowType;

      switch(glow->typeTag().number())
      {
         case GlowType::Command:
            handleCommand(static_cast<libember::glow::GlowCommand const*>(glow), pathToOid());
            break;

         case GlowType::RootElementCollection:
            walkElements(glow->begin(), glow->end());
            break;

         case GlowType::Matrix:
            walkMatrix(static_cast<libember::glow::GlowMatrix const*>(glow));
            break;

         case GlowType::Node:
            walkNode(static_cast<libember::glow::GlowNode const*>(glow));
            break;

         case GlowType::Parameter:
            walkParameter(static_cast<libember::glow::GlowParameter const*>(glow));
            break;

         case GlowType::QualifiedMatrix:
            walkQualifiedMatrix(static_cast<libember::glow::GlowQualifiedMatrix const*>(glow));
            break;

         case GlowType::QualifiedNode:
            walkQualifiedNode(static_cast<libember::glow::GlowQualifiedNode const*>(glow));
            break;

         case GlowType::QualifiedParameter:
            walkQualifiedParameter(static_cast<libember::glow::GlowQualifiedParameter const*>(glow));
            break;

         case GlowType::StreamCollection:
            walkStreamCollection(static_cast<libember::glow::GlowStreamCollection const*>(glow));
            break;
      }
   }

   void Walker::handleNode(libember::glow::GlowNodeBase const* glow, libember::ber::ObjectIdentifier const& path)
   {}

   void Walker::handleStreamEntry(libember::glow::GlowStreamEntry const* glow)
   {}

   void Walker::walkNode(libember::glow::GlowNode const* glow)
   {
      m_path.push_back(glow->number());

      handleNode(glow, pathToOid());

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.pop_back();
   }

   void Walker::walkParameter(libember::glow::GlowParameter const* glow)
   {
      m_path.push_back(glow->number());

      handleParameter(glow, pathToOid());

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.pop_back();
   }

   void Walker::walkMatrix(libember::glow::GlowMatrix const* glow)
   {
      m_path.push_back(glow->number());

      handleMatrix(glow, pathToOid());

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.pop_back();
   }

   void Walker::walkQualifiedNode(libember::glow::GlowQualifiedNode const* glow)
   {
      auto glowPath = glow->path();
      m_path.clear();
      m_path.insert(m_path.end(), glowPath.begin(), glowPath.end());

      handleNode(glow, glowPath);

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.clear();
   }

   void Walker::walkQualifiedParameter(libember::glow::GlowQualifiedParameter const* glow)
   {
      auto glowPath = glow->path();
      m_path.clear();
      m_path.insert(m_path.end(), glowPath.begin(), glowPath.end());

      handleParameter(glow, glowPath);

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.clear();
   }

   void Walker::walkQualifiedMatrix(libember::glow::GlowQualifiedMatrix const* glow)
   {
      auto glowPath = glow->path();
      m_path.clear();
      m_path.insert(m_path.end(), glowPath.begin(), glowPath.end());

      handleMatrix(glow, glowPath);

      auto children = glow->children();

      if(children != nullptr)
         walkElements(children->begin(), children->end());

      m_path.clear();
   }

   void Walker::walkStreamCollection(libember::glow::GlowStreamCollection const* glow)
   {
      for each(libember::dom::Node const& ember in *glow)
      {
         auto streamEntry = dynamic_cast<libember::glow::GlowStreamEntry const*>(&ember);

         if(streamEntry != nullptr)
            handleStreamEntry(streamEntry);
      }
   }
}
