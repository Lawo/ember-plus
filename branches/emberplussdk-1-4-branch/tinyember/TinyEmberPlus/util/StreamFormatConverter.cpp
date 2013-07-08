#include <algorithm>
#include "StreamFormatConverter.h"

using namespace gadget;

namespace util
{
    const std::pair<gadget::StreamFormat, std::string> StreamFormatConverter::s_entries[] = 
    {
        std::make_pair(StreamFormat::UnsignedInt8, "UInt8"),
        std::make_pair(StreamFormat::UnsignedInt16BigEndian, "UInt16 (BigEndian)"),
        std::make_pair(StreamFormat::UnsignedInt16LittleEndian, "UInt16 (LittleEndian)"),
        std::make_pair(StreamFormat::UnsignedInt32BigEndian, "UInt32 (BigEndian)"),
        std::make_pair(StreamFormat::UnsignedInt32LittleEndian, "UInt32 (LittleEndian)"),
        std::make_pair(StreamFormat::UnsignedInt64BigEndian, "UInt64 (BigEndian)"),
        std::make_pair(StreamFormat::UnsignedInt64LittleEndian, "UInt64 (LittleEndian)"),

        std::make_pair(StreamFormat::SignedInt8, "Int8"),
        std::make_pair(StreamFormat::SignedInt16BigEndian, "Int16 (BigEndian)"),
        std::make_pair(StreamFormat::SignedInt16LittleEndian, "Int16 (LittleEndian)"),
        std::make_pair(StreamFormat::SignedInt32BigEndian, "Int32 (BigEndian)"),
        std::make_pair(StreamFormat::SignedInt32LittleEndian, "Int32 (LittleEndian)"),
        std::make_pair(StreamFormat::SignedInt64BigEndian, "Int64 (BigEndian)"),
        std::make_pair(StreamFormat::SignedInt64LittleEndian, "Int64 (LittleEndian)"),

        std::make_pair(StreamFormat::IeeeFloat32BigEndian, "Float32 (BigEndian)"),
        std::make_pair(StreamFormat::IeeeFloat32LittleEndian, "Float32 (LittleEndian)"),
        std::make_pair(StreamFormat::IeeeFloat64BigEndian, "Float64 (BigEndian)"),
        std::make_pair(StreamFormat::IeeeFloat64LittleEndian, "Float64 (LittleEndian)"),
    };

    gadget::StreamFormat StreamFormatConverter::fromString(std::string const& text)
    {
        auto const first = s_entries;
        auto const last = s_entries + (sizeof(s_entries) / sizeof(s_entries[0]));
        auto const result = std::find_if(first, last, [&text](std::pair<StreamFormat, std::string> const& entry) -> bool
        {
            return entry.second == text;
        });

        if (result != last)
            return result->first;
        else
            return StreamFormat::Invalid;
    }

    std::string StreamFormatConverter::toString(gadget::StreamFormat const& format)
    {
        auto const first = s_entries;
        auto const last = s_entries + (sizeof(s_entries) / sizeof(s_entries[0]));
        auto const result = std::find_if(first, last, [&format](std::pair<StreamFormat, std::string> const& entry) -> bool
        {
            return entry.first.value() == format.value();
        });

        if (result != last)
            return result->second;
        else
            return "?";
    }

    StreamFormatConverter::const_iterator StreamFormatConverter::begin() const
    {
        return s_entries;
    }

    StreamFormatConverter::const_iterator StreamFormatConverter::end() const
    {
        return s_entries + size();
    }

    StreamFormatConverter::size_type StreamFormatConverter::size() const
    {
        return (sizeof(s_entries) / sizeof(s_entries[0]));
    }
}
