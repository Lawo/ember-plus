#ifndef __TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H
#define __TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H

#include "../../gadget/Node.h"
#include <ember\Ember.hpp>

namespace serialization 
{
    class Archive;
}

namespace serialization { namespace detail
{
    class GadgetTreeWriter 
    {
        friend class Archive;
        typedef libember::util::OctetStream OctetStream;
        public:
            typedef OctetStream::const_iterator const_iterator;
            typedef OctetStream::size_type size_type;

            size_type size() const;

            const_iterator begin() const;

            const_iterator end() const;

        private:
            explicit GadgetTreeWriter(gadget::Node const* node);

            ~GadgetTreeWriter();

            void write(gadget::Node const* node, libember::glow::GlowContainer* parent) const;

            void write(gadget::Parameter const* param, libember::glow::GlowContainer* parent) const;

        private:
            OctetStream m_stream;
    };
}
}

#endif//__TINYEMBER_SERIALIZATION_GADGETTREEWRITER_H
