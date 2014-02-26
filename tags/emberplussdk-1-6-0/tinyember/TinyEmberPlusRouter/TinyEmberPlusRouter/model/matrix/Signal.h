#ifndef __TINYEMBERROUTER_MODEL_MATRIX_SIGNAL_H
#define __TINYEMBERROUTER_MODEL_MATRIX_SIGNAL_H

#include <vector>
#include "../../util/Collection.h"

namespace model { namespace matrix
{
   class Signal
   {
   public:
      typedef std::vector<Signal*> Vector;

   public:
      Signal(int number);

      // getters
      inline int number() const { return m_number; }
      inline Vector const& connectedSources() const { return m_connectedSources; }

      // methods
      template<typename InputIterator>
      void connect(InputIterator firstSource, InputIterator lastSource, bool isAbsolute);

      template<typename InputIterator>
      void disconnect(InputIterator firstSource, InputIterator lastSource);

   private:
      int m_number;
      Vector m_connectedSources;
   };

   template<typename InputIterator>
   inline void Signal::connect(InputIterator firstSource, InputIterator lastSource, bool isAbsolute)
   {
      if(isAbsolute)
      {
         m_connectedSources.clear();
         m_connectedSources.insert(m_connectedSources.end(), firstSource, lastSource);
      }
      else
      {
         for( ; firstSource != lastSource; firstSource++)
         {
            if(util::contains(m_connectedSources.begin(), m_connectedSources.end(), *firstSource) == false)
               m_connectedSources.insert(m_connectedSources.end(), *firstSource);
         }
      }
   }

   template<typename InputIterator>
   inline void Signal::disconnect(InputIterator firstSource, InputIterator lastSource)
   {
      for( ; firstSource != lastSource; firstSource++)
      {
         auto last = m_connectedSources.end();
         auto where = util::find(m_connectedSources.begin(), last, *firstSource);

         if(where != last)
            m_connectedSources.erase(where);
      }
   }
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_SIGNAL_H
