using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using BerLib;
using EmberLib;
using EmberLib.Glow;
using EmberLib.Glow.Framing;
using System.Collections.Generic;
using EmberLib.Xml;
using System.Xml;

namespace EmberPlusSimpleProvider
{
   class Program
   {
      #region Entry Point
      static void Main(string[] args)
      {
         var program = new Program();

         if(args.Length > 0)
            program.Run(args[0]);
         else
            Console.WriteLine("USAGE: EmberPlusSimpleProvider <EmBER file>");
      }
      #endregion

      public void RemoveClient(Client client)
      {
         lock(_sync)
            _clients.Remove(client);
      }

      #region Implementation
      GlowContainer _glow;
      object _sync = new object();
      List<Client> _clients = new List<Client>();

      void Run(string path)
      {
         using(var stream = File.OpenRead(path))
         {
            var reader = new EmberReader(new BerStreamInput(stream));
            var app = new GlowApplicationInterface();

            _glow = (GlowContainer)EmberNode.Decode(reader, app);
         }

         var settings = new XmlWriterSettings
         {
            Indent = true,
            IndentChars = "  ",
            OmitXmlDeclaration = true,
         };
         using(var writer = XmlWriter.Create(Console.Out, settings))
            XmlExport.Export(_glow, writer);

         var children = _glow[GlowTags.CollectionItem][GlowTags.Node.Children];
         for(int index = 0; index < 2000; index++)
         {
            var node = new GlowNode(5 + index)
            {
               Identifier = "abc" + index,
               Description = "Hallo ich bin Node " + index,
            };
            children.Insert(node);
         }

         using(var output = new BerStreamOutput(File.Create("big.ember")))
            _glow.Encode(output);

         var listener = new TcpListener(IPAddress.Any, 9097);
         listener.Start();
         listener.BeginAcceptSocket(AcceptCallback, listener);

         Console.WriteLine("Press Enter to quit...");
         Console.ReadLine();

         lock(_sync)
         {
            foreach(var client in _clients)
               client.Close();
         }

         listener.Stop();
      }

      void AcceptCallback(IAsyncResult result)
      {
         var listener = (TcpListener)result.AsyncState;

         try
         {
            var socket = listener.EndAcceptSocket(result);
            var client = new Client(socket, this);

            lock(_sync)
               _clients.Add(client);

            if(_glow != null)
            {
               var glowOutPut = new GlowOutput(0, (_, e) => socket.Send(e.FramedPackage));

               using(glowOutPut)
               {
                  _glow.Encode(glowOutPut);

                  glowOutPut.Finish();
               }
            }

            listener.BeginAcceptSocket(AcceptCallback, listener);
         }
         catch
         {
         }
      }
      #endregion
   }
}
