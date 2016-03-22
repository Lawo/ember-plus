#include "Node.h"
#include "DynamicElementEmitter.h"

namespace model
{
   Element::Element(int number, Element* parent, std::string const& identifier)
      : m_number(number)
      , m_parent(parent)
      , m_identifier(identifier)
      , m_path(nullptr)
   {
      if(parent != nullptr)
         parent->insert(parent->end(), this);
   }

   Element::~Element()
   {
      for(auto child : *this)
         delete child;

      if(m_path != nullptr)
         delete m_path;
   }

   util::Oid Element::path() const
   {
      if(m_path == nullptr)
      {
         auto ids = std::vector<std::size_t>();

         for(auto elem = this; elem->isRoot() == false; elem = elem->parent())
            ids.insert(ids.end(), (std::size_t)elem->number());

         m_path = new util::Oid(ids.rbegin(), ids.rend());
      }

      return *m_path;
   }

   Element* Element::findDescendant(util::Oid const& path, bool& isDynamic) const
   {
      auto elem = (Element*)this;
      auto first = path.begin();
      auto last = path.end();

      isDynamic = false;

      for( ; first != last; first++)
      {
         auto number = *first;
         auto child = elem->findChild(number);

         if(child == nullptr)
         {
            auto dynamic = dynamic_cast<DynamicElementEmitter*>(elem);

            if(dynamic != nullptr)
            {
               auto emitted = dynamic->emitDescendant(path);
               isDynamic = emitted != nullptr;
               return emitted;
            }
            else
            {
               return nullptr;
            }
         }

         elem = child;
      }

      return elem;
   }

   Element* Element::findChild(int number) const
   {
      for(auto child : m_children)
      {
         if(child->number() == number)
            return child;
      }

      return nullptr;
   }

   // static
   Element* Element::createRoot()
   {
      return new Node(0, nullptr, std::string());
   }
}
