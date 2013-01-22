using System;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using System.Xml;
using EmberLib.Framing;
using EmberLib.Glow.Framing;

namespace EmberPlusWalk
{
   /// <summary>
   /// A communication port used to send and receive ember/glow data.
   /// </summary>
   class GlowEndPoint : IDisposable
   {
      /// <summary>
      /// Creates a new instance of GlowEndPoint.
      /// </summary>
      /// <param name="hostName">The name or address of the remote host to connect to.</param>
      /// <param name="tcpPort">The port of the remote host to connect to.</param>
      /// <param name="maxPackageLength">The maximum length of a tx package.</param>
      /// <param name="remoteSlotId">The slot id used for the framing protocol.</param>
      public GlowEndPoint(string hostName, int tcpPort, int maxPackageLength, byte remoteSlotId = 0)
      {
         HostName = hostName;
         TcpPort = tcpPort;
         MaxPackageLength = maxPackageLength;
         RemoteSlotId = remoteSlotId;
      }

      /// <summary>
      /// Gets the name of the remote host.
      /// </summary>
      public string HostName { get; private set; }

      /// <summary>
      /// Gets the port of the remote host.
      /// </summary>
      public int TcpPort { get; private set; }

      /// <summary>
      /// Gets the maximum length of tx package.
      /// </summary>
      public int MaxPackageLength { get; private set; }

      /// <summary>
      /// Gets the slot id used for the framing protocol.
      /// </summary>
      public byte RemoteSlotId { get; private set; }

      #region GlowRootReady Event
      public class GlowRootReadyArgs : EventArgs
      {
         public EmberLib.Glow.GlowContainer Root { get; private set; }

         public GlowRootReadyArgs(EmberLib.Glow.GlowContainer root)
         {
            Root = root;
         }
      }

      /// <summary>
      /// Raised when a valid ember/glow package has been read.
      /// </summary>
      public event EventHandler<GlowRootReadyArgs> GlowRootReady;

      protected virtual void OnGlowRootReady(GlowRootReadyArgs e)
      {
         if(GlowRootReady != null)
            GlowRootReady(this, e);
      }
      #endregion

      #region Notification Event
      public class NotificationArgs : EventArgs
      {
         public string Message { get; private set; }

         public NotificationArgs(string message)
         {
            Message = message;
         }
      }

      /// <summary>
      /// Raised when encountering errors.
      /// </summary>
      public event EventHandler<NotificationArgs> Notification;

      protected virtual void OnNotification(NotificationArgs e)
      {
         if(Notification != null)
            Notification(this, e);
      }
      #endregion

      /// <summary>
      /// Synchronously connects to the remote host.
      /// </summary>
      /// <returns>True if the connection attempt completed successfully, otherwise false.</returns>
      public bool Connect()
      {
         Close();

         lock(_sync)
         {
            _tcpClient = new TcpClient();

            try
            {
               _tcpClient.Connect(HostName, TcpPort);

               _glowReader = new GlowReader(GlowReader_RootReady, GlowReader_KeepAliveRequestReceived);
               _glowReader.Error += GlowReader_Error;

               var stream = _tcpClient.GetStream();
               stream.BeginRead(_buffer, 0, _buffer.Length, ReceiveCallback, stream);

               return true;
            }
            catch(Exception ex)
            {
               Console.WriteLine(ex.Message);
            }
         }

         return false;
      }

      /// <summary>
      /// Sends an ember/glow tree to the remote host.
      /// </summary>
      /// <param name="root">The root of the tree to send.</param>
      public void Write(EmberLib.Glow.GlowContainer root)
      {
         lock(_sync)
         {
            var tcpClient = _tcpClient;

            if(tcpClient != null
            && tcpClient.Connected)
            {
               var stream = tcpClient.GetStream();
               var output = CreateFramingOutput((_, e) => stream.Write(e.FramedPackage, 0, e.FramedPackageLength));

               using(output)
               {
                  root.Encode(output);

                  output.Finish();
               }
            }
         }
      }

      /// <summary>
      /// Closes the endpoint.
      /// </summary>
      public void Close()
      {
         lock(_sync)
         {
            if(_glowReader != null)
               _glowReader.Dispose();

            if(_tcpClient != null)
            {
               try
               {
                  _tcpClient.Close();
               }
               catch
               {
               }
            }

            _glowReader = null;
            _tcpClient = null;
         }
      }

      #region Implementation
      object _sync = new object();
      TcpClient _tcpClient;
      byte[] _buffer = new byte[1024];
      EmberLib.Glow.Framing.GlowReader _glowReader;
      int _lastGlowFramingError = -1;

      FramingBerOutput CreateFramingOutput(EventHandler<FramingBerOutput.PackageReadyArgs> handler)
      {
         return new GlowOutput(MaxPackageLength, RemoteSlotId, handler);
      }

      void ReceiveCallback(IAsyncResult result)
      {
         var stream = (Stream)result.AsyncState;

         try
         {
            var read = stream.EndRead(result);

            if(read > 0)
            {
               GlowReader reader;

               lock(_sync)
                  reader = _glowReader;

               if(reader != null)
                  reader.ReadBytes(_buffer, 0, read);

               stream.BeginRead(_buffer, 0, _buffer.Length, ReceiveCallback, stream);
            }
         }
         catch(SocketException ex)
         {
            OnNotification(new NotificationArgs(ex.Message));
         }
         catch(ObjectDisposedException)
         {
         }
      }

      /// <summary>
      /// Invoked everytime _glowReader has decoded a complete Ember tree
      /// </summary>
      void GlowReader_RootReady(object sender, EmberLib.AsyncDomReader.RootReadyArgs e)
      {
         var root = e.Root as EmberLib.Glow.GlowContainer;

         if(root != null)
            OnGlowRootReady(new GlowRootReadyArgs(root));
         else
            OnNotification(new NotificationArgs(String.Format("Unexpected Ember Root: {0} ({1})", root, root.GetType())));
      }

      /// <summary>
      /// Invoked everytime _glowReader has unframed a Keep-Alive request.
      /// </summary>
      void GlowReader_KeepAliveRequestReceived(object sender, FramingReader.KeepAliveRequestReceivedArgs e)
      {
         _tcpClient.Client.Send(e.Response, e.ResponseLength, SocketFlags.None);
      }

      /// <summary>
      /// Invoked everytime _glowReader encounters an error in the inbound data.
      /// </summary>
      void GlowReader_Error(object sender, GlowReader.ErrorArgs e)
      {
         if(e.ErrorCode != _lastGlowFramingError)
         {
            OnNotification(new NotificationArgs(String.Format("Error: {0}", e.Message)));

            _lastGlowFramingError = e.ErrorCode;
         }
      }
      #endregion

      #region IDisposable Members
      void IDisposable.Dispose()
      {
         Close();

         Notification = null;
      }
      #endregion
   }
}
