using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EmberLib.Glow;

namespace EmberPlusRouter.Model
{
   interface IDynamicPathHandler
   {
      void HandleParameter(GlowParameterBase parameter, int[] path, Client source);
      void HandleCommand(GlowCommand command, int[] path, Client source);
   }
}
