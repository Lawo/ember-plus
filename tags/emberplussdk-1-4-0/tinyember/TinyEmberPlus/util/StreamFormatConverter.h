#ifndef __TINYEMBER_UTIL_STREAMFORMATCONVERTER_H
#define __TINYEMBER_UTIL_STREAMFORMATCONVERTER_H

#include <string>
#include "../gadget/StreamFormat.h"

namespace util
{
    /**
     * Provides methods to get the human readable string for a stream format.
     */
    class StreamFormatConverter
    {
        public:
            typedef std::size_t size_type;
            typedef std::pair<gadget::StreamFormat, std::string> value_type;
            typedef value_type const* const_iterator;

            /**
             * Returns an iterator that points to the first pair of a stream format and its display name.
             * @return An iterator that points to the first pair of a stream format and its display name.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator that points to the first element beyond the buffer.
             * @return An iterator that points to the first element beyond the buffer.
             */
            const_iterator end() const;

            /**
             * Returns the number of pairs available.
             * @return The number of pairs available.
             */
            size_type size() const;

            /**
             * Searches for a stream format that is associated with the specified display string.
             * @param text The display string of the stream format to lookup.
             * @return The associated stream format.
             */
            static gadget::StreamFormat fromString(std::string const& text);

            /**
             * Returns the display string of the passed stream format.
             * @param format The format to get the display string for.
             * @return The display string of the passed stream format.
             */
            static std::string toString(gadget::StreamFormat const& format);

        private:
            static const value_type s_entries[];
    };
}

#endif//__TINYEMBER_UTIL_STREAMFORMATCONVERTER_H
