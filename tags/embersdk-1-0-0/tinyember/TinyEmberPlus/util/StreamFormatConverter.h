#ifndef __TINYEMBER_UTIL_STREAMFORMATCONVERTER_H
#define __TINYEMBER_UTIL_STREAMFORMATCONVERTER_H

#include <string>
#include "../gadget/StreamFormat.h"

namespace util
{
    class StreamFormatConverter
    {
        public:
            typedef std::size_t size_type;
            typedef std::pair<gadget::StreamFormat, std::string> value_type;
            typedef value_type const* const_iterator;

            const_iterator begin() const;

            const_iterator end() const;

            size_type size() const;

            static gadget::StreamFormat fromString(std::string const& text);

            static std::string toString(gadget::StreamFormat const& format);

        private:
            static const value_type s_entries[];
    };
}

#endif//__TINYEMBER_UTIL_STREAMFORMATCONVERTER_H
