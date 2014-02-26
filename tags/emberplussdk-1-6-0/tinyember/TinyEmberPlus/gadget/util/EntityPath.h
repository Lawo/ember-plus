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
    template<typename InputIterator>
    inline Node const* resolve_node(Node const* root, InputIterator first, InputIterator last)
    {
        auto const size = std::distance(first, last);
        if (root->number() == *first)
        {
            for( ; first != last; ++first)
            {
                auto const& nodes = root->nodes();
                auto result = std::find_if(std::begin(nodes), std::end(nodes), [first](Node const* node) -> bool
                {
                    return node->number() == *first;
                });

                if (result != std::end(nodes))
                {
                    return *result;
                }
            }
        }

        return nullptr;
    }

    /**
     * Searches for a child node which is identified by the provided pair of iterators which
     * contains the path of the node to look for.
     * @param root The root node to start the lookup.
     * @param first Reference to the first path element.
     * @param last Reference to the first element beyond the buffer storing the path.
     * @return The node that matches the specified path or nullptr, if the node could not be found.
     */
    template<typename InputIterator>
    inline Node* resolve_node(Node* root, InputIterator first, InputIterator last)
    {
        auto const size = std::distance(first, last);
        auto node = static_cast<Node*>(root);
        if (size == 1)
        {
            if (root->number() == *first)
                return root;
        }
        if (size > 1 && root->number() == *first)
        {
            first++;
            for(; first != last; ++first)
            {
                auto& nodes = node->nodes();
                auto result = std::find_if(std::begin(nodes), std::end(nodes), [first](Node* node) -> bool
                {
                    return node->number() == *first;
                });

                if (result != std::end(nodes))
                {
                    node = *result;
                }
                else
                {
                    return nullptr;
                }
            }
        }

        return node;
    }

    /**
     * Searches for a parameter which is identified by the provided pair of iterators which
     * contains the path of the parameter to look for.
     * @param root The root node to start the lookup.
     * @param first Reference to the first path element.
     * @param last Reference to the first element beyond the buffer storing the path.
     * @return The parameter that matches the specified path or nullptr, if the parameter could not be found.
     */
    template<typename InputIterator>
    inline Parameter const* resolve_parameter(Node const* root, InputIterator first, InputIterator last)
    {
        auto const size = std::distance(first, last);
        if (size > 0)
        {
            --last;
            auto const node = resolve_node(root, first, last);
            if (node != nullptr)
            {
                auto it = last;
                auto const& parameters = node->parameters();
                auto result = std::find_if(std::begin(parameters), std::end(parameters), [it](Parameter const* p) -> bool
                {
                    return p->number() == *it;
                });

                return result != std::end(parameters) ? *result : nullptr;
            }
        }

        return nullptr;
    }

    /**
     * Searches for a parameter which is identified by the provided pair of iterators which
     * contains the path of the parameter to look for.
     * @param root The root node to start the lookup.
     * @param first Reference to the first path element.
     * @param last Reference to the first element beyond the buffer storing the path.
     * @return The parameter that matches the specified path or nullptr, if the parameter could not be found.
     */
    template<typename InputIterator>
    inline Parameter* resolve_parameter(Node* root, InputIterator first, InputIterator last)
    {
        auto const size = std::distance(first, last);
        if (size > 0)
        {
            --last;
            auto node = resolve_node(root, first, last);
            if (node != nullptr)
            {
                auto& parameters = node->parameters();
                auto it = last;
                auto result = std::find_if(std::begin(parameters), std::end(parameters), [it](Parameter* p) -> bool
                {
                    return p->number() == *it;
                });

                return result != std::end(parameters) ? *result : nullptr;
            }
        }

        return nullptr;
    }
}
}

#endif//__TINYEMBER_GADGET_UTIL_ENTITYPATH_H
