#include "Matrix.h"

namespace model { namespace matrix
{
   Matrix::Matrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : Element(number, parent, identifier)
      , m_notificationSink(notificationSink)
   {}

   Matrix::~Matrix()
   {
      for each(auto signal in m_targets)
         delete signal;

      for each(auto signal in m_sources)
         delete signal;
   }
}}
