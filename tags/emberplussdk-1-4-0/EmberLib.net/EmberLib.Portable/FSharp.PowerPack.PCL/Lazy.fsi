// (c) Microsoft Corporation 2005-2009.  

[<CompilationRepresentation(CompilationRepresentationFlags.ModuleSuffix)>]
module Microsoft.FSharp.Core.Lazy

/// See Lazy.Force
val force: Lazy<'T> -> 'T
