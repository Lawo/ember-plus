using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Framing;
using EmberLib.Glow.Framing;

namespace EmberPlusRouter
{
   class Program
   {
      static void Main(string[] args)
      {
         int port;
         int maxPackageLength;

         ParseArgs(args, out port, out maxPackageLength);

         Console.WriteLine("Ember+ Router v{0} (GlowDTD v{1} - EmBER v{2}) started.",
                           typeof(Program).Assembly.GetName().Version,
                           GlowReader.UshortVersionToString(EmberLib.Glow.GlowDtd.Version),
                           GlowReader.UshortVersionToString(EmberLib.EmberEncoding.Version));

         var dispatcher = new Dispatcher();
         dispatcher.Root = CreateTree(dispatcher);

         using(var listener = new GlowListener(port, maxPackageLength, dispatcher))
         {
            Console.WriteLine("Listening on port {0}. Press Enter to quit...", port);
            Console.ReadLine();
         }
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
      static void ParseArgs(string[] args, out int tcpPort, out int maxPackageLength)
      {
         tcpPort = 9098;
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

      static Model.Node CreateTree(Dispatcher dispatcher)
      {
         var root = Model.Node.CreateRoot();
         var router = new Model.Node(1, root, "router");

         CreateOneToN(router, 1, dispatcher);
         CreateNToN(router, 2, dispatcher);
         CreateDynamic(router, 3, dispatcher);
         CreateSparse(router, 4, dispatcher);
         CreateOneToOne(router, 5, dispatcher);

         return root;
      }

      static string IdentOf(string baseStr, int n)
      {
         return baseStr + "-" + n;
      }

      static void CreateOneToN(Model.Node router, int nodeNumber, Dispatcher dispatcher)
      {
         var oneToN = new Model.Node(nodeNumber, router, "oneToN")
         {
            Description = "Linear 1:N",
         };

         var labels = new Model.Node(1, oneToN, "labels");
         var targetLabels = new Model.Node(1, labels, "targets");
         var sourceLabels = new Model.Node(2, labels, "sources");

         var targets = new List<Model.Signal>();
         var sources = new List<Model.Signal>();

         for(int number = 0; number < 200; number++)
         {
            var targetParameter = new Model.StringParameter(number, targetLabels, IdentOf("t", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("SDI-T", number),
            };

            targets.Add(new Model.Signal(number, targetParameter));

            var sourceParameter = new Model.StringParameter(number, sourceLabels, IdentOf("s", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("SDI-S", number),
            };

            sources.Add(new Model.Signal(number, sourceParameter));
         }

         var matrix = new Model.OneToNMatrix(
            2,
            oneToN,
            "matrix",
            dispatcher,
            targets,
            sources,
            labels);

         foreach(var target in matrix.Targets)
            matrix.Connect(target, new[] { matrix.GetSource(target.Number) }, null);
      }

      static void CreateNToN(Model.Node router, int nodeNumber, Dispatcher dispatcher)
      {
         var nToN = new Model.Node(nodeNumber, router, "nToN")
         {
            Description = "Non-Linear N:N",
         };

         var labels = new Model.Node(1, nToN, "labels");
         var targetLabels = new Model.Node(1, labels, "targets");
         var sourceLabels = new Model.Node(2, labels, "sources");

         var parameters = new Model.Node(2, nToN, "parameters");
         var targetParams = new Model.Node(1, parameters, "targets");
         var sourceParams = new Model.Node(2, parameters, "sources");
         var xpointParams = new Model.Node(3, parameters, "connections");

         var targets = new List<Model.Signal>();
         var sources = new List<Model.Signal>();
         var number = 0;

         for(int index = 0; index < 4; index++)
         {
            number += 3;

            var targetLabel = new Model.StringParameter(number, targetLabels, IdentOf("t", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("AES-T", number)
            };
            targets.Add(new Model.Signal(number, targetLabel));
            var targetNode = new Model.Node(number, targetParams, IdentOf("t", number));
            new Model.IntegerParameter(1, targetNode, "targetGain", dispatcher, -128, 15, isWriteable: true);
            new Model.StringParameter(2, targetNode, "targetMode", dispatcher, isWriteable: true) { Value = "something" };

            var sourceLabel = new Model.StringParameter(number, sourceLabels, IdentOf("s", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("AES-S", number)
            };
            sources.Add(new Model.Signal(number, sourceLabel));
            var sourceNode = new Model.Node(number, sourceParams, IdentOf("s", number));
            new Model.IntegerParameter(1, sourceNode, "sourceGain", dispatcher, -128, 15, isWriteable: true);
         }

         foreach(var target in targets)
         {
            var targetNode = new Model.Node(target.Number, xpointParams, IdentOf("t", target.Number));

            foreach(var source in sources)
            {
               var sourceNode = new Model.Node(source.Number, targetNode, IdentOf("s", source.Number));
               new Model.IntegerParameter(1, sourceNode, "gain", dispatcher, -128, 15, isWriteable: true);
            }
         }

         var matrix = new Model.NToNMatrix(
            3,
            nToN,
            "matrix",
            dispatcher,
            targets,
            sources,
            labels,
            parameters);

         foreach(var target in matrix.Targets)
         {
            if(target.Number % 2 == 0)
               matrix.Connect(target, new[] { matrix.GetSource(target.Number) }, null);
         }
      }

      static void CreateDynamic(Model.Node router, int nodeNumber, Dispatcher dispatcher)
      {
         var dyna = new Model.Node(nodeNumber, router, "dynamic")
         {
            Description = "Linear N:N with Dynamic Parameters",
         };

         var labels = new Model.Node(1, dyna, "labels");
         var targetLabels = new Model.Node(1, labels, "targets");
         var sourceLabels = new Model.Node(2, labels, "sources");

         var targets = new List<Model.Signal>();
         var sources = new List<Model.Signal>();

         for(int index = 0; index < 1000; index++)
         {
            var targetLabel = new Model.StringParameter(index, targetLabels, IdentOf("t", index), dispatcher, isWriteable: true)
            {
               Value = IdentOf("DYN-T", index)
            };
            targets.Add(new Model.Signal(index, targetLabel));

            var sourceLabel = new Model.StringParameter(index, sourceLabels, IdentOf("s", index), dispatcher, isWriteable: true)
            {
               Value = IdentOf("DYN-S", index)
            };
            sources.Add(new Model.Signal(index, sourceLabel));
         }

         var matrix = new Model.DynamicMatrix(
            3,
            dyna,
            "matrix",
            dispatcher,
            targets,
            sources,
            labels);

         foreach(var target in matrix.Targets)
            matrix.Connect(target, new[] { matrix.GetSource(target.Number) }, null);
            //matrix.Connect(target, matrix.Sources, null);
      }

      static void CreateSparse(Model.Node router, int nodeNumber, Dispatcher dispatcher)
      {
         var sparse = new Model.Node(nodeNumber, router, "sparse")
         {
            Description = "Linear 1:N with Sparse Signals",
         };

         var labels = new Model.Node(1, sparse, "labels");
         var targetLabels = new Model.Node(1, labels, "targets");
         var sourceLabels = new Model.Node(2, labels, "sources");

         var targets = new List<Model.Signal>();
         var sources = new List<Model.Signal>();

         for(int number = 3; number < 200; number += 2)
         {
            var targetParameter = new Model.StringParameter(number, targetLabels, IdentOf("t", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("Sparse-T", number),
            };

            targets.Add(new Model.Signal(number, targetParameter));

            var sourceParameter = new Model.StringParameter(number, sourceLabels, IdentOf("s", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("Sparse-S", number),
            };

            sources.Add(new Model.Signal(number, sourceParameter));
         }

         var matrix = new Model.OneToNMatrix(
            2,
            sparse,
            "matrix",
            dispatcher,
            targets,
            sources,
            labels,
            targetCount: 1000,
            sourceCount: 1000);

         foreach(var target in matrix.Targets)
            matrix.Connect(target, new[] { matrix.GetSource(target.Number) }, null);
      }

      static void CreateOneToOne(Model.Node router, int nodeNumber, Dispatcher dispatcher)
      {
         var oneToOne = new Model.Node(nodeNumber, router, "oneToOne")
         {
            Description = "Linear 1:1",
         };

         var labels = new Model.Node(1, oneToOne, "labels");
         var targetLabels = new Model.Node(1, labels, "targets");
         var sourceLabels = new Model.Node(2, labels, "sources");

         var targets = new List<Model.Signal>();
         var sources = new List<Model.Signal>();

         for(int number = 0; number < 16; number++)
         {
            var targetParameter = new Model.StringParameter(number, targetLabels, IdentOf("t", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("OneToOne-T", number),
            };

            targets.Add(new Model.Signal(number, targetParameter));

            var sourceParameter = new Model.StringParameter(number, sourceLabels, IdentOf("s", number), dispatcher, isWriteable: true)
            {
               Value = IdentOf("OneToOne-S", number),
            };

            sources.Add(new Model.Signal(number, sourceParameter));
         }

         var matrix = new Model.OneToOneMatrix(
            2,
            oneToOne,
            "matrix",
            dispatcher,
            targets,
            sources,
            labels);

         foreach(var target in matrix.Targets)
            matrix.Connect(target, new[] { matrix.GetSource(target.Number) }, null);
      }
   }
}
