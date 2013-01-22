#ifndef STATETRAITS_H
#define STATETRAITS_H

#include "../states/StringState.h"
#include "StateTraits.h"

namespace gadgets { namespace serialization {
    template<typename StreamType>
    struct StateSerializationTraits<states::String, StreamType>
    {
        typedef states::StringState value_type;
        typedef value_type& reference_type;
        typedef StreamType stream_type;

        static void load(stream_type& stream, reference_type& state)
        {
            bool isCommand = false, isWriteable = false;
            int maxLength = 0;
            String value;

            stream >> isCommand >> isWriteable >> max >> value;
            state.setCommand(isCommand);
            state.setWriteable(isWriteable);
            state.setMaxLength(maxLength);
            state.setValue(value);
        }

        static void save(stream_type& stream, reference_type const& state)
        {
            stream << state.isCommand() << state.isWriteable() << state.maxLength() << state.value();
        }
    };
}
}

#endif // STATETRAITS_H
