using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using BerLib;
using EmberLib;
using EmberLib.Xml;
static class Program {
 static int Main(string[] args) {
  var tag=new BerTag(BerClass.ContextSpecific,1);
  EmberNode[] cases={new OctetStringEmberLeaf(tag,new byte[]{0,0xAB,0xFF}),new OctetStringEmberLeaf(tag,Array.Empty<byte>()),new ObjectIdentifierEmberLeaf(tag,new[]{2,100,3}),new RelativeOidEmberLeaf(tag,new[]{100,3})};
  int failures=0;
  foreach(var node in cases) {
   var xml=new StringBuilder();using(var writer=XmlWriter.Create(xml,new XmlWriterSettings{OmitXmlDeclaration=true}))XmlExport.Export(node,writer);
   bool okay=false;
   try {
    using(var reader=XmlReader.Create(new StringReader(xml.ToString()))) {
     var imported=XmlImport.Import(reader,null);
     if(node is OctetStringEmberLeaf b)okay=imported is OctetStringEmberLeaf result && b.Value.SequenceEqual(result.Value);
     else if(node is ObjectIdentifierEmberLeaf o)okay=imported is ObjectIdentifierEmberLeaf result && o.Value.SequenceEqual(result.Value);
     else if(node is RelativeOidEmberLeaf r)okay=imported is RelativeOidEmberLeaf result && r.Value.SequenceEqual(result.Value);
    }
   } catch(Exception e) {Console.WriteLine("Importer threw "+e.GetType().Name);}
   if(!okay)failures++;
   Console.WriteLine(node.GetType().Name+" actual exporter→importer parity="+okay);
  }
  Console.WriteLine("Public native XML roundtrip: failures="+failures+"/4");
  return failures == 0 ? 0 : 1;
 }
}
