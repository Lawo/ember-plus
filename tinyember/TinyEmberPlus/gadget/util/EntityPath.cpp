#include "EntityPath.h"
#include "../../gadget/Parameter.h"
#include "../../gadget/Node.h"

namespace gadget { namespace util 
{
    std::size_t entity_depth(Node const* entity)
    {
        std::size_t depth = 0;
        while(entity != nullptr)
        {
            depth += 1;
            entity = entity->parent();
        }

        return depth;
    }

    std::size_t entity_depth(Parameter const* entity)
    {
        if (entity != nullptr)
        {
            return 1 + entity_depth(entity->parent());
        }
        else
        {
            return 0;
        }
    }

    EntityPath make_path(Node const* entity)
    {
        auto const depth = entity_depth(entity);
        auto container = std::vector<int>();
        
        container.reserve(depth);
        for(/** Nothing */; entity != nullptr; entity = entity->parent())
        {
            container.push_back(entity->number());
        }

        return EntityPath(container.rbegin(), container.rend());
    }

    EntityPath make_path(Parameter const* entity)
    {
        if (entity != nullptr)
        {
            auto const depth = entity_depth(entity);
            auto container = std::vector<int>();
            
            container.reserve(depth);
            container.push_back(entity->number());
            for(auto node = entity->parent(); node != nullptr; node = node->parent())
            {
                container.push_back(node->number());
            }

            return EntityPath(container.rbegin(), container.rend());
        }
        else
        {
            return EntityPath();
        }
    }
}
}
