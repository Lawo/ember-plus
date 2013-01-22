#ifndef __TINYEMBER_GADGET_ACCESS_H
#define __TINYEMBER_GADGET_ACCESS_H

namespace gadget
{
    struct Access
    {
        public:
            typedef enum _Domain
            {
                None = 0,
                ReadOnly,
                WriteOnly,
                ReadWrite,
            };

            typedef std::size_t value_type;

            Access(_Domain access)
                : m_value(access)
            {}

            value_type value() const
            {
                return m_value;
            }

        private:
            value_type const m_value;
    };
}

#endif//__TINYEMBER_GADGET_ACCESS_H
