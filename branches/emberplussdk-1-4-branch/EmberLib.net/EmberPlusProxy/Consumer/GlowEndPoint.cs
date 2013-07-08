using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using EmberLib.Framing;
using EmberLib.Glow.Framing;
using BerLib;
using EmberLib.Glow;

namespace EmberPlusProxy.Consumer
{
   /// <summary>
   /// A communication port used to send and receive ember/glow data.
   /// </summary>
   class GlowEndPoint : IDisposable
   {
      public GlowEndPoint(string hostName, int tcpPort, int maxPackageLength, int localNumber, byte remoteSlotId = 0, string description = null)
      {
         HostName = hostName;
         TcpPort = tcpPort;
         MaxPackageLength = maxPackageLength;
         RemoteSlotId = remoteSlotId;
         LocalNumber = localNumber;
         Description = description;

         _state = GlowEndPointState.Closed;
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

      public int LocalNumber { get; private set; }

      public string Description { get; private set; }

      public GlowEndPointState State
      {
         get { return _state; }
         private set
         {
            _state = value;

            OnStateChanged();
         }
      }

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

      #region StateChanged Event
      public event EventHandler StateChanged;

      protected virtual void OnStateChanged()
      {
         if(StateChanged != null)
            StateChanged(this, EventArgs.Empty);
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
            State = GlowEndPointState.Connecting;
            _tcpClient = new TcpClient();

            try
            {
               _tcpClient.Connect(HostName, TcpPort);

               _glowReader = new GlowReader(GlowReader_RootReady, GlowReader_KeepAliveRequestReceived);
               _glowReader.Error += GlowReader_Error;

               var stream = _tcpClient.GetStream();
               stream.BeginRead(_buffer, 0, _buffer.Length, ReceiveCallback, stream);

               State = GlowEndPointState.ProtocolProbing;

               var glow = GlowRootElementCollection.CreateRoot();
               glow.Insert(new GlowCommand(GlowCommandType.GetDirectory));
               Write(glow);
               return true;
            }
            catch(Exception ex)
            {
               State = GlowEndPointState.Error;
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
               var output = CreateFramingOutput(
                  (_, e) =>
                  {
                     var stream = tcpClient.GetStream();
                     stream.Write(e.FramedPackage, 0, e.FramedPackageLength);
                  });

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
            State = GlowEndPointState.Closed;
         }
      }

      #region Implementation
      object _sync = new object();
      TcpClient _tcpClient;
      byte[] _buffer = new byte[1024];
      EmberLib.Glow.Framing.GlowReader _glowReader;
      int _lastGlowFramingError = -1;
      GlowEndPointState _state;

      EmberLib.Framing.FramingBerOutput CreateFramingOutput(EventHandler<EmberLib.Framing.FramingBerOutput.PackageReadyArgs> handler)
      {
         return new EmberLib.Glow.Framing.GlowOutput(MaxPackageLength, RemoteSlotId, handler);
      }

      void ReceiveCallback(IAsyncResult result)
      {
         var stream = (Stream)result.AsyncState;

         try
         {
            var read = stream.EndRead(result);

            if(read > 0)
            {
               EmberLib.Framing.FramingReader reader;

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

      void GlowReader_RootReady(object sender, EmberLib.AsyncDomReader.RootReadyArgs e)
      {
         var root = e.Root as EmberLib.Glow.GlowContainer;
         State = GlowEndPointState.Connected;

         if(root != null)
            OnGlowRootReady(new GlowRootReadyArgs(root));
         else
            OnNotification(new NotificationArgs(String.Format("Unexpected Ember Root: {0} ({1})", root, root.GetType())));
      }

      void GlowReader_KeepAliveRequestReceived(object sender, FramingReader.KeepAliveRequestReceivedArgs e)
      {
         TcpClient tcpClient;
         State = GlowEndPointState.Connected;

         lock(_sync)
            tcpClient = _tcpClient;

         if(tcpClient != null)
            tcpClient.Client.Send(e.Response, e.ResponseLength, SocketFlags.None);
      }

      void GlowReader_Error(object sender, EmberLib.Glow.Framing.GlowReader.ErrorArgs e)
      {
         if(e.ErrorCode != _lastGlowFramingError)
         {
            OnNotification(new NotificationArgs(String.Format("Framing Error: {0}", e.Message)));

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

   /// <summary>
   /// Possible states for GlowEndPoint.State.
   /// </summary>
   enum GlowEndPointState
   {
      Closed,
      Connecting,
      ProtocolProbing,
      Connected,
      Error,
   }
}
