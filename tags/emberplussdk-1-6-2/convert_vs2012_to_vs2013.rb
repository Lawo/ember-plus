require 'rexml/document'

include REXML

Dir.chdir("build") do
    Dir.glob("*.vcxproj") do |filename|

        file = File.read(filename)
        doc = Document.new(file)
        doc.context[:attribute_quote] = :quote

        XPath.each(doc, "//PlatformToolset") do |elem|
            elem.text = "v120"
        end

        File.open(filename, "w") do |file|
            doc.write(:output => file, :ie_hack => true)
        end
    end
end
