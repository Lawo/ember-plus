#r "bin/Debug/BerLib.dll"

open BerLib
open System.Text

let tags =
  [ new BerTag(BerClass.Application, 1u, isContainer = false);
    new BerTag(BerClass.Universal, 3u, isContainer = false);
    new BerTag(BerClass.Application, 3u, isContainer = false);
    new BerTag(BerClass.Universal, 2u, isContainer = false);
    new BerTag(BerClass.Private, 111u, isContainer = true);
    new BerTag(BerClass.Universal, 3u, isContainer = true);
    new BerTag(BerClass.ContextSpecific, 2u, isContainer = false);
    new BerTag(BerClass.Application, 2u, isContainer = false);
    new BerTag(BerClass.Universal, 2u, isContainer = true);
    new BerTag(BerClass.Universal, 1u, isContainer = true);
    new BerTag(BerClass.ContextSpecific, 3u, isContainer = false);
    new BerTag(BerClass.Universal, 1u, isContainer = false);
    new BerTag(BerClass.ContextSpecific, 1u, isContainer = true);
    new BerTag(BerClass.Private, 333u, isContainer = false);
    new BerTag(BerClass.Private, 222u, isContainer = true); ]

tags
|> List.sort
|> List.iter (fun tag -> printfn "%O" tag)


do
    [0 .. 9]
    |> List.iter (fun exp ->
        let n = pown 10 exp
        let berLength = BerEncoding.GetIntegerLength(n)
        let utf8Length =
            let text = n.ToString()
            Encoding.UTF8.GetByteCount(text)
        printfn "Length of '%10d': BER=%d UTF8=%d" n berLength utf8Length)
