using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace EmberPlusProxy.Provider
{
   class GlowListener : IDisposable
   {
      public GlowListener(int port, int maxPackageLength, Proxy proxy)
      {
         Port = port;
         MaxPackageLength = maxPackageLength;
         Proxy = proxy;

         _listener = new TcpListener(IPAddress.Any, port);
         _listener.Start();
         _listener.BeginAcceptSocket(AcceptCallback, _listener);

         proxy.GlowFromProviderReady += Proxy_GlowFromProviderReady;
      }

      public int Port { get; private set; }
      public int MaxPackageLength { get; private set; }
      public Proxy Proxy { get; private set; }

      public void CloseClient(Client client)
      {
         lock(_sync)
            _clients.Remove(client);

         client.Dispose();
      }

      #region Implementation
      TcpListener _listener;
      List<Client> _clients = new List<Client>();
      byte[] _buffer = new byte[1024];
      object _sync = new object();

      void AcceptCallback(IAsyncResult result)
      {
         var listener = (TcpListener)result.AsyncState;

         try
         {
            var socket = listener.EndAcceptSocket(result);
            var client = new Client(this, socket, MaxPackageLength, Proxy);

            lock(_sync)
               _clients.Add(client);

            listener.BeginAcceptSocket(AcceptCallback, listener);
            socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, ReceiveCallback, client);
         }
         catch(SocketException ex)
         {
            Console.WriteLine("Accept error: {0}", ex);
         }
         catch(ObjectDisposedException)
         {
         }
      }

      void ReceiveCallback(IAsyncResult result)
      {
         var client = (Client)result.AsyncState;
         var socket = client.Socket;

         if(socket != null)
         {
            try
            {
               var count = socket.EndReceive(result);

               if(count > 0)
               {
                  client.Read(_buffer, count);

                  socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, ReceiveCallback, client);
               }
               else
               {
                  CloseClient(client);
               }
            }
            catch(SocketException)
            {
               CloseClient(client);
            }
            catch(ObjectDisposedException)
            {
            }
         }
      }

      void Proxy_GlowFromProviderReady(object sender, Proxy.GlowFromProviderReadyArgs e)
      {
         lock(_sync)
         {
            foreach(var client in _clients)
               client.Write(e.Root);
         }
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         lock(_sync)
         {
            foreach(var client in _clients)
               client.Dispose();

            _clients.Clear();
         }
      }
      #endregion
   }
}
