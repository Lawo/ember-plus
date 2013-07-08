#include "LinearMatrix.h"

namespace model { namespace matrix
{
   LinearMatrix::LinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount)
      : Matrix(number, parent, identifier, notificationSink)
   {
      auto& targets = this->targets();
      auto& sources = this->sources();

      for(int index = 0; index < targetCount; index++)
         targets.insert(targets.end(), new Signal(index));

      for(int index = 0; index < sourceCount; index++)
         sources.insert(sources.end(), new Signal(index));
   }

   int LinearMatrix::targetCount() const
   {
      return targets().size();
   }

   int LinearMatrix::sourceCount() const
   {
      return sources().size();
   }

   Signal* LinearMatrix::getTarget(int number) const
   {
      auto& targets = this->targets();

      if(number >= 0 && number < (int)targets.size())
         return targets[number];

      return nullptr;
   }

   Signal* LinearMatrix::getSource(int number) const
   {
      auto& sources = this->sources();

      if(number >= 0 && number < (int)sources.size())
         return sources[number];

      return nullptr;
   }
}}
