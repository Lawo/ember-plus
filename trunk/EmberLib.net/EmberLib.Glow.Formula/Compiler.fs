// EmberLib.net -- .NET implementation of the Ember+ Protocol
// Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

namespace EmberLib.Glow.Formula

open System
open System.Reflection
open System.Collections.Generic
open System.Text
open Microsoft.FSharp.Text.Lexing
open EmberLib.Glow
open Ast
open Lexer
open Parser


/// Contains a compiled formula that can be evaluated
[<AllowNullLiteral>]
type ICompiledFormula =
    /// Evaluates the formula, using the passed System.Double argument
    /// as the "it" value.
    /// Returns a GlowValue containing the result of the computation.
    abstract Eval : float -> GlowValue

    /// Evaluates the formula, using the passed System.Int64 argument
    /// as the "it" value.
    /// Returns a GlowValue containing the result of the computation.
    abstract Eval : int64 -> GlowValue


/// Contains the result of a call to Compiler.Compile.
/// If ICompilationResult.Success is true, the ICompilationResult.Formula
/// property returns an instance of ICompiledFormula.
/// Otherwise, the Error property can be used to examine the compilation
/// error.
type ICompilationResult =
    /// Gets a value indicating whether compilation was successful or not.
    abstract Success : bool

    /// Gets the compiled formula if compilation was successful.
    abstract Formula : ICompiledFormula

    /// Gets an exception describing the error encountered during compilation.
    abstract Error : exn


/// Static formula compiler.
module Compiler =
    type private result<'TSuccess, 'TError> =
        | Success of 'TSuccess
        | Error of 'TError

    let private parse lexbuf =
        try Success(Parser.start Lexer.token lexbuf)
        with ex -> Error(lexbuf.EndPos, ex)

    let private createInterpretedFormula expr =
        let valueToGlowValue (v : Interpreter.value) =
            match v with
            | Interpreter.Integer n -> new GlowValue(n)
            | Interpreter.Double d -> new GlowValue(d)

        { new ICompiledFormula with
            member f.Eval (value : float) =
                let result = Interpreter.interpret (Interpreter.Double value) expr
                valueToGlowValue result
            member f.Eval (value : int64) =
                let result = Interpreter.interpret (Interpreter.Integer value) expr
                valueToGlowValue result }
#if NO_EMIT
#else
    let private createIlFormula expr =
        let doubleMethod, integerMethod = Emitter.emit expr
        { new ICompiledFormula with
            member f.Eval (value : float) =
                match doubleMethod with
                | Emitter.DoubleToDouble del -> new GlowValue(del.Invoke(value))
                | Emitter.DoubleToInteger del -> new GlowValue(del.Invoke(value))
            member f.Eval (value : int64) =
                match integerMethod with
                | Emitter.IntegerToDouble del -> new GlowValue(del.Invoke(value))
                | Emitter.IntegerToInteger del -> new GlowValue(del.Invoke(value)) }
#endif

    /// <summary>
    /// Compiles the passed formula source and returns an instance
    /// of ICompilationResult.
    /// </summary>
    /// <param name="source">The formula source code</param>
    /// <param name="emitIlFlag">If true, the formula is compiled to IL.
    /// This yields a speedup between 4 an 20 for formula evaluation but
    /// compilation takes about twice as long.</param>
    let Compile(source : string, emitIlFlag : bool) =
        let lexbuf = LexBuffer<char>.FromString(source)
        match parse lexbuf with
        | Success expr ->
            let formula =
#if NO_EMIT
#else
                if emitIlFlag then createIlFormula expr
                else
#endif
                    createInterpretedFormula expr
            { new ICompilationResult with
                member this.Success = true
                member this.Error = null
                member this.Formula = formula }
        | Error(pos, ex) ->
            { new ICompilationResult with
                member this.Success = false
                member this.Error = ex
                member this.Formula = null }
