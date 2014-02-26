namespace Microsoft.FSharp
open System.Reflection
[<assembly:AssemblyDescription("FSharp.PowerPack.dll")>]
[<assembly:AssemblyCompany("F# PowerPack CodePlex Project")>]
[<assembly:AssemblyTitle("FSharp.PowerPack.dll")>]
[<assembly:AssemblyProduct("F# Power Pack")>]
[<assembly: AutoOpen("Microsoft.FSharp.Text")>]
[<assembly: AutoOpen("Microsoft.FSharp.Control")>]
[<assembly: AutoOpen("Microsoft.FSharp.Collections")>]
[<assembly: AutoOpen("Microsoft.FSharp.Core")>]
[<assembly: AutoOpen("Microsoft.FSharp.Math")>]
[<assembly: AutoOpen("Microsoft.FSharp")>]
[<assembly: System.CLSCompliant(true)>]
//[<assembly: System.Security.SecurityTransparent>]
#if FX_NO_SECURITY_PERMISSIONS
#else
#if FX_SIMPLE_SECURITY_PERMISSIONS
[<assembly: System.Security.Permissions.SecurityPermission(System.Security.Permissions.SecurityAction.RequestMinimum)>]
#else
#endif
#endif
do()


