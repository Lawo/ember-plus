using System;
using System.IO;
using System.Text;
using System.Xml;
using System.Globalization;
using EmberLib.Glow;
using EmberLib.Glow.PowerPack.Xml;
static class Program {
 static GlowContainer RoundTrip(GlowContainer node) {
  var text=new StringBuilder();using(var writer=XmlWriter.Create(text,new XmlWriterSettings{OmitXmlDeclaration=true}))GlowXmlExport.Export(node,writer);
  using(var reader=XmlReader.Create(new StringReader(text.ToString())))return GlowXmlImport.Import(reader);
 }
 static int Main(string[] args) {
  int bad=0;var parameter=new GlowParameter(1){IsOnline=false,Default=new GlowValue(5L)};
  var result=RoundTrip(parameter) as GlowParameter;
  if(result==null||result.IsOnline!=false)bad++;
  if(result==null||result.Default==null||result.Default.ToString(CultureInfo.InvariantCulture)!="5")bad++;
  Console.WriteLine("parameter metadata mismatches="+bad);
  foreach(var payload in new GlowElement[]{new GlowNode(2,GlowTags.Template.Element),new GlowParameter(2,GlowTags.Template.Element),new GlowMatrix(2,GlowTags.Template.Element),new GlowFunction(2,GlowTags.Template.Element)}) {
   var template=new GlowTemplate(1){Description="native",Element=payload};var imported=RoundTrip(template) as GlowTemplate;
   bool ok=imported!=null&&imported.Description=="native"&&imported.Element!=null&&imported.Element.GetType()==payload.GetType()&&imported.Element.Tag==GlowTags.Template.Element;
   if(!ok)bad++;Console.WriteLine(payload.GetType().Name+" template parity="+ok);
  }
  var qualified=new GlowQualifiedTemplate(new[]{1,2},null){Description="qualified",Element=new GlowNode(3,GlowTags.Template.Element)};
  var q=RoundTrip(qualified) as GlowQualifiedTemplate;
  if(q==null||q.Description!="qualified"||q.Element==null||q.Element.Tag!=GlowTags.Template.Element)bad++;
  Console.WriteLine("Public Glow exporter→importer mismatches="+bad+"/7");
  return bad == 0 ? 0 : 1;
 }
}
