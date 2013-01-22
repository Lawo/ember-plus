using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Framing;
using EmberLib.Glow.Framing;

namespace EmberPlusProxy
{
   class Program
   {
      static void Main(string[] args)
      {
         int port;
         int maxPackageLength;

         var settings = Settings.Load();

         ParseArgs(args, out port, out maxPackageLength);

         Console.WriteLine("Ember+ Proxy v{0} (GlowDTD v{1} - EmBER v{2}) started.",
                           typeof(Program).Assembly.GetName().Version,
                           GlowReader.UshortVersionToString(EmberLib.Glow.GlowDtd.Version),
                           GlowReader.UshortVersionToString(EmberLib.EmberEncoding.Version));

         using(var proxy = new Proxy(settings.EndPoints, maxPackageLength))
         {
            proxy.Connect();

            using(var listener = new Provider.GlowListener(port, maxPackageLength, proxy))
            {
               Console.WriteLine("Listening on port {0}. Press enter to quit...", port);
               Console.ReadLine();
            }
         }
      }

      #region Implementation
      /// <summary>
      /// Parses the command line arguments and fills some out variables
      /// with the parsed information.
      /// </summary>
      /// <param name="args">Command line arguments as passed to the entry point method.</param>
      /// <param name="hostName">Receives the host name to connect to.</param>
      /// <param name="tcpPort">Receives the port number to connect to.</param>
      /// <param name="maxPackageLength">Receives the maximum package length for
      /// tx packages.</param>
      static void ParseArgs(string[] args, out int tcpPort, out int maxPackageLength)
      {
         tcpPort = 9090;
         maxPackageLength = ProtocolParameters.MaximumPackageLength;

         var argTokens = from arg in args
                         where arg.StartsWith("-") || arg.StartsWith("/")
                         let tokens = arg.Split('=')
                         where tokens.Length == 2
                         select Tuple.Create(tokens[0].ToLower().TrimStart('-', '/'), tokens[1]);

         foreach(var token in argTokens)
         {
            switch(token.Item1)
            {
               case "port":
                  Int32.TryParse(token.Item2, out tcpPort);
                  break;
               case "maxpackagelength":
                  Int32.TryParse(token.Item2, out maxPackageLength);
                  break;
            }
         }
      }
      #endregion
   }
}
