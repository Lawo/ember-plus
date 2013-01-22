#ifndef __TINYEMBER_GADGET_UTIL_ENTITYPATH_H
#define __TINYEMBER_GADGET_UTIL_ENTITYPATH_H

#include <vector>

namespace gadget
{
    class Node;
    class Parameter;
}

namespace gadget { namespace util
{
    class EntityPath
    {
        typedef std::vector<int> Vector;
        public:
            typedef Vector::value_type value_type;
            typedef Vector::size_type size_type ;
            typedef Vector::iterator iterator;
            typedef Vector::const_iterator const_iterator;

            template<typename InputIterator>
            EntityPath(InputIterator first, InputIterator last);

            EntityPath();

            const_iterator begin() const;

            const_iterator end() const;

            size_type size() const;

        private:
            Vector m_items;
    };

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

    std::size_t entity_depth(Node const* entity);

    std::size_t entity_depth(Parameter const* entity);

    EntityPath make_path(Node const* entity);

    EntityPath make_path(Parameter const* entity);

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
