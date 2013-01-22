#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ember/util/StreamBuffer.hpp>

#define THROW_TEST_EXCEPTION(message)                       \
            {                                               \
                std::ostringstream msgStream;               \
                msgStream << message ;                      \
                throw std::runtime_error(msgStream.str());  \
            }
    
template class libember::util::StreamBuffer<unsigned short>;

int main(int, char const* const*)
{
    try
    {
        using libember::util::StreamBuffer;

        StreamBuffer<unsigned short> testStream;
        for (unsigned int i = 0; i < 1000; ++i)
        {
            {
                unsigned short writeBuffer[7];
                for (unsigned int j = 0; j < 7; ++j)
                {
                    writeBuffer[j] = static_cast<unsigned short>(i * 7 + j);
                }
                unsigned short const* const begin = writeBuffer;
                unsigned short const* const end   = writeBuffer + sizeof(writeBuffer) / sizeof(writeBuffer[0]);
                testStream.append(begin, end);
            }
            for (unsigned int j = 0; j < 5; ++j)
            {
                unsigned short const current = testStream.front();
                testStream.consume();
                if (current != (i * 5 + j))
                {
                    THROW_TEST_EXCEPTION("Invalid head of buffer! Expected " << (i * 5 + j) << ", found " << current);
                }
            }
            std::size_t const size = testStream.size();
            if (size != 2 * (i + 1))
            {
                THROW_TEST_EXCEPTION("Invalid size of buffer! Expected " << ((i + 1) * 2) << ", found " << size);
            }
        }
        for (unsigned int i = 0; i < 1000; ++i)
        {
            for (unsigned int j = 0; j < 2; ++j)
            {
                unsigned short const current = testStream.front();
                testStream.consume();
                if (current != (5000 + i * 2 + j))
                {
                    THROW_TEST_EXCEPTION("Invalid head of buffer! Expected " << ((i + 1000) * 5 + j) << ", found " << current);
                }
            }
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

