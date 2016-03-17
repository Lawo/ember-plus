#include "GadgetTreeWriter.h"
#include "../../glow/util/NodeConverter.h"
#include "../../glow/util/ParameterConverter.h"

namespace serialization { namespace detail
{
    GadgetTreeWriter::GadgetTreeWriter(gadget::Node const* node)
        : m_stream(0)
    {
        auto root = libember::glow::GlowRootElementCollection::create();
        write(node, root);

        root->encode(m_stream);
        delete root;
    }

    GadgetTreeWriter::~GadgetTreeWriter()
    {
    }

    GadgetTreeWriter::size_type GadgetTreeWriter::size() const
    {
        return m_stream.size();
    }

    GadgetTreeWriter::const_iterator GadgetTreeWriter::begin() const
    {
        return m_stream.begin();
    }

    GadgetTreeWriter::const_iterator GadgetTreeWriter::end() const
    {
        return m_stream.end();
    }

    void GadgetTreeWriter::write(gadget::Node const* node, libember::glow::GlowContainer* parent) const
    {
        auto transform = glow::util::NodeConverter::create(parent, node, gadget::NodeField::All);
        auto const& nodes = node->nodes();
        auto const& params = node->parameters();
        auto children = transform->children();

        for(auto param : params)
        {
            write(param, children);
        }

        for(auto child : nodes)
        {
            write(child, children);
        }
    }

    void GadgetTreeWriter::write(gadget::Parameter const* param, libember::glow::GlowContainer* parent) const
    {
        glow::util::ParameterConverter::create(parent, param, gadget::ParameterField::All | gadget::ParameterField::Type);
    }
}
}
