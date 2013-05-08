#ifndef __TINYEMBERROUTER_MODEL_ELEMENT_H
#define __TINYEMBERROUTER_MODEL_ELEMENT_H

#include <string>
#include <vector>
#include "../util/Types.h"

namespace model
{
   class ElementVisitor;

   /**
     * Abstract base class for all types that make up the local DOM,
     * which includes Nodes, Parameters and Matrices.
     */
   class Element
   {
      friend class Lookup;

   // ========================================================
   // Iterator Typedefs
   // ========================================================
   public:
      typedef std::vector<Element*>  Vector;
      typedef Vector::iterator       iterator;
      typedef Vector::const_iterator const_iterator;
      typedef std::size_t            size_type;


   // ========================================================
   // Nested Types
   // ========================================================
   public:
      /**
        * Helper class for looking up Elements
        * and automatically deleting them if necessary.
        */
      class Lookup
      {
      public:
         /**
           * Looks up the @p root's descendant identified by @p path
           * and stores the result of the lookup.
           */
         Lookup(model::Element const* root, util::Oid const& path);

         /**
           * Deletes the result of the lookup if necessary.
           */
         ~Lookup();

         /**
           * Returns the result of the lookup.
           */
         inline model::Element* result() const { return m_result; }

      private:
         model::Element* m_result;
         bool m_isDynamic;
      };


   // ========================================================
   // Element Declaration
   // ========================================================
   public:
      /**
        * Creates a new instance of Element.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        */
      Element(int number, Element* parent, std::string const& identifier);

      /**
        * Destructor.
        */
      virtual ~Element();


   // ========================================================
   // Standard collection stuff:
   // An element is a collection of its child elements.
   // ========================================================
   public:
      inline bool empty() const { return m_children.empty(); }
      inline size_type size() const { return m_children.size(); }
      inline iterator begin() { return m_children.begin(); }
      inline const_iterator begin() const { return m_children.begin(); }
      inline iterator end() { return m_children.end(); }
      inline const_iterator end() const { return m_children.end(); }
      inline iterator insert(iterator where, Element* child) { return m_children.insert(where, child); }


   // ========================================================
   // Getters
   // ========================================================
   public:
      /**
        * Returns the Ember+ object number.
        * @return The Ember+ object number.
        */
      inline int number() const { return m_number; }

      /**
        * Returns a pointer to the parent element.
        * @return A pointer to the parent element.
        */
      inline Element* parent() const { return m_parent; }

      /**
        * Returns the Ember+ object identifier.
        * @return The Ember+ object identifier.
        */
      inline std::string identifier() const { return m_identifier; }

      /**
        * Returns the Ember+ object description.
        * @return The Ember+ object description.
        */
      inline std::string description() const { return m_description; }

      /**
        * Returns true if this element is the DOM tree root.
        * @return True if this element is the DOM tree root.
        */
      inline bool isRoot() const { return m_parent == nullptr; }


   // ========================================================
   // Setters
   // ========================================================
   public:
      /**
        * Sets the Ember+ object description.
        * @param value The Ember+ object description.
        */
      inline void setDescription(std::string const& value)
      {
         m_description = value;
      }


   // ========================================================
   // Methods
   // ========================================================
   public:
      /**
        * Looks for a child element with the passed number.
        * @param number The number of the child to find.
        * @return Either a pointer to the found child or nullptr.
        */
      Element* findChild(int number) const;

      /**
        * Creates a new instance of Element to act as DOM root.
        * @return A pointer to the new root Element. The caller
        *     must delete this object.
        */
      static Element* createRoot();

      /**
        * Returns the path to this element.
        * @Return the path to this element.
        */
      virtual util::Oid path() const;

      /**
        * Accepts a visitor of type ElementVisitor.
        */
      virtual void accept(ElementVisitor* visitor) = 0;

   private:
      /**
        * Walks down the passed @p path, finding the descendant the
        * path points to.
        * @param path The path to resolve.
        * @param isDynamic Receives the value true if the returned
        *     Element was created on-the-fly by an Element implementing
        *     the DynamicElementEmitter interface.
        *     In this case, the returned object must be deleted by the caller.
        */
      Element* findDescendant(util::Oid const& path, bool& isDynamic) const;

   private:
      int m_number;
      std::string m_identifier;
      std::string m_description;
      Element* m_parent;
      Vector m_children;
      mutable util::Oid* m_path;
   };


   // ========================================================
   //
   // Inline Implementation
   //
   // ========================================================

   inline Element::Lookup::Lookup(model::Element const* root, util::Oid const& path)
   {
      m_result = root->findDescendant(path, m_isDynamic);
   }

   inline Element::Lookup::~Lookup()
   {
      if(m_result != nullptr && m_isDynamic)
         delete m_result;
   }
}

#endif//__TINYEMBERROUTER_MODEL_ELEMENT_H
