// EmberLib.net -- .NET implementation of the Ember+ Protocol
// Copyright (C) 2012  L-S-B Broadcast Technologies GmbH
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

module internal EmberLib.Glow.Formula.Emitter

#if NO_EMIT
#else
open System
open System.Reflection.Emit
open Ast

[<AutoOpen>]
module private impl =
    type dataType =
        | Integer
        | Double

    let convInteger (il : ILGenerator) typ =
        match typ with
        | Integer -> ()
        | Double ->
            il.Emit(OpCodes.Conv_I8)


    let convDouble (il : ILGenerator) typ =
        match typ with
        | Double -> ()
        | Integer ->
            il.Emit(OpCodes.Conv_R8)


    let rec determineType itType expr =
        match expr with
        | Plus(left, right) ->
            match determineType itType left, determineType itType right with
            | Integer, Integer -> Integer
            | _ -> Double
        | Minus(left, right) ->
            match determineType itType left, determineType itType right with
            | Integer, Integer -> Integer
            | _ -> Double
        | Times(left, right) ->
            match determineType itType left, determineType itType right with
            | Integer, Integer -> Integer
            | _ -> Double
        | Divide(left, right) -> Double
        | IntegerDivide(left, right) -> Integer
        | Modulo(left, right) -> Integer
        | Negate expr -> determineType itType expr
        | Pow(left, right) -> Double
        | It -> itType
        | expr.Double d -> Double
        | expr.Integer n -> Integer
        | Sin expr -> Double
        | Cos expr -> Double
        | Tan expr -> Double
        | Asin expr -> Double
        | Acos expr -> Double
        | Atan expr -> Double
        | Atan2(expr1, expr2) -> Double
        | Sinh expr -> Double
        | Cosh expr -> Double
        | Tanh expr -> Double
        | Sqrt expr -> Double
        | Pi -> Double
        | E -> Double
        | Log expr -> Double
        | LogTo(expr1, expr2) -> Double
        | Exp expr -> Double
        | Round expr -> Integer
        | Ceiling expr -> Integer
        | AsInteger expr -> Integer
        | AsFloat expr -> Double
        | Abs expr -> determineType itType expr
        | Sgn expr -> Integer


    let rec walk itType (il : ILGenerator) expr =
        let convDouble = convDouble il
        let convInteger = convInteger il
        let mathMethod (argTypes : Type[]) (name : string) =
            typeof<Math>.GetMethod(name, argTypes)
        let mathMethod1 = mathMethod [| typeof<Double> |]
        let mathMethod2 = mathMethod [| typeof<Double>; typeof<Double> |]
        match expr with
        | Plus(left, right) ->
            match walk itType il left, walk itType il right with
            | Integer, Integer ->
                il.Emit(OpCodes.Add)
                Integer
            | Double, Double ->
                il.Emit(OpCodes.Add)
                Double
            | _ ->
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Add)
                Double
        | Minus(left, right) ->
            match walk itType il left, walk itType il right with
            | Integer, Integer ->
                il.Emit(OpCodes.Sub)
                Integer
            | Double, Double ->
                il.Emit(OpCodes.Sub)
                Double
            | _ ->
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Sub)
                Double
        | Times(left, right) ->
            match walk itType il left, walk itType il right with
            | Integer, Integer ->
                il.Emit(OpCodes.Mul)
                Integer
            | Double, Double ->
                il.Emit(OpCodes.Mul)
                Double
            | _ ->
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Mul)
                Double
        | Divide(left, right) ->
            left |> walk itType il |> convDouble
            right |> walk itType il |> convDouble
            il.Emit(OpCodes.Div)
            Double
        | IntegerDivide(left, right) ->
            left |> walk itType il |> convInteger
            right |> walk itType il |> convInteger
            il.Emit(OpCodes.Div)
            Integer
        | Modulo(left, right) ->
            left |> walk itType il |> convInteger
            right |> walk itType il |> convInteger
            il.Emit(OpCodes.Rem)
            Integer
        | Negate expr ->
            let res = walk itType il expr
            il.Emit(OpCodes.Neg)
            res
        | Pow(left, right) ->
            match walk itType il left, walk itType il right with
            | Double, Double -> ()
            | _ ->
                il.Emit(OpCodes.Conv_R8)
                il.Emit(OpCodes.Conv_R8)
            il.Emit(OpCodes.Call, mathMethod2 "Pow")
            Double
        | It ->
            il.Emit(OpCodes.Ldarg_0)
            itType
        | expr.Double d ->
            il.Emit(OpCodes.Ldc_R8, d)
            Double
        | expr.Integer n ->
            il.Emit(OpCodes.Ldc_I8, n)
            Integer
        | Sin expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Sin")
            Double
        | Cos expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Cos")
            Double
        | Tan expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Tan")
            Double
        | Asin expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Asin")
            Double
        | Acos expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Acos")
            Double
        | Atan expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Atan")
            Double
        | Atan2(expr1, expr2) ->
            expr1 |> walk itType il |> convDouble
            expr2 |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod2 "Atan2")
            Double
        | Sinh expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Sinh")
            Double
        | Cosh expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Cosh")
            Double
        | Tanh expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Tanh")
            Double
        | Sqrt expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Sqrt")
            Double
        | Pi ->
            il.Emit(OpCodes.Ldc_R8, Math.PI)
            Double
        | E ->
            il.Emit(OpCodes.Ldc_R8, Math.E)
            Double
        | Log expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Log")
            Double
        | LogTo(expr1, expr2) ->
            expr1 |> walk itType il |> convDouble
            expr2 |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod2 "Log")
            Double
        | Exp expr ->
            expr |> walk itType il |> convDouble
            il.Emit(OpCodes.Call, mathMethod1 "Exp")
            Double
        | Round expr ->
            match walk itType il expr with
            | Integer -> ()
            | Double ->
                il.Emit(OpCodes.Call, mathMethod1 "Round")
                il.Emit(OpCodes.Conv_I8)
            Integer
        | Ceiling expr ->
            match walk itType il expr with
            | Integer -> ()
            | Double ->
                il.Emit(OpCodes.Call, mathMethod1 "Ceiling")
                il.Emit(OpCodes.Conv_I8)
            Integer
        | AsInteger expr ->
            match walk itType il expr with
            | Integer -> ()
            | Double ->
                il.Emit(OpCodes.Call, mathMethod1 "Truncate")
                il.Emit(OpCodes.Conv_I8)
            Integer
        | AsFloat expr ->
            match walk itType il expr with
            | Double -> ()
            | Integer ->
                il.Emit(OpCodes.Conv_R8)
            Double
        | Abs expr ->
            match walk itType il expr with
            | Integer ->
                il.Emit(OpCodes.Call, mathMethod [| typeof<int64> |] "Abs")
                Integer
            | Double ->
                il.Emit(OpCodes.Call, mathMethod [| typeof<float> |] "Abs")
                Double
        | Sgn expr ->
            match walk itType il expr with
            | Integer ->
                il.Emit(OpCodes.Call, mathMethod [| typeof<int64> |] "Sign")
            | Double ->
                il.Emit(OpCodes.Call, mathMethod [| typeof<float> |] "Sign")
            Integer


type doubleToDoubleDel = delegate of float -> float
type doubleToIntegerDel = delegate of float -> int64
type integerToDoubleDel = delegate of int64 -> float
type integerToIntegerDel = delegate of int64 -> int64


type doubleResult =
    | DoubleToDouble of doubleToDoubleDel
    | DoubleToInteger of doubleToIntegerDel


type integerResult =
    | IntegerToDouble of integerToDoubleDel
    | IntegerToInteger of integerToIntegerDel


let emit expr =
    let netModule = typeof<expr>.Module

    let doubleMethod =
        let netType =
            match determineType Double expr with
            | Double -> typeof<Double>
            | Integer -> typeof<Int64>
        let temp = new DynamicMethod("evalFormulaDouble", netType, [| typeof<Double> |], netModule)
        let il = temp.GetILGenerator(256)
        let res = walk Double il expr
        il.Emit(OpCodes.Ret)
        match res with
        | Double -> DoubleToDouble(temp.CreateDelegate(typeof<doubleToDoubleDel>) :?> doubleToDoubleDel)
        | Integer -> DoubleToInteger(temp.CreateDelegate(typeof<doubleToIntegerDel>) :?> doubleToIntegerDel)

    let integerMethod =
        let netType =
            match determineType Integer expr with
            | Double -> typeof<Double>
            | Integer -> typeof<Int64>
        let temp = new DynamicMethod("evalFormulaInteger", netType, [| typeof<Int64> |], netModule)
        let il = temp.GetILGenerator(256)
        let res = walk Integer il expr
        il.Emit(OpCodes.Ret)
        match res with
        | Double -> IntegerToDouble(temp.CreateDelegate(typeof<integerToDoubleDel>) :?> integerToDoubleDel)
        | Integer -> IntegerToInteger(temp.CreateDelegate(typeof<integerToIntegerDel>) :?> integerToIntegerDel)

    doubleMethod, integerMethod
#endif
