#ifndef __TINYEMBER_GADGET_STREAMFORMAT_H
#define __TINYEMBER_GADGET_STREAMFORMAT_H

namespace gadget
{
    struct StreamFormat
    {
        public:
            typedef enum _Domain
            {
                UnsignedInt8                  = ( 0),
                UnsignedInt16BigEndian        = ( 2),
                UnsignedInt16LittleEndian     = ( 3),
                UnsignedInt32BigEndian        = ( 4),
                UnsignedInt32LittleEndian     = ( 5),
                UnsignedInt64BigEndian        = ( 6),
                UnsignedInt64LittleEndian     = ( 7),
                SignedInt8                    = ( 8),
                SignedInt16BigEndian          = (10),
                SignedInt16LittleEndian       = (11),
                SignedInt32BigEndian          = (12),
                SignedInt32LittleEndian       = (13),
                SignedInt64BigEndian          = (14),
                SignedInt64LittleEndian       = (15),
                IeeeFloat32BigEndian          = (20),
                IeeeFloat32LittleEndian       = (21),
                IeeeFloat64BigEndian          = (22),
                IeeeFloat64LittleEndian       = (23),

                Invalid = (0xFFFFFFFF),
            };

            typedef std::size_t value_type;

            StreamFormat(_Domain value)
                : m_value(value)
            {}

            value_type value() const
            {
                return m_value;
            }

            std::size_t size() const
            {
                auto const bits = ((m_value >> 1) & 3);
                auto const size = (1 << bits);
                return size;
            }

        private:
            value_type m_value;
    };
}

#endif//__TINYEMBER_GADGET_STREAMFORMAT_H
