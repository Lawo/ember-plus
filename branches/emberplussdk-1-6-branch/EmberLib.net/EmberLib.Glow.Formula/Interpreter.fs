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

module internal EmberLib.Glow.Formula.Interpreter

open System
open Ast

type value =
    | Double of double
    | Integer of int64

    member this.AsInteger =
        match this with
        | Integer n -> n
        | Double d  -> int64 d

    member this.AsDouble =
        match this with
        | Integer n -> float n
        | Double d  -> d


let rec interpret it expr =
    match expr with
    | Plus(left, right) ->
        match interpret it left, interpret it right with
        | Integer ln, Integer rn -> Integer(ln + rn)
        | l, r -> Double(l.AsDouble + r.AsDouble)
    | Minus(left, right) ->
        match interpret it left, interpret it right with
        | Integer ln, Integer rn -> Integer(ln - rn)
        | l, r -> Double(l.AsDouble - r.AsDouble)
    | Times(left, right) ->
        match interpret it left, interpret it right with
        | Integer ln, Integer rn -> Integer(ln * rn)
        | l, r -> Double(l.AsDouble * r.AsDouble)
    | Divide(left, right) ->
        Double <| (interpret it left).AsDouble / (interpret it right).AsDouble
    | IntegerDivide(left, right) ->
        Integer <| (interpret it left).AsInteger / (interpret it right).AsInteger
    | Modulo(left, right) ->
        Integer <| (interpret it left).AsInteger % (interpret it right).AsInteger
    | Negate expr ->
        match interpret it expr with
        | Double d -> Double(-d)
        | Integer n -> Integer(-n)
    | Pow(left, right) ->
        match interpret it left, interpret it right with
        | Integer ln, Integer rn when rn >= 0L -> Integer(pown ln (int rn))
        | l, r -> Double(l.AsDouble ** r.AsDouble)
    | It ->
        it
    | expr.Double d ->
        Double d
    | expr.Integer n ->
        Integer n
    | Sin expr ->
        Double <| sin (interpret it expr).AsDouble
    | Cos expr ->
        Double <| cos (interpret it expr).AsDouble
    | Tan expr ->
        Double <| tan (interpret it expr).AsDouble
    | Asin expr ->
        Double <| asin (interpret it expr).AsDouble
    | Acos expr ->
        Double <| acos (interpret it expr).AsDouble
    | Atan expr ->
        Double <| atan (interpret it expr).AsDouble
    | Atan2(expr1, expr2) ->
        Double <| atan2 (interpret it expr1).AsDouble (interpret it expr2).AsDouble
    | Sinh expr ->
        Double <| sinh (interpret it expr).AsDouble
    | Cosh expr ->
        Double <| cosh (interpret it expr).AsDouble
    | Tanh expr ->
        Double <| tanh (interpret it expr).AsDouble
    | Sqrt expr ->
        Double <| sqrt (interpret it expr).AsDouble
    | Pi ->
        Double Math.PI
    | E ->
        Double Math.E
    | Log expr ->
        Double <| log (interpret it expr).AsDouble
    | LogTo(expr1, expr2) ->
        Double <| Math.Log((interpret it expr1).AsDouble, (interpret it expr2).AsDouble)
    | Exp expr ->
        Double <| exp (interpret it expr).AsDouble
    | Round expr ->
        Integer <| int64 (round (interpret it expr).AsDouble)
    | Ceiling expr ->
        Integer <| int64 (ceil (interpret it expr).AsDouble)
    | AsInteger expr ->
        Integer (interpret it expr).AsInteger
    | AsFloat expr ->
        Double (interpret it expr).AsDouble
    | Abs expr ->
        match interpret it expr with
        | Double d -> Double <| Math.Abs(d)
        | Integer n -> Integer <| Math.Abs(n)
    | Sgn expr ->
        match interpret it expr with
        | Double d -> Math.Sign(d) |> int64 |> Integer
        | Integer n -> Math.Sign(n) |> int64 |> Integer
