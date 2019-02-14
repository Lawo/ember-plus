// EmberLib.net -- .NET implementation of the Ember+ Protocol
//
// Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

module internal EmberLib.Glow.Formula.Ast


type expr =
    // --- Term
    | Plus of expr * expr
    | Minus of expr * expr
    // --- Product
    | Times of expr * expr
    | Divide of expr * expr
    | IntegerDivide of expr * expr
    | Modulo of expr * expr
    // --- Molecule
    | Negate of expr
    // --- Power
    | Pow of expr * expr
    // --- Atom
    | It
    | Double of double
    | Integer of int64
    | Sin of expr
    | Cos of expr
    | Tan of expr
    | Asin of expr
    | Acos of expr
    | Atan of expr
    | Atan2 of expr * expr
    | Sinh of expr
    | Cosh of expr
    | Tanh of expr
    | Sqrt of expr
    | Pi
    | E
    | Log of expr
    | LogTo of expr * expr
    | Exp of expr
    | Round of expr
    | Ceiling of expr
    | AsInteger of expr
    | AsFloat of expr
    | Abs of expr
    | Sgn of expr
