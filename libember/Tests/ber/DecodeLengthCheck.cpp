/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2024 Lawo AG (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ember/ber/Ber.hpp"

//SimianIgnore

namespace
{
    /**
     * Global definition of the length type used by this example.
     */
    typedef libember::ber::Length<unsigned long> LengthType;

    template<typename ValueType>
    void assertDecodeThrows(libember::util::OctetStream& input)
    {
        try
        {
            libember::ber::decode<ValueType>(input);
        }
        catch (std::exception const&)
        {
            return;
        }
        throw std::logic_error("Decode did not throw.");
    }

    template<typename ValueType>
    void assertDecodeThrows(libember::util::OctetStream& input, std::size_t length)
    {
        try
        {
            libember::ber::decode<ValueType>(input, length);
        }
        catch (std::exception const&)
        {
            return;
        }
        throw std::logic_error("Decode did not throw.");
    }
}

int main(int argc, char const* const* argv)
{
    if (argc != 2)
    {
        std::cerr << "Test name required" << std::endl;
        return 1;
    }

    std::string test_name = argv[1];

    try
    {
        libember::util::OctetStream testStream;

        if (test_name == "negative_zero")
        {
            // issue #120
            testStream.append(0x43);
            float decoded = libember::ber::decode<float>(testStream, testStream.size());
            if ((decoded != 0.0) || !libember::util::signbit(decoded))
            {
                throw std::runtime_error("Encountered unexpected value in input stream.");
            }
        }
        else if (test_name == "encoded_length")
        {
            assertDecodeThrows<float>(testStream, testStream.size() + 1);
        }
        else if (test_name == "exponent_length")
        {
            testStream.append(0x03);
            assertDecodeThrows<float>(testStream, testStream.size());
        }
        else if (test_name == "zero_mantissa")
        {
            testStream.append(0x00);
            testStream.append(0x00);
            float decoded = libember::ber::decode<float>(testStream, testStream.size());
            if (decoded != 1.0f)
            {
                throw std::runtime_error("Encountered unexpected value in input stream.");
            }
        }
        else if (test_name == "preamble_length_not_one")
        {
            // preamble 0x40 should be decoded as +Infinity, but due to length 2, is decoded as -1 instead (non-conforming to X.690)
            testStream.append(0x40);
            testStream.append(0x00);
            float decoded = libember::ber::decode<float>(testStream, testStream.size());
            if (decoded != -1.0f)
            {
                throw std::runtime_error("Encountered unexpected value in input stream.");
            }
        }
        else if (test_name == "integral")
        {
            assertDecodeThrows<unsigned long>(testStream, testStream.size() + 1);
        }
        else if (test_name == "boolean")
        {
            assertDecodeThrows<bool>(testStream, testStream.size() + 1);
        }
        else if (test_name == "length_first_byte")
        {
            assertDecodeThrows<LengthType>(testStream);
        }
        else if (test_name == "length_second_byte")
        {
            testStream.append(0x81U);
            assertDecodeThrows<LengthType>(testStream);
        }
        else if (test_name == "object_identifier_empty")
        {
            assertDecodeThrows<libember::ber::ObjectIdentifier>(testStream, testStream.size() + 1);
        }
        else if (test_name == "object_identifier_too_short")
        {
            testStream.append(0x80);
            assertDecodeThrows<libember::ber::ObjectIdentifier>(testStream, testStream.size());
        }
        else if (test_name == "tag")
        {
            assertDecodeThrows<libember::ber::Tag>(testStream);
        }
        else if (test_name == "tag_multibyte")
        {
            testStream.append(0x1F);
            assertDecodeThrows<libember::ber::Tag>(testStream);
        }
        else if (test_name == "tag_multibyte_too_short")
        {
            testStream.append(0x1F);
            testStream.append(0x80);
            assertDecodeThrows<libember::ber::Tag>(testStream);
        }
        else
        {
            std::cerr << "Invalid test name" << std::endl;
            return 1;
        }

        // Make sure the previously encoded byte stream was consumed in its entirety.
        if (!testStream.empty())
        {
            throw std::runtime_error("Stream not empty after all values have been decoded.");
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "ERROR: " << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}

//EndSimianIgnore
