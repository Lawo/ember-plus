using System;
using System.Collections.Generic;
using System.Text;
using EmberLib;
using System.IO;
using System.Xml;
using System.Diagnostics;
using BerLib;
using EmberLib.Legacy;
using EmberLib.Xml;
using EmberLib.Glow;
using EmberLib.Framing;
using EmberLib.Glow.Framing;
using EmberLib.Glow.Formula;
using EmberLib.Legacy.Extensions;
using System.Globalization;

namespace EmberTest.NET
{
   class Program : EmberApplicationInterface
   {
      const BerClass DefaultClass = BerClass.ContextSpecific;

      #region Entry Point
      static void Main(string[] args)
      {
         var program = new Program();

         program.Run();
      }

      void Run()
      {
         Test_GlowFormula();
         Test_DOM();
         Test_InteropDom();
         Test_ReaderWriter();
         Test_XmlImport();

         Console.Write("Press Enter to quit...");
         Console.ReadLine();
      }
      #endregion

      string DumpXml(EmberNode node)
      {
         var xmlBuffer = new StringBuilder();
         var xmlSettings = new XmlWriterSettings { OmitXmlDeclaration = true };

         using(var writer = XmlWriter.Create(xmlBuffer, xmlSettings))
            XmlExport.Export(node, writer);

         var str = xmlBuffer.ToString();
         Console.WriteLine(str);
         return str;
      }

      #region Formula
      void Test_FormulaPerf(string source, int iterations)
      {
         var result = Compiler.Compile(source, false);

         if(result.Success)
         {
            Stopwatch stopwatch;
            var integerValue = null as GlowValue;
            var doubleValue = null as GlowValue;

            Console.WriteLine("--- Interpreter compilation");
            stopwatch = Stopwatch.StartNew();
            for(int i = 0; i < 10000; i++)
               Compiler.Compile(source, false);
            stopwatch.Stop();
            Console.WriteLine("perf {0}ms", stopwatch.ElapsedMilliseconds);

            Console.WriteLine("--- Emitter compilation");
            stopwatch = Stopwatch.StartNew();
            for(int i = 0; i < 10000; i++)
               Compiler.Compile(source, true);
            stopwatch.Stop();
            Console.WriteLine("perf {0}ms", stopwatch.ElapsedMilliseconds);

            Console.WriteLine("--- Interpreter");
            stopwatch = Stopwatch.StartNew();
            for(int i = 0; i < iterations; i++)
            {
               doubleValue = result.Formula.Eval(100.0);
               integerValue = result.Formula.Eval(100);
            }
            stopwatch.Stop();

            Console.WriteLine("INT> {0}", integerValue.ToString(CultureInfo.InvariantCulture));
            Console.WriteLine("FLT> {0}", doubleValue.ToString(CultureInfo.InvariantCulture));
            Console.WriteLine("perf {0}ms", stopwatch.ElapsedMilliseconds);


            Console.WriteLine("--- Emitter");
            result = Compiler.Compile(source, true);
            stopwatch = Stopwatch.StartNew();
            for(int i = 0; i < iterations; i++)
            {
               doubleValue = result.Formula.Eval(100.0);
               integerValue = result.Formula.Eval(100);
            }
            stopwatch.Stop();

            Console.WriteLine("INT> {0}", integerValue.ToString(CultureInfo.InvariantCulture));
            Console.WriteLine("FLT> {0}", doubleValue.ToString(CultureInfo.InvariantCulture));
            Console.WriteLine("perf {0}ms", stopwatch.ElapsedMilliseconds);
         }
         else
         {
            Console.WriteLine("! {0}", result.Error.Message);
         }
      }

      void Test_GlowFormula()
      {
         const int iterations = 500000;
         string source;

         Console.Write(": ");
         while(String.IsNullOrEmpty(source = Console.ReadLine()) == false)
         {
            Test_FormulaPerf(source, iterations);

            Console.Write(": ");
         }
      }
      #endregion

      #region Xml Import
      void Test_XmlImport()
      {
         var frame = new EmberSequence(new BerTag(BerClass.Application, 1));
         var appDefined1 = EmberApplicationInterface.CreateApplicationDefinedSet(new BerTag(BerClass.ContextSpecific, 444), 1, frame);
         var appDefined2 = EmberApplicationInterface.CreateApplicationDefinedSequence(new BerTag(BerClass.ContextSpecific, 445), 2, appDefined1);
         appDefined2.Insert(new BerTag(BerClass.ContextSpecific, 1), -1);
         appDefined2.Insert(new BerTag(BerClass.ContextSpecific, 2), true);
         appDefined2.Insert(new BerTag(BerClass.ContextSpecific, 3), false);
         appDefined2.Insert(new BerTag(BerClass.ContextSpecific, 4), 12345.6789);
         appDefined2.Insert(new BerTag(BerClass.ContextSpecific, 5), "wasgehtäb?");

         Console.WriteLine("\r\n------------------------ XML Import");
         DumpXml(frame);

         var buffer = new StringBuilder();
         using(var writer = XmlWriter.Create(buffer))
            XmlExport.Export(frame, writer);

         using(var stream = new StringReader(buffer.ToString()))
         using(var reader = new XmlTextReader(stream))
         {
            var root = XmlImport.Import(reader, this);

            DumpXml(root);
         }
      }
      #endregion

      #region DOM
      void Test_InteropDom()
      {
         Console.WriteLine("\r\n------------------------ Interop DOM");

         var testFilePath = @"N:\Temp\test.ber";
         using(var stream = File.OpenRead(testFilePath))
         {
            var input = new BerStreamInput(stream);
            var reader = new EmberReader(input);

            var root = EmberNode.Decode(reader, this);
            DumpXml(root);
         }
      }

      void Test_DOM()
      {
         EmberContainer container1;
         EmberContainer frame = new EmberFrame();

         container1 = new EmberSet(new BerTag(DefaultClass, 0));
         container1.Insert(new BerTag(DefaultClass, 0), -1);
         container1.Insert(new BerTag(DefaultClass, 1), 128);
         container1.Insert(new BerTag(DefaultClass, 2), -128);
         container1.Insert(new BerTag(DefaultClass, 3), 255);
         container1.Insert(new BerTag(DefaultClass, 4), -255);
         container1.Insert(new BerTag(DefaultClass, 5), 12345);
         container1.Insert(new BerTag(DefaultClass, 6), -12345);
         container1.Insert(new BerTag(DefaultClass, 7), 16384);
         container1.Insert(new BerTag(DefaultClass, 8), -16384);
         container1.Insert(new BerTag(DefaultClass, 9), 65535);
         container1.Insert(new BerTag(DefaultClass, 10), -65535);
         container1.Insert(new BerTag(DefaultClass, 11), 0);
         container1.Insert(new BerTag(DefaultClass, 12), 127);
         container1.Insert(new BerTag(DefaultClass, 13), -127);
         container1.Insert(new BerTag(DefaultClass, 1111222), 0xFFFFFFFF);
         container1.InsertOid(new BerTag(DefaultClass, 14), new int[] { 1, 3, 6, 0 });
         container1.InsertOid(new BerTag(DefaultClass, 15), new int[] { 1 });
         container1.InsertRelativeOid(new BerTag(DefaultClass, 16), new int[] { 1, 2, 3, 4, 5, 6 });
         frame.Insert(container1);

         container1 = new EmberSequence(new BerTag(DefaultClass, 1));
         container1.Insert(new BerTag(DefaultClass, 3), -0.54321);
         container1.Insert(new BerTag(DefaultClass, 5), "Wuppdich");

         var appDefined = EmberApplicationInterface.CreateApplicationDefinedSequence(new BerTag(BerClass.Application, 889), 2, container1);
         appDefined.Insert(new BerTag(DefaultClass, 100), true);

         frame.Insert(container1);

         //DumpEmberXml(frame);
         WriteNodeDeep(frame);

         var output = new BerMemoryOutput();
         frame.Encode(output);

         var memory = output.ToArray();
         using(var stream = new FileStream(@"N:\Temp\test.ber", FileMode.Create, FileAccess.Write))
            stream.Write(memory, 0, memory.Length);

         var input = new BerMemoryInput(memory);

         var stopwatch = new Stopwatch();
         stopwatch.Start();

         var asyncReader = new AsyncFrameReader(this);
         asyncReader.ReadBytes(output.Memory);
         var loadedFrame = asyncReader.DetachRoot();

         stopwatch.Stop();
         Console.WriteLine("load tree: {0}ms", stopwatch.ElapsedMilliseconds);

         Console.WriteLine("\r\n------------------------ DOM");
         DumpXml(loadedFrame);
      }
      #endregion

      #region Generic Output
      struct StackElem
      {
         public EmberNode Node;
         public IEnumerator<EmberNode> Position;
         public int Indent;

         public StackElem(EmberNode oNode, int nIndent)
         {
            this.Node = oNode;
            this.Position = oNode.GetEnumerator();
            this.Indent = nIndent;
         }
      }

      void WriteNodeDeep(EmberNode frame)
      {
         var nodeStack = new Stack<StackElem>();
         nodeStack.Push(new StackElem(frame, 1));
         Console.WriteLine(frame.ToString());

         while(nodeStack.Count > 0)
         {
            var elem = nodeStack.Peek();
            var node = elem.Node;
            var position = elem.Position;
            var indent = elem.Indent;
            var isPushed = false;

            while(position.MoveNext())
            {
               var current = position.Current;

               Console.WriteLine(new String(' ', indent * 3) + current.ToString());

               if(current is EmberContainer)
               {
                  nodeStack.Push(new StackElem(current, indent + 1));
                  isPushed = true;
                  break;
               }
            }

            if(isPushed == false)
               nodeStack.Pop();
         }
      }
      #endregion

      #region Reader Writer
      void Test_ReaderWriter()
      {
         var output = new BerMemoryOutput();
         var writer = new EmberWriter(output);

         writer.WriteFrameBegin();

         writer.WriteSequenceBegin(new BerTag(DefaultClass, 1));

         for(uint index = 0; index <= 100; index++)
            writer.Write(new BerTag(DefaultClass, index + 111122), index);

         writer.WriteContainerEnd();

         writer.WriteContainerEnd();

         Console.WriteLine("\r\n------------------------ Reader, Writer");

         var asyncReader = new AsyncDomReader(null);
         asyncReader.ReadBytes(output.Memory);
         DumpXml(asyncReader.DetachRoot());

         //var input = new BerMemoryInput(output.Memory);
         //var reader = new EmberReader(input);

         //var xmlBuffer = new StringBuilder();
         //var xmlSettings = new XmlWriterSettings
         //{
         //   OmitXmlDeclaration = true,
         //};

         //using(var xmlWriter = XmlWriter.Create(xmlBuffer, xmlSettings))
         //   EmberToXml(reader, xmlWriter);
         //Console.WriteLine(xmlBuffer.ToString());
      }
      #endregion

      #region Custom EmberToXml
      void EmberToXml(EmberReader reader, XmlWriter writer)
      {
         if(reader.Read())
            EmberToXml_ProcessNode(reader, writer, 0);
      }

      void EmberToXml_ProcessNode(EmberReader reader, XmlWriter writer, int indent)
      {
         var isContainer = reader.IsContainer;
         var indentStr = new String(' ', 2 * indent);

         writer.WriteWhitespace(indentStr);
         writer.WriteStartElement(reader.Tag.ToString());

         writer.WriteStartAttribute("type");
         writer.WriteString(BerDefinitions.GetTypeName(reader.Type));
         writer.WriteEndAttribute();

         if(isContainer)
         {
            writer.WriteWhitespace(Environment.NewLine);

            EmberToXml_Recurse(new EmberReader(reader), writer, indent + 1);

            writer.WriteWhitespace(indentStr);
         }
         else
         {
            switch(reader.Type)
            {
               case BerType.Boolean:
                  writer.WriteValue(reader.GetBoolean());
                  break;

               case BerType.Integer:
                  writer.WriteValue(reader.GetLong());
                  break;

               case BerType.Real:
                  writer.WriteValue(reader.GetReal());
                  break;

               case BerType.UTF8String:
                  writer.WriteValue(reader.GetString());
                  break;
            }
         }

         writer.WriteEndElement();
         writer.WriteWhitespace(Environment.NewLine);
      }

      void EmberToXml_Recurse(EmberReader reader, XmlWriter writer, int indent)
      {
         while(reader.Read())
            EmberToXml_ProcessNode(reader, writer, indent);
      }
      #endregion

      #region Glow
      /*
      delegate GlowParameter CreateParameterFunc();

      void Test_Glow()
      {
         // --- create tree
         //
         var root = GlowElementCollection.CreateRoot();
         var random = new Random();

         for(int i = 0; i < 20; i++)
         {
            CreateParameterFunc createParameter =
               () =>
               {
                  var node = new GlowParameter(i + 1000)
                  {
                     Identifier = "ParameterIdentifier_" + random.Next(),
                     IsWriteable = (random.Next() & 1) == 1,
                     IsOnline = (random.Next() & 1) == 1,
                     IsCommand = (random.Next() & 1) == 1,
                  };

                  switch(random.Next(3))
                  {
                     case 0:
                        {
                           var value = random.Next();
                           node.Value = new GlowValue(value);
                           node.Minimum = new GlowMinMax(value - 100);
                           node.Maximum = new GlowMinMax(value + 100);
                           break;
                        }
                     case 1:
                        {
                           var value = random.NextDouble() * 1000;
                           node.Value = new GlowValue(value);
                           node.Minimum = new GlowMinMax(value - 100);
                           node.Maximum = new GlowMinMax(value + 100);
                           break;
                        }
                     case 2:
                        node.Value = new GlowValue("TestString");
                        break;
                  }

                  return node;
               };

            var element = null as GlowElement;

            switch(random.Next(3))
            {
               case 0:
               {
                  element = createParameter();
                  break;
               }

               case 1:
               {
                  var node = new GlowNode(i + 2000);
                  node.Identifier = "NodeIdentifier_" + random.Next();

                  var children = new GlowElementCollection(GlowTags.Node.Children);
                  children.Insert(createParameter());

                  node.Children = children;
                  element = node;
                  break;
               }

               case 2:
               {
                  var node = new GlowCommand(i + 3000);
                  element = node;
                  break;
               }
            }

            if(element != null)
               root.Insert(element);
         }

         Console.WriteLine("\r\n------------------------ Glow");
         //DumpXml(root);

         // --- encoding and decoding
         //
         //var output = new BerMemoryOutput();
         //root.Write(output);

         //var memory = output.Memory;
         //var input = new BerMemoryInput(memory);
         //var reader = new EmberReader(input);

         //var decodedRoot = EmberNode.BuildTree(reader, true, new GlowApplicationInterface());
         //DumpXml(decodedRoot);

         // --- framing output
         //
         var stopwatch = new Stopwatch();
         stopwatch.Start();

         var asyncReader = new AsyncDomReader(false, new GlowApplicationInterface());

         using(var framingReader = new GlowReader(asyncReader))
         {
            var framingOut = new GlowOutput(
               (_, e) =>
               {
                  framingReader.ReadBytes(e.FramedPackage, 0, e.FramedPackageLength);
               });

            using(framingOut)
            {
               root.Write(framingOut);

               framingOut.Finish();
            }
         }

         stopwatch.Stop();
         var ms = stopwatch.ElapsedMilliseconds;

         var decodedRoot2 = asyncReader.DetachRoot();
         DumpXml(decodedRoot2);

         ((GlowContainer)decodedRoot2).Accept(new GlowVisitor(), "");

         Console.WriteLine("Framing encode/decode cycle took {0} ms.", ms);
      }

      class GlowOutput : FramingBerOutput
      {
         public GlowOutput(EventHandler<PackageReadyArgs> packageReadyHandler)
         : base(64*1024, 100, Dtd.Glow, packageReadyHandler)
         {
         }

         protected override byte[] GetApplicationBytes(out ushort length)
         {
            var bytes = new byte[] { 71, 72, 73, 74, 75 };
            length = (ushort)bytes.Length;
            return bytes;
         }
      }

      class GlowVisitor : IGlowVisitor<string, object>
      {
         #region IGlowVisitor<object,object> Members
         public object Visit(GlowCommand node, string state)
         {
            Console.WriteLine(state + "Command: {0} {1}", node.Tag, node.Number);
            return null;
         }

         public object Visit(GlowElementCollection node, string state)
         {
            Console.WriteLine(state + "ElementCollection: {0}", node.Tag);

            foreach(var element in node.Elements)
               element.Accept(this, state + "    ");

            return null;
         }

         public object Visit(GlowNode node, string state)
         {
            Console.WriteLine(state + "Node: {0} {1}", node.Tag, node.Identifier);

            var children = node.Children;

            if(children != null)
               children.Accept(this, state + "    ");

            return null;
         }

         public object Visit(GlowParameter node, string state)
         {
            Console.WriteLine(state + "Parameter: {0} {1}", node.Tag, node.Identifier);

            var children = node.Children;

            if(children != null)
               children.Accept(this, state + "    ");

            return null;
         }

         public object Visit(GlowStreamCollection node, string state)
         {
            Console.WriteLine(state + "StreamCollection: {0}", node.Tag);

            foreach(var entry in node.StreamEntries)
               entry.Accept(this, state + "    ");

            return null;
         }

         public object Visit(GlowStreamEntry node, string state)
         {
            Console.WriteLine(state + "StreamEntry: {0}", node.Tag);
            return null;
         }

         public object Visit(GlowStringIntegerPair node, string state)
         {
            Console.WriteLine(state + "StringIntegerPair: {0}", node.Tag);
            return null;
         }

         public object Visit(GlowStringIntegerCollection node, string state)
         {
            Console.WriteLine(state + "StringIntegerCollection: {0}", node.Tag);
            return null;
         }
         #endregion
      }
      */
      #endregion

      #region EmberApplicationInterface Members
      public override EmberNode CreateNodeFromReader(uint type, BerReaderBase reader)
      {
         switch(type & ~BerType.ApplicationFlag)
         {
            case 1:
               return CreateSet(type, reader);

            case 2:
               return CreateSequence(type, reader);
         }

         return null;
      }

      public override EmberNode CreateNodeFromXml(uint type, BerTag tag, XmlReader reader)
      {
         switch(type & ~BerType.ApplicationFlag)
         {
            case 1:
               return CreateSet(type, tag, reader);

            case 2:
               return CreateSequence(type, tag, reader);
         }

         return null;
      }
      #endregion
   }
}
