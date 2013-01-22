using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

namespace EmberPlusSimpleProvider
{
   class Client : IDisposable
   {
      public Client(Socket socket, Program program)
      {
         _socket = socket;
         _program = program;

         socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, ReceiveCallback, null);
      }

      public void Close()
      {
         if(_socket != null)
         {
            try { _socket.Shutdown(SocketShutdown.Both); }
            catch { }

            try { _socket.Close(); }
            catch { }
         }

         _socket = null;
      }

      #region Implementation
      Socket _socket;
      Program _program;
      byte[] _buffer = new byte[1024];

      void ReceiveCallback(IAsyncResult result)
      {
         bool continueFlag = false;

         try
         {
            var read = _socket.EndReceive(result);

            if(read > 0)
            {
               Console.WriteLine("{0} bytes received", read);
               continueFlag = true;
            }
         }
         catch
         {
         }

         if(continueFlag)
            _socket.BeginReceive(_buffer, 0, _buffer.Length, SocketFlags.None, ReceiveCallback, null);
         else
            _program.RemoveClient(this);
      }
      #endregion

      #region IDisposable Members
      void IDisposable.Dispose()
      {
         Close();
      }
      #endregion
   }
}
