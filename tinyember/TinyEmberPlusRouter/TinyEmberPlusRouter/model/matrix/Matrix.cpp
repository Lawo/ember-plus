#include "Matrix.h"

namespace model { namespace matrix
{
   Matrix::Matrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink)
      : Element(number, parent, identifier)
      , m_notificationSink(notificationSink)
   {}

   Matrix::~Matrix()
   {
      for(auto signal : m_targets)
         delete signal;

      for(auto signal : m_sources)
         delete signal;
   }
}}
