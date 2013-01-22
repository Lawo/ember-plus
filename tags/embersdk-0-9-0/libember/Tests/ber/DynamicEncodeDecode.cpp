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
     * Factory function that generates type erased value wrappers containing
     * differnt values of different types depending on the value passed in
     * @p i.
     * @param i an unsigned integer used to (deterministically) vary the
     *      type and value wrapped within the result.
     * @return A type erased value wrapper in a regular state.
     */
    libember::ber::Value generateDynamicValue(unsigned int i)
    {
        switch (i % TYPE_COUNT)
        {
            case TYPE_INTEGER:
                return static_cast<int>(i);

            case TYPE_FLOAT:
                return static_cast<float>(i);

            case TYPE_BOOLEAN:
                return (((i / TYPE_COUNT) % 2) != 0);

            case TYPE_STRING:
                return std::string("Some test.");

            default:
                throw std::runtime_error("Encountered invalid type enumeration value.");
        }
    }

    /**
     * Factory function that generates different application tags depending
     * on the value passed in @p i.
     * @param i an unsigned integer used to (deterministically) vary the
     *      returned application tag.
     * @return An application tag.
     */
    libember::ber::Tag generateApplicationTag(unsigned int i)
    {
        return make_tag(libember::ber::Class::Application, i);
    }

    /**
     * Helper function that encodes the passed value as an explicitly tagged
     * frame.
     * @param output a reference to the octet stream to which the encoded
     *      frame should be written.
     * @param application the tag that should be used for the outer frame.
     *      This tag is automatically converted to a container tag before
     *      encoding.
     * @param value a type erased value wrapper containing the value to
     *      encode in the inner frame.
     */
    void encodeExplicitlyTaggedFrame(libember::util::OctetStream& output, libember::ber::Tag appTag, libember::ber::Value value)
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

    /**
     * Helper function used to decode and check a certain tag.
     * @param input a reference to the octet stream from which the tag should
     *      be decoded.
     * @param expectedTag the tag that is expected by the caller.
     * @throw std::runtime_error if the tag decoded from @p input does not
     *      compare equal to @p expectedTag.
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
     * Helper function used to decode and check a dynamically typed value.
     * @param input a reference to the octet stream from which the value should
     *      be decoded.
     * @param expectedValue the value that is expected by the caller.
     * @throw std::runtime_error if the value decoded from @p input does not
     *      compare equal to @p expectedValue.
     */
    void decodeAndAssertValue(libember::util::OctetStream& input, libember::ber::Value expectedValue)
    {
        libember::ber::Value const decodedValue = libember::ber::decode(input);
        if (decodedValue != expectedValue)
        {
            throw std::runtime_error("Encountered unexpected value in input stream.");
        }
    }

}

int main(int, char const* const*)
{
    try
    {
        using libember::ber::Tag;
        using libember::ber::Value;

        libember::util::OctetStream testStream;

        /*
         * Fill the testStream buffer with a series of BER encoded, explicitly tagged
         * value frames.
         */
        for (unsigned int i = 0; i < TEST_ITERATIONS; ++i)
        {
            Tag const appTag = generateApplicationTag(i);
            Value const value = generateDynamicValue(i); 
            encodeExplicitlyTaggedFrame(testStream, appTag, value);
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
            Tag const expectedTag     = generateApplicationTag(i).toContainer();
            Value const expectedValue = generateDynamicValue(i);

            decodeAndAssertTag(testStream, expectedTag);
            
            // Decode the outer frame length (and ignore it)
            libember::ber::decode<LengthType>(testStream);

            decodeAndAssertValue(testStream, expectedValue);
        }

        /*
         * Decode the previously encoded sequence of explicitly tagged value
         * frames.
         */
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

