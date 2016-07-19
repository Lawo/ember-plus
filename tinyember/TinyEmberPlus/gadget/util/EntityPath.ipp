/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __TINYEMBER_GADGET_UTIL_ENTITYPATH_IPP
#define __TINYEMBER_GADGET_UTIL_ENTITYPATH_IPP
namespace gadget
{
    namespace util
    {
        /**
         * Searches for a child node which is identified by the provided pair of iterators which
         * contains the path of the node to look for.
         * @param root The root node to start the lookup.
         * @param first Reference to the first path element.
         * @param last Reference to the first element beyond the buffer storing the path.
         * @return The node that matches the specified path or nullptr, if the node could not be found.
         */
        template<typename InputIterator>
        Node const* resolve_node(Node const* root, InputIterator first, InputIterator last)
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
        Node* resolve_node(Node* root, InputIterator first, InputIterator last)
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
        Parameter const* resolve_parameter(Node const* root, InputIterator first, InputIterator last)
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
        Parameter* resolve_parameter(Node* root, InputIterator first, InputIterator last)
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
#endif //__TINYEMBER_GADGET_UTIL_ENTITYPATH_IPP
