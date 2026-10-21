#define LIBEMBER_HEADER_ONLY
#include <iostream>
#include <string>
#include "ember/glow/GlowTemplate.hpp"
#include "ember/glow/GlowConnection.hpp"
#include "ember/glow/GlowCommand.hpp"

static std::size_t occurrences(const libember::glow::GlowContainer& object, const libember::ber::Tag& tag)
{
    std::size_t count = 0;
    for (libember::glow::GlowContainer::const_iterator it = object.begin(); it != object.end(); ++it)
        if (it->applicationTag() == tag) ++count;
    return count;
}

int main()
{
    using namespace libember::glow;
    GlowTemplate element(23);
    element.setDescription("first");
    element.setDescription("latest");
    std::size_t failures = 0;
    if (element.description() != "latest" || occurrences(element, GlowTags::Template::Description()) != 1 || element.number() != 23) {
        std::cerr << "GlowTemplate repeated description setter failed\n"; ++failures;
    }

    GlowConnection connection(41);
    connection.setOperation(ConnectionOperation::Absolute);
    connection.setOperation(ConnectionOperation::Connect);
    if (connection.operation().value() != ConnectionOperation::Connect || occurrences(connection, GlowTags::Connection::Operation()) != 1 || connection.target() != 41) {
        std::cerr << "GlowConnection repeated operation setter failed\n"; ++failures;
    }
    GlowCommand command(CommandType::GetDirectory);
    command.setDirFieldMask(DirFieldMask::Identifier);
    command.setDirFieldMask(DirFieldMask::Description);
    if (command.dirFieldMask().value() != DirFieldMask::Description ||
        occurrences(command, GlowTags::Command::DirFieldMask()) != 1 ||
        command.number().value() != CommandType::GetDirectory) {
        std::cerr << "GlowCommand repeated directory field mask setter failed\n"; ++failures;
    }
    libember::ber::ObjectIdentifier firstSources(3);
    libember::ber::ObjectIdentifier latestSources(7);
    connection.setSources(firstSources);
    connection.setSources(latestSources);
    if (connection.sources().size() != 1 || connection.sources().front() != 7 ||
        occurrences(connection, GlowTags::Connection::Sources()) != 1 || connection.target() != 41) {
        std::cerr << "GlowConnection repeated sources setter failed\n"; ++failures;
    }
    connection.setDisposition(ConnectionDisposition::Tally);
    connection.setDisposition(ConnectionDisposition::Pending);
    if (connection.disposition().value() != ConnectionDisposition::Pending ||
        occurrences(connection, GlowTags::Connection::Disposition()) != 1 || connection.target() != 41) {
        std::cerr << "GlowConnection repeated disposition setter failed\n"; ++failures;
    }
    GlowNode* firstNode = element.makeNode(11);
    GlowNode* latestNode = element.makeNode(12);
    element.setElement(firstNode);
    element.setElement(latestNode);
    if (element.element() != latestNode || occurrences(element, element.elementTag()) != 1) {
        std::cerr << "GlowTemplate repeated element setter failed\n"; ++failures;
    } else {
        element.setElement(latestNode);
        if (element.element() != latestNode || occurrences(element, element.elementTag()) != 1 || element.description() != "latest") {
            std::cerr << "GlowTemplate same-pointer setter or sibling preservation failed\n"; ++failures;
        }
    }
    if (failures != 0) return 1;
    std::cout << "PASS: production GlowTemplate/GlowConnection/GlowCommand public setters; latest getters, one tagged child, sibling fields retained\n";
    return 0;
}
