using System;
using System.Net.Sockets;
using System.Text;
using System.Xml;
using EmberLib;
using EmberLib.Framing;
using EmberLib.Glow;
using EmberLib.Glow.Framing;
using EmberLib.Xml;

namespace EmberPlusProxy.Provider
{
   class Client : IDisposable
   {
      public Client(GlowListener host, Socket socket, int maxPackageLength, Proxy proxy)
      {
         Host = host;
         Socket = socket;
         MaxPackageLength = maxPackageLength;
         Proxy = proxy;

         _reader = new GlowReader(GlowReader_RootReady, GlowReader_KeepAliveRequestReceived);
         _reader.Error += GlowReader_Error;
         _reader.FramingError += GlowReader_FramingError;
      }

      public GlowListener Host { get; private set; }
      public Socket Socket { get; private set; }
      public int MaxPackageLength { get; private set; }
      public Proxy Proxy { get; private set; }

      public void Read(byte[] buffer, int count)
      {
         GlowReader reader;

         lock(_sync)
         {
            reader = _reader;
            Console.WriteLine("Received {0} bytes from {1}", count, Socket.RemoteEndPoint);
         }

         if(reader != null)
            reader.ReadBytes(buffer, 0, count);
      }

      public void Write(GlowContainer glow)
      {
         var output = CreateOutput();

         glow.Encode(output);

         output.Finish();
      }

      #region Implementation
      GlowReader _reader;
      object _sync = new object();

      void GlowReader_RootReady(object sender, AsyncDomReader.RootReadyArgs e)
      {
         var root = e.Root as GlowContainer;

         if(root != null)
         {
            var buffer = new StringBuilder();
            var settings = new XmlWriterSettings
            {
               OmitXmlDeclaration = true,
               Indent = true,
               IndentChars = "  ",
            };

            using(var writer = XmlWriter.Create(Console.Out, settings))
               XmlExport.Export(root, writer);

            Proxy.DispatchGlow(root, this);
         }
         else
         {
            Console.WriteLine("Unexpected Ember Root: {0} ({1})", e.Root, e.Root.GetType());
         }
      }

      void GlowReader_Error(object sender, GlowReader.ErrorArgs e)
      {
         Console.WriteLine("GlowReader error {0}: {1}", e.ErrorCode, e.Message);
      }

      void GlowReader_FramingError(object sender, EmberLib.Framing.FramingReader.FramingErrorArgs e)
      {
         Console.WriteLine("GlowReader framing error: {0}", e.Message);
      }

      void GlowReader_KeepAliveRequestReceived(object sender, FramingReader.KeepAliveRequestReceivedArgs e)
      {
         Socket socket;

         lock(_sync)
            socket = Socket;

         if(socket != null)
            socket.Send(e.Response, e.ResponseLength, SocketFlags.None);
      }

      GlowOutput CreateOutput()
      {
         return new GlowOutput(MaxPackageLength, 0,
            (_, e) =>
            {
               Socket socket;
               GlowListener host;

               lock(_sync)
               {
                  socket = Socket;
                  host = Host;
               }

               if(socket != null)
               {
                  try
                  {
                     socket.Send(e.FramedPackage, e.FramedPackageLength, SocketFlags.None);
                  }
                  catch(SocketException)
                  {
                     if(host != null)
                        host.CloseClient(this);
                  }
               }
            });
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         Socket socket;
         GlowReader reader;

         lock(_sync)
         {
            socket = Socket;
            reader = _reader;

            Socket = null;
            _reader = null;
            Host = null;
         }

         if(socket != null)
         {
            try
            {
               socket.Close();
            }
            catch
            {
            }
         }

         if(reader != null)
            reader.Dispose();
      }
      #endregion
   }
}
