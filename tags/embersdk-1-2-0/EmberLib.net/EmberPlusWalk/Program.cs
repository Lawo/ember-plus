using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using EmberLib.Glow;
using System.Globalization;
using System.Diagnostics;
using System.IO;
using EmberLib.Framing;
using EmberLib.Glow.Framing;

namespace EmberPlusWalk
{
   /// <summary>
   /// Contains main application logic
   /// </summary>
   class Program
   {
      /// <summary>
      /// Constructs the single instance of the Program class.
      /// </summary>
      /// <param name="endPoint">Connected GlowEndPoint.</param>
      Program(GlowEndPoint endPoint)
      {
         _endPoint = endPoint;

         _rootElement = Element.CreateRoot();
         _cursor = _rootElement;

         _endPoint.GlowRootReady +=
            (_, e) =>
            {
               if(e.Root.Accept(_rootElement, null))
               {
                  Console.WriteLine();
                  foreach(var elem in _cursor.Children)
                     Console.WriteLine(PrintElement(elem));

                  WritePrompt();
               }
            };

         _endPoint.Notification +=
            (_, e) =>
            {
               Debug.Print(e.Message);
            };
      }

      /// <summary>
      /// Entry point
      /// </summary>
      /// <param name="args">Command line arguments</param>
      static void Main(string[] args)
      {
         string hostName;
         int tcpPort;
         int maxPackageLength;

         ParseArgs(args, out hostName, out tcpPort, out maxPackageLength);

         var endPoint = new GlowEndPoint(hostName, tcpPort, maxPackageLength);

         Console.WriteLine("Ember+ Walker v{0} (GlowDTD v{1} - EmBER v{2}) connecting...",
                           typeof(Program).Assembly.GetName().Version,
                           GlowReader.UshortVersionToString(EmberLib.Glow.GlowDtd.Version),
                           GlowReader.UshortVersionToString(EmberLib.EmberEncoding.Version));

         if(endPoint.Connect())
         {
            Console.WriteLine("Connected. Enter '?' for help.");

            var program = new Program(endPoint);

            program.Run();
         }

         endPoint.Close();
      }

      #region Implementation
      GlowEndPoint _endPoint;
      Element _rootElement;
      Element _cursor;

      /// <summary>
      /// Runs the application loop: read a line of input from stdin,
      /// and execute the command matching the input.
      /// </summary>
      void Run()
      {
         // write initial "dir" command to get the root's children.
         _endPoint.Write(_rootElement.GetDirectory());
         WriteWaitingPrompt();

         string input;

         while((input = Console.ReadLine()) != "quit")
         {
            var match = null as Match;

            if(input == "dir")
            {
               _endPoint.Write(_cursor.GetDirectory());

               WriteWaitingPrompt();
            }
            else if(input == "cd..")
            {
               if(_cursor.Parent != null)
                  _cursor = _cursor.Parent;

               WritePrompt();
            }
            else if((match = Regex.Match(input, @"^cd (.*)\r?$")).Success)
            {
               var elem = GetElement(match.Groups[1].Value);

               if(elem != null)
                  _cursor = elem;

               WritePrompt();
            }
            else if((match = Regex.Match(input, @"^print (.*)\r?$")).Success)
            {
               var elem = GetElement(match.Groups[1].Value);

               if(elem != null)
                  Console.Write(elem.Xml);

               WritePrompt();
            }
            else if((match = Regex.Match(input, @"^set (.*)\s?'(.*)'\r?$")).Success)
            {
               var elem = GetElement(match.Groups[1].Value.Trim());
               var isOk = false;

               if(elem != null
               && elem.Type == ElementType.Parameter)
               {
                  GlowValue value;

                  if(GlowValue.TryParse(
                     match.Groups[2].Value,
                     elem.ParameterType,
                     CultureInfo.CurrentCulture,
                     out value))
                  {
                     _endPoint.Write(elem.SetParameterValue(value));
                     WriteWaitingPrompt();
                     isOk = true;
                  }
               }

               if(isOk == false)
                  WritePrompt();
            }
            else if(input == "?" || input == "help")
            {
               Console.WriteLine("cd <id>        - change to node with identifier <id>");
               Console.WriteLine("cd..           - change to current parent node");
               Console.WriteLine("dir            - list children of current node");
               Console.WriteLine("print <id>     - print node with identifier <id>");
               Console.WriteLine("set <id> '<v>' - set value of parameter with identifier <id> to <v>");
               Console.WriteLine("quit           - exit program");
               Console.WriteLine();
               Console.WriteLine("for <id> you can also write '.' for current node");

               WritePrompt();
            }
            else if(String.IsNullOrEmpty(input) == false)
            {
               Console.WriteLine("Unknown command");
               WritePrompt();
            }
            else
            {
               WritePrompt();
            }
         }
      }

      /// <summary>
      /// Gets the current cursor's child with the specified identifier
      /// or the cursor itself.
      /// </summary>
      /// <param name="ident">The identifier of the element to get or "." to get
      /// the current cursor.</param>
      /// <returns>The found element or null.</returns>
      Element GetElement(string ident)
      {
         if(ident == ".")
            return _cursor;

         return (from elem in _cursor.Children
                 where String.Compare(elem.Identifier, ident, ignoreCase: true) == 0
                 select elem)
                 .FirstOrDefault();
      }

      /// <summary>
      /// Writes out an input prompt to the console, including the current cursor's path.
      /// </summary>
      void WritePrompt()
      {
         var elem = _cursor;
         var idents = new LinkedList<string>();

         while(elem.Type != ElementType.Root)
         {
            idents.AddFirst(elem.Identifier);
            elem = elem.Parent;
         }

         var buffer = new StringBuilder();
         var index = 0;
         foreach(var ident in idents)
         {
            if(index > 0)
               buffer.Append("/");

            buffer.Append(ident);
            index++;
         }

         Console.Write("{0}:{1}/{2}> ", _endPoint.HostName, _endPoint.TcpPort, buffer);
      }

      /// <summary>
      /// Writes out a prompt to the console indicating that an async operation has been started.
      /// </summary>
      void WriteWaitingPrompt()
      {
         Console.Write("|>");
      }

      /// <summary>
      /// Writes out information about an element to a string.
      /// Used for listing the current cursor's children.
      /// </summary>
      /// <param name="elem">The element to print.</param>
      /// <returns>A string containing information about the passed element.</returns>
      static string PrintElement(Element elem)
      {
         string type;

         switch(elem.Type)
         {
            case ElementType.Node:
               type = "N";
               break;

            case ElementType.Parameter:
               type = "P";
               break;

            default:
               type = String.Empty;
               break;
         }

         return String.Format("{0} {1:000} {2}", type, elem.Number, elem.Identifier);
      }

      /// <summary>
      /// Parses the command line arguments and fills some out variables
      /// with the parsed information.
      /// </summary>
      /// <param name="args">Command line arguments as passed to the entry point method.</param>
      /// <param name="hostName">Receives the host name to connect to.</param>
      /// <param name="tcpPort">Receives the port number to connect to.</param>
      /// <param name="maxPackageLength">Receives the maximum package length for
      /// tx packages.</param>
      static void ParseArgs(string[] args, out string hostName, out int tcpPort, out int maxPackageLength)
      {
         hostName = "localhost";
         tcpPort = 9000;
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
               case "host":
                  hostName = token.Item2;
                  break;
               case "port":
                  Int32.TryParse(token.Item2, out tcpPort);
                  break;
               case "maxpackagelength":
                  Int32.TryParse(token.Item2, out maxPackageLength);
                  break;
            }
         }

         if(maxPackageLength > ProtocolParameters.MaximumPackageLength)
            maxPackageLength = ProtocolParameters.MaximumPackageLength;
      }
      #endregion
   }
}
