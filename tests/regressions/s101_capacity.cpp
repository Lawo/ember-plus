#include <iostream>
#include <vector>
#include "s101/StreamEncoder.hpp"
#include "s101/StreamEncoderWithoutEscaping.hpp"
#include "s101/StreamDecoder.hpp"

struct CollectDecoded {
    explicit CollectDecoded(std::vector<unsigned char>& output) : output_(output) {}
    void operator()(libs101::StreamDecoder<>::const_iterator first,
                    libs101::StreamDecoder<>::const_iterator last) const {
        output_.assign(first, last);
    }
    std::vector<unsigned char>& output_;
};

static bool equal(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b)
{
    return a == b;
}

template <typename Encoder>
static std::vector<unsigned char> bytes(const Encoder& encoder)
{
    return std::vector<unsigned char>(encoder.begin(), encoder.end());
}

static bool escaped(std::size_t capacity)
{
    const unsigned char payload[] = {0x01, 0xFE, 0x02};
    libs101::StreamEncoder<> encoder(capacity);
    if (encoder.size() != 0) return false;
    encoder.encode(payload, payload + 3);
    encoder.finish();
    const std::vector<unsigned char> frame = bytes(encoder);
    std::vector<unsigned char> decoded;
    libs101::StreamDecoder<> decoder;
    decoder.read(frame.begin(), frame.end(), CollectDecoded(decoded));
    return !frame.empty() && frame[0] == libs101::Byte::BoF && equal(decoded, std::vector<unsigned char>(payload, payload + 3));
}

static bool unescaped(std::size_t capacity)
{
    const unsigned char payload[] = {0x01, 0xFE, 0x02};
    libs101::StreamEncoderWithoutEscaping<> encoder(capacity);
    if (encoder.size() != 0) return false;
    encoder.encode(payload, payload + 3);
    encoder.finish();
    const std::vector<unsigned char> frame = bytes(encoder);
    std::vector<unsigned char> decoded;
    libs101::StreamDecoder<> decoder;
    decoder.read(frame.begin(), frame.end(), CollectDecoded(decoded));
    return frame.size() == 9 && frame[0] == libs101::Byte::Invalid && frame[1] == 0x04 &&
        frame[2] == 0 && frame[3] == 0 && frame[4] == 0 && frame[5] == 3 &&
        equal(decoded, std::vector<unsigned char>(payload, payload + 3));
}

int main()
{
    const std::size_t capacities[] = {0, 1, 8, 64};
    std::size_t failures = 0;
    for (std::size_t i = 0; i != sizeof(capacities) / sizeof(capacities[0]); ++i) {
        const std::size_t capacity = capacities[i];
        if (!escaped(capacity)) { std::cerr << "escaped encoder/decoder failed capacity=" << capacity << "\n"; ++failures; }
        if (!unescaped(capacity)) { std::cerr << "unescaped encoder/decoder failed capacity=" << capacity << "\n"; ++failures; }
    }
    if (failures != 0) return 1;
    std::cout << "PASS: both public S101 encoder variants; capacities 0,1,8,64; frame/header/payload and decoder roundtrip\n";
    return 0;
}
