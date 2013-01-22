#ifndef __TINYEMBER_GADGET_STREAMDESCRIPTOR_H
#define __TINYEMBER_GADGET_STREAMDESCRIPTOR_H

#include "StreamFormat.h"

namespace gadget
{
    class StreamDescriptor
    {
        public:
            typedef std::size_t offset_type;

            StreamDescriptor(StreamFormat const& format, offset_type offset);

            offset_type offset() const;

            StreamFormat const& format() const;

        private:
            StreamFormat m_format;
            offset_type m_offset;
    };


    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline StreamDescriptor::StreamDescriptor(StreamFormat const& format, offset_type offset)
        : m_format(format)
        , m_offset(offset)
    {}

    inline StreamDescriptor::offset_type StreamDescriptor::offset() const
    {
        return m_offset;
    }

    inline StreamFormat const& StreamDescriptor::format() const
    {
        return m_format;
    }
}

#endif//__TINYEMBER_GADGET_STREAMDESCRIPTOR_H
