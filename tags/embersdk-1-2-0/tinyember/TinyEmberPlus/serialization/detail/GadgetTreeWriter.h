#ifndef __TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H
#define __TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H

#include "../../gadget/Node.h"
#include <ember/Ember.hpp>

/** Forward declaration */
namespace serialization 
{
    class Archive;
}

namespace serialization { namespace detail
{
    /**
     * The gadget tree writer stores the local gadget tree as glow dom.
     */
    class GadgetTreeWriter 
    {
        friend class Archive;
        typedef libember::util::OctetStream OctetStream;
        public:
            typedef OctetStream::const_iterator const_iterator;
            typedef OctetStream::size_type size_type;

            /**
             * Returns the number of bytes stored in the current octet stream.
             * @return The number of bytes in the current output stream.
             */
            size_type size() const;

            /**
             * Returns an iterator that points to the first item in the output buffer.
             * @return An iterator that points to the first item in the output buffer.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator that points one past the last item of the output buffer.
             * @return An iterator that points one past the last item of the output buffer.
             */
            const_iterator end() const;

        private:
            /**
             * Initializes a new GadgetTreeWriter.
             * @param node The node to write to the output stream.
             */
            explicit GadgetTreeWriter(gadget::Node const* node);

            /** Destructor */
            ~GadgetTreeWriter();

            /**
             * Recursively writes a node and all of its parameters and child nodes.
             * @param node The node to encode.
             * @param parent The container where the encoded node shall be appended.
             */
            void write(gadget::Node const* node, libember::glow::GlowContainer* parent) const;

            /**
             * Encodes a single parameter.
             * @param param The parameter to encode.
             * @param parent The glow container where the parameter will be appended to.
             */
            void write(gadget::Parameter const* param, libember::glow::GlowContainer* parent) const;

        private:
            OctetStream m_stream;
    };
}
}

#endif//__TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H
