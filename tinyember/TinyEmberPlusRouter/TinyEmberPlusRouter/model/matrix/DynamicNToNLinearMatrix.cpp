#include "../ElementVisitor.h"
#include "DynamicNToNLinearMatrix.h"
#include "detail/Connect.h"

namespace model { namespace matrix
{
   // ========================================================
   //
   // DynamicNToNLinearMatrix::DynamicNode
   // Definitions
   //
   // ========================================================

   DynamicNToNLinearMatrix::DynamicNode::DynamicNode(util::Oid const& path)
      : Node(*(path.end() - 1), nullptr, "")
      , m_path(path)
   {}


   // ========================================================
   //
   // DynamicNToNLinearMatrix::DynamicIntegerParameter
   // Definitions
   //
   // ========================================================

   DynamicNToNLinearMatrix::DynamicIntegerParameter::DynamicIntegerParameter(util::Oid const& path,
                                                                             Element* parent,
                                                                             std::string identifier,
                                                                             DynamicNToNLinearMatrix* owner,
                                                                             int value,
                                                                             int minimum,
                                                                             int maximum)
      : IntegerParameter(*(path.end() - 1), parent, identifier, owner->notificationSink(), minimum, maximum)
      , m_owner(owner)
      , m_path(path)
   {
      setValueSilent(value);
   }

   void DynamicNToNLinearMatrix::DynamicIntegerParameter::onValueChanged()
   {
      if(m_owner->onParameterValueChanged(m_path, value()))
         IntegerParameter::onValueChanged();
   }


   // ========================================================
   //
   // DynamicNToNLinearMatrix Definitions
   //
   // ========================================================

   DynamicNToNLinearMatrix::DynamicNToNLinearMatrix(int number,
                                                    Element* parent,
                                                    std::string const& identifier,
                                                    NotificationSink* notificationSink,
                                                    int targetCount, int sourceCount)
      : LinearMatrix(number, parent, identifier, notificationSink, targetCount, sourceCount)
   {
      auto xpointCount = targetCount * sourceCount;

      m_xpointGains = new int[xpointCount];

      for(auto index = 0; index < xpointCount; index++)
         m_xpointGains[index] = minimumGain();
   }

   DynamicNToNLinearMatrix::~DynamicNToNLinearMatrix()
   {
      if(m_xpointGains != nullptr)
         delete [] m_xpointGains;
   }

   void DynamicNToNLinearMatrix::accept(ElementVisitor* visitor)
   {
      visitor->visit(this);
   }

   bool DynamicNToNLinearMatrix::connectOverride(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation)
   {
      return detail::connectNToN(target, sources, state, operation);
   }

   bool DynamicNToNLinearMatrix::onParameterValueChanged(util::Oid const& path, int value)
   {
      auto offset = this->path().size();

      if(path.size() == offset + 5
      && path[offset + 0] == parametersSubid()
      && path[offset + 1] == 3 // connections
      && path[offset + 4] == gainParameterNumber())
      {
         auto targetNumber = path[offset + 2];
         auto sourceNumber = path[offset + 3];
         auto index = targetNumber * sourceCount() + sourceNumber;

         if(index >= 0 && index < (unsigned int)(targetCount() * sourceCount()))
         {
            m_xpointGains[index] = value;
            return true;
         }
      }

      return false;
   }

   Element* DynamicNToNLinearMatrix::emitDescendant(util::Oid const& path)
   {
      auto offset = this->path().size();

      if(path.size() >= offset + 4
      && path[offset + 0] == parametersSubid()
      && path[offset + 1] == 3) // connections)
      {
         auto targetNumber = path[offset + 2];
         auto sourceNumber = path[offset + 3];
         auto index = targetNumber * sourceCount() + sourceNumber;

         if(index >= 0 && index < (unsigned int)(targetCount() * sourceCount()))
         {
            if(path.size() == offset + 4) // return node "parameters/<targetNumber>/<sourceNumber>"
            {
               auto node = new DynamicNode(path);
               auto gainPath = std::vector<int>(path.begin(), path.end());
               gainPath.insert(gainPath.end(), gainParameterNumber());

               auto gain = new DynamicIntegerParameter(util::Oid(gainPath.begin(), gainPath.end()),
                                                       node,
                                                       "gain",
                                                       this,
                                                       m_xpointGains[index],
                                                       minimumGain(),
                                                       maximumGain());

               return node;
            }
            else // return parameter "parameters/<targetNumber>/<sourceNumber>/<gainParameterNumber>"
            {
               if(path[offset + 4] == gainParameterNumber())
                  return new DynamicIntegerParameter(path, nullptr, "gain", this, m_xpointGains[index], minimumGain(), maximumGain());
            }
         }
      }

      return nullptr;
   }
}}
