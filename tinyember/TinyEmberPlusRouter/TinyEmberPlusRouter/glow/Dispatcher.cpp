#include "../model/model.h"
#include "Consumer.h"
#include "Encoder.h"
#include "Dispatcher.h"

namespace glow
{
   // ========================================================
   //
   // Dispatcher::Walker Definitions
   //
   // ========================================================

   Dispatcher::GlowWalker::GlowWalker(Dispatcher* dispatcher, Consumer* source)
      : m_dispatcher(dispatcher)
      , m_source(source)
   {}

   void Dispatcher::GlowWalker::handleCommand(libember::glow::GlowCommand const* glow, libember::ber::ObjectIdentifier const& path)
   {
      auto lookup = model::Element::Lookup(m_dispatcher->m_root, path);
      auto parent = lookup.result();

      if(parent != nullptr)
      {
         if(glow->number().value() == libember::glow::CommandType::GetDirectory)
         {
            auto glowRoot = libember::glow::GlowRootElementCollection::create();

            if(dynamic_cast<model::ParameterBase*>(parent) != nullptr
            || dynamic_cast<model::Function*>(parent) != nullptr)
            {
               auto glowElement = m_dispatcher->elementToGlow(parent, glow->dirFieldMask().value(), false);
               glowRoot->insert(glowRoot->end(), glowElement);
            }
            else if(dynamic_cast<model::matrix::Matrix*>(parent) != nullptr)
            {
               auto glowElement = m_dispatcher->elementToGlow(parent, glow->dirFieldMask().value(), true);
               glowRoot->insert(glowRoot->end(), glowElement);
            }
            else if(dynamic_cast<model::Node*>(parent) != nullptr)
            {
               if(parent->empty())
               {
                  glowRoot->insert(glowRoot->end(), new libember::glow::GlowQualifiedNode(parent->path())); // empty node
               }
               else
               {
                  for(auto child : *parent)
                  {
                     auto glowElement = m_dispatcher->elementToGlow(child, glow->dirFieldMask().value(), false);
                     glowRoot->insert(glowRoot->end(), glowElement);
                  }
               }
            }

            m_source->writeGlow(glowRoot);
            delete glowRoot;
         }
         else if(glow->number().value() == libember::glow::CommandType::Invoke)
         {
            auto const function = dynamic_cast<model::Function*>(parent);
            auto const invocation = glow->invocation();

            if(function != nullptr
            && invocation != nullptr)
            {
               auto invocationId = invocation->invocationId();
               auto argumentValues = util::VariantValueVector();
               invocation->typedArguments(std::back_inserter(argumentValues));

               auto result = util::VariantValueVector();
               auto success = function->invoke(argumentValues.begin(), argumentValues.end(), result);

               if(invocationId >= 0)
               {
                  auto invocationResult = libember::glow::GlowInvocationResult();
                  invocationResult.setInvocationId(invocationId);

                  if(success)
                     invocationResult.setTypedResult(result.begin(), result.end());
                  else
                     invocationResult.setSuccess(false);

                  m_source->writeGlow(&invocationResult);
               }
            }
         }
      }
   }

   void Dispatcher::GlowWalker::handleParameter(libember::glow::GlowParameterBase const* glow, libember::ber::ObjectIdentifier const& path)
   {
      auto lookup = model::Element::Lookup(m_dispatcher->m_root, path);
      auto parent = lookup.result();

      if(parent != nullptr)
      {
         if(glow->contains(libember::glow::ParameterProperty::Value))
         {
            auto glowValue = glow->value();

            switch(glowValue.type().value())
            {
               case libember::glow::ParameterType::Integer:
               {
                  auto integerParameter = dynamic_cast<model::IntegerParameter*>(parent);

                  if(integerParameter != nullptr)
                     integerParameter->setValue(glowValue.toInteger());

                  break;
               }

               case libember::glow::ParameterType::String:
               {
                  auto stringParameter = dynamic_cast<model::StringParameter*>(parent);

                  if(stringParameter != nullptr)
                     stringParameter->setValue(glowValue.toString());

                  break;
               }
            }
         }
      }
   }

   void Dispatcher::GlowWalker::handleMatrix(libember::glow::GlowMatrixBase const* glow, libember::ber::ObjectIdentifier const& path)
   {
      auto lookup = model::Element::Lookup(m_dispatcher->m_root, path);
      auto parent = lookup.result();

      if(parent != nullptr)
      {
         auto matrix = dynamic_cast<model::matrix::Matrix*>(parent);

         if(matrix != nullptr)
         {
            auto connections = glow->connections();

            if(connections != nullptr)
            {
               for(libember::dom::Node const& ember : *connections)
               {
                  auto connection = dynamic_cast<libember::glow::GlowConnection const*>(&ember);

                  if(connection != nullptr)
                  {
                     auto target = matrix->getTarget(connection->target());

                     if(target != nullptr)
                     {
                        auto glowSources = connection->sources();
                        auto sources = std::vector<model::matrix::Signal*>();

                        for(auto sourceNumber : glowSources)
                        {
                           auto source = matrix->getSource(sourceNumber);

                           if(source != nullptr)
                              sources.insert(sources.end(), source);
                        }

                        matrix->connect(target, sources.begin(), sources.end(), m_source, connection->operation());
                     }
                  }
               }
            }
         }
      }
   }


   // ========================================================
   //
   // Dispatcher::ElementToGlowConverter Definitions
   //
   // ========================================================

   Dispatcher::ElementToGlowConverter::ElementToGlowConverter(int dirFieldMask, bool isCompleteMatrixEnquired)
      : m_dirFieldMask(dirFieldMask == 0 ? libember::glow::DirFieldMask::All : dirFieldMask)
      , m_isCompleteMatrixEnquired(isCompleteMatrixEnquired)
      , m_result(nullptr)
   {
   }

   libember::glow::GlowElement* Dispatcher::ElementToGlowConverter::detachResult()
   {
      auto result = m_result;

      m_result = nullptr;

      return result;
   }

   void Dispatcher::ElementToGlowConverter::visit(model::Node* element)
   {
      auto glow = store(new libember::glow::GlowQualifiedNode(element->path()));

      if(hasDirField(libember::glow::DirFieldMask::Identifier))
         glow->setIdentifier(element->identifier());

      if(hasDirField(libember::glow::DirFieldMask::Description)
      && element->description().empty() == false)
         glow->setDescription(element->description());
   }

   void Dispatcher::ElementToGlowConverter::visit(model::IntegerParameter* element)
   {
      auto glow = store(new libember::glow::GlowQualifiedParameter(element->path()));

      if(hasDirField(libember::glow::DirFieldMask::Identifier))
         glow->setIdentifier(element->identifier());

      if(hasDirField(libember::glow::DirFieldMask::Description)
      && element->description().empty() == false)
         glow->setDescription(element->description());

      if(hasDirField(libember::glow::DirFieldMask::Value))
         glow->setValue(element->value());

      if(hasDirField(libember::glow::DirFieldMask::All))
      {
         glow->setMinimum(element->minimum());
         glow->setMaximum(element->maximum());
         
         if(element->isReadOnly() == false)
            glow->setAccess(libember::glow::Access::ReadWrite);
      }
   }

   void Dispatcher::ElementToGlowConverter::visit(model::StringParameter* element)
   {
      auto glow = store(new libember::glow::GlowQualifiedParameter(element->path()));

      if(hasDirField(libember::glow::DirFieldMask::Identifier))
         glow->setIdentifier(element->identifier());

      if(hasDirField(libember::glow::DirFieldMask::Description)
      && element->description().empty() == false)
         glow->setDescription(element->description());

      if(hasDirField(libember::glow::DirFieldMask::Value))
         glow->setValue(element->value());

      if(hasDirField(libember::glow::DirFieldMask::All))
      {
         if(element->isReadOnly() == false)
            glow->setAccess(libember::glow::Access::ReadWrite);
      }
   }

   void Dispatcher::ElementToGlowConverter::visit(model::Function* element)
   {
      auto glow = store(new libember::glow::GlowQualifiedFunction(element->path()));

      if(hasDirField(libember::glow::DirFieldMask::Identifier))
         glow->setIdentifier(element->identifier());

      if(hasDirField(libember::glow::DirFieldMask::Description)
      && element->description().empty() == false)
         glow->setDescription(element->description());

      if(hasDirField(libember::glow::DirFieldMask::All))
      {
         for(auto item : element->arguments())
         {
            auto arguments = glow->arguments();
            auto glowTupleItem = new libember::glow::GlowTupleItemDescription(item.type(), item.name());
            arguments->insert(arguments->end(), glowTupleItem);
         }

         for(auto item : element->result())
         {
            auto result = glow->result();
            auto glowTupleItem = new libember::glow::GlowTupleItemDescription(item.type(), item.name());
            result->insert(result->end(), glowTupleItem);
         }
      }
   }

   void Dispatcher::ElementToGlowConverter::visit(model::matrix::OneToNLinearMatrix* element)
   {
      auto glow = MatrixToGlow(element);
   }

   void Dispatcher::ElementToGlowConverter::visit(model::matrix::NToNLinearMatrix* element)
   {
      auto glow = MatrixToGlow(element);

      if(hasDirField(libember::glow::DirFieldMask::All))
         glow->setType(libember::glow::MatrixType::NToN);
   }

   void Dispatcher::ElementToGlowConverter::visit(model::matrix::NToNNonlinearMatrix* element)
   {
      auto glow = MatrixToGlow(element);

      if(hasDirField(libember::glow::DirFieldMask::All))
      {
         glow->setType(libember::glow::MatrixType::NToN);
         glow->setAddressingMode(libember::glow::MatrixAddressingMode::NonLinear);
         glow->setParametersLocation(element->parametersPath());

         if(element->gainParameterNumber() >= 0)
            glow->setGainParameterNumber(element->gainParameterNumber());

         if(m_isCompleteMatrixEnquired)
         {
            auto glowTargets = glow->targets();
            auto glowSources = glow->sources();

            for(auto signal : element->targets())
               glowTargets->insert(glowTargets->end(), new libember::glow::GlowTarget(signal->number()));

            for(auto signal : element->sources())
               glowSources->insert(glowSources->end(), new libember::glow::GlowSource(signal->number()));
         }
      }
   }

   void Dispatcher::ElementToGlowConverter::visit(model::matrix::DynamicNToNLinearMatrix* element)
   {
      auto glow = MatrixToGlow(element);

      if(hasDirField(libember::glow::DirFieldMask::All))
      {
         glow->setType(libember::glow::MatrixType::NToN);
         glow->setParametersLocation(element->parametersSubid());
         glow->setGainParameterNumber(element->gainParameterNumber());
      }
   }

   libember::glow::GlowMatrixBase* Dispatcher::ElementToGlowConverter::MatrixToGlow(model::matrix::Matrix const* element)
   {
      auto glow = store(new libember::glow::GlowQualifiedMatrix(element->path()));

      if(hasDirField(libember::glow::DirFieldMask::Identifier))
         glow->setIdentifier(element->identifier());

      glow->setTargetCount(element->targetCount());
      glow->setSourceCount(element->sourceCount());

      if(hasDirField(libember::glow::DirFieldMask::Description)
      && element->description().empty() == false)
         glow->setDescription(element->description());

      if(hasDirField(libember::glow::DirFieldMask::All)
      && element->labelsPath().empty() == false)
      {
         auto labels = glow->labels();
         auto label = new libember::glow::GlowLabel(element->labelsPath(), "Primary");
         labels->insert(labels->end(), label);
      }

      if(hasDirField(libember::glow::DirFieldMask::Connections)
      && m_isCompleteMatrixEnquired)
      {
         auto glowConnections = glow->connections();

         for(auto signal : element->targets())
         {
            auto glowConnection = new libember::glow::GlowConnection(signal->number());

            if(signal->connectedSources().empty() == false)
            {
               auto sourceNumbers = std::vector<int>();

               for(auto source : signal->connectedSources())
                  sourceNumbers.insert(sourceNumbers.end(), source->number());

               glowConnection->setSources(libember::ber::ObjectIdentifier(sourceNumbers.begin(), sourceNumbers.end()));
            }

            glowConnections->insert(glowConnections->end(), glowConnection);
         }
      }

      return glow;
   }


   // ========================================================
   //
   // Dispatcher Definitions
   //
   // ========================================================

   Dispatcher::Dispatcher(QObject* parent, int port)
      : m_server(parent, this, port)
   {}

   void Dispatcher::notifyMatrixConnection(model::matrix::Matrix* matrix, model::matrix::Signal* target, void* state)
   {
      auto glow = libember::glow::GlowRootElementCollection::create();
      auto glowMatrix = new libember::glow::GlowQualifiedMatrix(matrix->path());
      auto glowConnection = new libember::glow::GlowConnection(target->number());

      auto sourceNumbers = std::vector<int>();
      for(auto source : target->connectedSources())
         sourceNumbers.insert(sourceNumbers.end(), source->number());

      glowConnection->setSources(libember::ber::ObjectIdentifier(sourceNumbers.begin(), sourceNumbers.end()));
      glowConnection->setDisposition(libember::glow::ConnectionDisposition::Modified);

      auto glowConnections = glowMatrix->connections();
      glowConnections->insert(glowConnections->end(), glowConnection);
      glow->insert(glow->end(), glowMatrix);

      writeGlow(glow);
      delete glow;
   }

   void Dispatcher::notifyParameterValueChanged(util::Oid const& parameterPath, int value)
   {
      auto glow = libember::glow::GlowRootElementCollection::create();
      auto glowParam = new libember::glow::GlowQualifiedParameter(parameterPath);
      glowParam->setValue(value);

      glow->insert(glow->end(), glowParam);

      writeGlow(glow);
      delete glow;
   }

   void Dispatcher::notifyParameterValueChanged(util::Oid const& parameterPath, std::string const& value)
   {
      auto glow = libember::glow::GlowRootElementCollection::create();
      auto glowParam = new libember::glow::GlowQualifiedParameter(parameterPath);
      glowParam->setValue(value);

      glow->insert(glow->end(), glowParam);

      writeGlow(glow);
      delete glow;
   }

   net::TcpClient* Dispatcher::create(QTcpSocket* socket)
   {
      return new Consumer(socket, this);
   }

   void Dispatcher::receiveGlow(libember::glow::GlowContainer const* glow, Consumer* source)
   {
      auto walker = GlowWalker(this, source);

      walker.walk(glow);
   }

   libember::glow::GlowElement* Dispatcher::elementToGlow(model::Element* element, int dirFieldMask, bool isCompleteMatrixEnquired) const
   {
      auto converter = ElementToGlowConverter(dirFieldMask, isCompleteMatrixEnquired);
      element->accept(&converter);
      return converter.detachResult();
   }

   void Dispatcher::writeGlow(libember::glow::GlowContainer const* glow)
   {
      auto encoder = Encoder::createEmberMessage(glow);

      for(auto packet : encoder)
         m_server.write(packet.begin(), packet.end());
   }
}
