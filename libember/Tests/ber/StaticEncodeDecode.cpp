#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ember/ber/Ber.hpp"

namespace
{
    /**
     * Global definition of the length type used by this example.
     */
    typedef libember::ber::Length<unsigned long> LengthType;

    /**
     * An enumeration type used to assign symbolic names to the test cases.
     */
    enum
    {
        TYPE_INTEGER = 0,
        TYPE_FLOAT,
        TYPE_BOOLEAN,
        TYPE_STRING,
        
        TYPE_COUNT
    };

    /**
     * The number of iterations to perform in this test.
     */
    unsigned int const TEST_ITERATIONS = 1000000;

    /**
     * Helper function used to decode and check a certain tag.
     * @param input a reference to the octet stream from which the tag should be
     *      decoded.
     * @param expectedTag the tag that is expected by the caller.
     * @throw std::runtime_error if the tag decoded from @p input does not compare
     *      equal to @p expectedTag.
     */
    void decodeAndAssertTag(libember::util::OctetStream& input, libember::ber::Tag expectedTag)
    {
        using libember::ber::decode;
        using libember::ber::Tag;

        Tag const decodedTag = decode<Tag>(input);
        if (decodedTag != expectedTag)
        {
            throw std::runtime_error("Encountered unexpected tag in input stream.");
        }
    }

    /**
     * Helper function used to decode and check a statically typed value.
     * @param input a reference to the octet stream from which the value should be
     *      decoded.
     * @param expectedValue the value that is expected by the caller.
     * @throw std::runtime_error if the tag decoded from @p input does not compare
     *      equal to @p expectedValue.
     */
    template<typename ValueType>
    void decodeAndAssertValue(libember::util::OctetStream& input, ValueType expectedValue)
    {
        using libember::ber::decode;

        // Decode the universal tag identifying the type of the encoded value
        decodeAndAssertTag(input, libember::ber::universalTag<ValueType>());
        // Decode the length of the type specific encoded value
        LengthType const innerLength = decode<LengthType>(input);
        // Decode the actual typed value
        ValueType const value = decode<ValueType>(input, innerLength.value);
        // And assert, whether it matches the expected result
        if (value != expectedValue)
        {
            throw std::runtime_error("Encountered unexpected value in input stream.");
        }
    }

    template<typename ValueType>
    void encodeExplicitlyTaggedFrame(libember::util::OctetStream& output, libember::ber::Tag appTag, ValueType value)
    {
        using libember::ber::encode;
        using libember::ber::encodeFrame;
        using libember::ber::encodedFrameLength;
        using libember::ber::make_length;

        std::size_t const innerFrameLength = encodedFrameLength(value);

        encode(output, appTag.toContainer());
        encode(output, make_length(innerFrameLength));
        encodeFrame(output, value);
    }

}

int main(int, char const* const*)
{
    try
    {
        using libember::ber::Tag;
        using libember::ber::make_tag;

        libember::util::OctetStream testStream;

        /*
         * Fill the testStream buffer with a series of BER encoded, explicitly tagged
         * value frames.
         */
        for (unsigned int i = 0; i < TEST_ITERATIONS; ++i)
        {
            Tag const appTag = make_tag(libember::ber::Class::Application, i);
            switch (i % TYPE_COUNT)
            {
                case TYPE_INTEGER:
                    // Encode as int
                    encodeExplicitlyTaggedFrame(testStream, appTag, static_cast<int>(i));
                    break;

                case TYPE_FLOAT:
                    // Encode as float
                    encodeExplicitlyTaggedFrame(testStream, appTag, static_cast<float>(i));
                    break;

                case TYPE_BOOLEAN:
                    // Encode as boolean
                    encodeExplicitlyTaggedFrame(testStream, appTag, (((i / TYPE_COUNT) % 2) != 0));
                    break;

                case TYPE_STRING:
                    // Encode as string
                    {
                        std::ostringstream valueStream;
                        valueStream << "String value #" << i;
                        encodeExplicitlyTaggedFrame(testStream, appTag, valueStream.str());
                    }
                    break;

                default:
                    break;
            }
        }

        std::cout
            << "After encoding " << TEST_ITERATIONS << " explicitly tagged values the size of the stream buffer is " << testStream.size() << " bytes."
            << std::endl;

        /*
         * Decode the previously encoded sequence of explicitly tagged value
         * frames.
         */
        for (unsigned int i = 0; i < TEST_ITERATIONS; ++i)
        {
            Tag const expectedTag = make_tag(libember::ber::Class::Application, i).toContainer();
            // Decode the outer application defined tag
            decodeAndAssertTag(testStream, expectedTag);
            // Decode the outer frame length (and ignore it)
            libember::ber::decode<LengthType>(testStream);
            switch (i % TYPE_COUNT)
            {
                case TYPE_INTEGER:
                    // Decode int and assert the decoded value
                    decodeAndAssertValue(testStream, static_cast<int>(i));
                    break;

                case TYPE_FLOAT:
                    // Decode float and assert the decoded value
                    decodeAndAssertValue(testStream, static_cast<float>(i));
                    break;

                case TYPE_BOOLEAN:
                    // Decode boolean and assert the decoded value
                    decodeAndAssertValue(testStream, (((i / TYPE_COUNT) % 2) != 0));
                    break;

                case TYPE_STRING:
                    // Decode string and assert the decoded value
                    {
                        std::ostringstream valueStream;
                        valueStream << "String value #" << i;
                        decodeAndAssertValue(testStream, valueStream.str());
                    }
                    break;

                default:
                    break;
            }
        }
        // Make the previously encoded byte stream was consumed in its entirety.
        if (!testStream.empty())
        {
            throw std::runtime_error("Stream not empty after all values have been encoded.");
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "ERROR: " << "An unknown error occured." << std::endl;
        return 1;
    }
    return 0;
}

