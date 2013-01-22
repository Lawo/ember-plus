// (c) Microsoft Corporation 2005-2009.  

[<CompilationRepresentation(CompilationRepresentationFlags.ModuleSuffix)>]
module Microsoft.FSharp.Core.Lazy

let force (x: Lazy<'T>) = x.Force()
