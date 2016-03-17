#ifndef __TINYEMBER_UTIL_VALIDATION_H
#define __TINYEMBER_UTIL_VALIDATION_H

#include "../Types.h"
#include "../gadget/Node.h"
#include "../gadget/Parameter.h"

namespace util
{
    /**
     * This class provides some validation methods.
     */
    class Validation
    {
        public:
            /**
             * Evaluates if the provided identifier is unique and doesn't already exist in the provided
             * container.
             * @note Please note that the value type of the input iterator must be of type gadget::Node
             *      or gadget::Parameter.
             * @param first An iterator that points to the first node or parameter within the collection.
             * @param last An iterator that points one past the last node or parameter withing the collection.
             * @return true if the identifier is unique, false if it is already in use.
             */
            template<typename InputIterator>
            static bool isUniqueIdentifier(InputIterator first, InputIterator last, String const& identifier)
            {
                auto const result = std::find_if(first, last, [&identifier](decltype(*first) const& it) -> bool
                {
                    return it->identifier() == identifier;
                });
                return result == last;
            }

            /**
             * Tests whether the passed identifier is already used as identifier for a child node or parameter
             * of the provided parent node.
             * @param parent The node whos child elements will be traversed.
             * @param identifier The identifier to look for.
             * @return true if the identifier is unique, false if it is already in use.
             */
            static bool isUniqueIdentifier(gadget::Node const* parent, String const& identifier)
            {
                if (parent != nullptr)
                {
                    auto const nodes = parent->nodes();
                    auto const parameters = parent->parameters();
                    return isUniqueIdentifier(nodes.begin(), nodes.end(), identifier)
                        && isUniqueIdentifier(parameters.begin(), parameters.end(), identifier);
                }
                else
                {
                    return true;
                }
            }
    };
}

#endif//__TINYEMBER_UTIL_VALIDATION_H
