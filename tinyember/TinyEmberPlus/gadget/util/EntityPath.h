#ifndef __TINYEMBER_GADGET_UTIL_ENTITYPATH_H
#define __TINYEMBER_GADGET_UTIL_ENTITYPATH_H

#include <vector>

/** Forward declarations */
namespace gadget
{
    class Node;
    class Parameter;
}

namespace gadget { namespace util
{
    /**
     * Represents the numeric path of a node or a parameter.
     */
    class EntityPath
    {
        typedef std::vector<int> Vector;
        public:
            typedef Vector::value_type value_type;
            typedef Vector::size_type size_type ;
            typedef Vector::iterator iterator;
            typedef Vector::const_iterator const_iterator;

            /**
             * Initializes a new EntityPath.
             * @param first Points to the first element of an integer collection which contains the path.
             * @param last Points to the first element beyond the integer collection.
             */
            template<typename InputIterator>
            EntityPath(InputIterator first, InputIterator last);

            /**
             * Initializes an empty EntityPath
             */
            EntityPath();

            /**
             * Returns a reference to the first element of this path.
             * @return A reference to the first element of this path.
             */
            const_iterator begin() const;

            /**
             * Returns a reference to the first element beyond the buffer containing the entity path.
             * @return A reference to the first element beyond the buffer containing the entity path.
             */
            const_iterator end() const;

            /**
             * Returns the number of elements this path contains.
             * @return The number of elements this path contains.
             */
            size_type size() const;

        private:
            Vector m_items;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline EntityPath::EntityPath(InputIterator first, InputIterator last)
        : m_items(first, last)
    {}

    inline EntityPath::EntityPath()
    {}

    inline EntityPath::const_iterator EntityPath::begin() const
    {
        return m_items.begin();
    }

    inline EntityPath::const_iterator EntityPath::end() const
    {
        return m_items.end();
    }

    inline EntityPath::size_type EntityPath::size() const
    {
        return m_items.size();
    }


    /**
     * Determines the depth of the provided entity by looping
     * through all parents.
     * @param entity The entity to compute the depth for.
     * @return The depth of this entity.
     */
    std::size_t entity_depth(Node const* entity);

    /**
     * Determines the depth of the provided entity by looping
     * through all parents.
     * @param entity The entity to compute the depth for.
     * @return The depth of this entity.
     */
    std::size_t entity_depth(Parameter const* entity);

    /**
     * Creates a new path from the passed node.
     * @param entity The node to create the path for.
     * @return The node's path.
     */
    EntityPath make_path(Node const* entity);

    /**
     * Creates a new path from the passed parameter.
     * @param entity The parameter to create the path for.
     * @return The parameter's path.
     */
    EntityPath make_path(Parameter const* entity);

    /**
     * Searches for a child node which is identified by the provided pair of iterators which
     * contains the path of the node to look for.
     * @param root The root node to start the lookup.
     * @param first Reference to the first path element.
     * @param last Reference to the first element beyond the buffer storing the path.
     * @return The node that matches the specified path or nullptr, if the node could not be found.
     */
    // moved to .ipp
}
}

#endif//__TINYEMBER_GADGET_UTIL_ENTITYPATH_H
