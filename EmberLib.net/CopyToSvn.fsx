open System
open System.IO
open System.Text.RegularExpressions

let targetBaseDir = @"N:\MyProjects.svn\ember-sdk\Library\trunk\EmberLib.net\"


let isBinObjDir (path : string) =
    let path = path.ToLower()
    path.Contains(@"\bin\")
    || path.EndsWith(@"\bin")
    || path.Contains(@"\obj\")
    || path.EndsWith(@"\obj")


// copy new directories
Directory.EnumerateDirectories(".", "*", SearchOption.AllDirectories)
|> Seq.filter (fun dir ->
    not (isBinObjDir dir || dir.Contains(@"\_UpgradeReport_Files")))
|> Seq.map (fun dir -> dir.TrimStart('.', '\\'))
|> Seq.iter (fun dir ->
    let targetDir = targetBaseDir + dir
    if Directory.Exists(targetDir) = false then
        printfn "creating %s" targetDir
        Directory.CreateDirectory(targetDir) |> ignore)


// copy modified or new files
let newFiles =
    Directory.EnumerateFiles(".", "*", SearchOption.AllDirectories)
    |> Seq.filter (fun file ->
        not (isBinObjDir file || file.EndsWith(".scc") || file.EndsWith(".vspscc") || file.EndsWith(".suo") || file.EndsWith(".vssscc")))
    |> Seq.map (fun file -> file.TrimStart('.', '\\'))
    |> Seq.choose (fun file ->
        let targetFile = targetBaseDir + file
        let isCopyRequired =
            if Directory.Exists(Path.GetDirectoryName(targetFile))
            then File.Exists(targetFile) = false || File.GetLastWriteTime(file) > File.GetLastWriteTime(targetFile)
            else false
        if isCopyRequired then
            printfn "copying %s" file
            File.Copy(file, targetFile, overwrite = true)
            let attribs = File.GetAttributes(file) &&& ~~~FileAttributes.ReadOnly
            File.SetAttributes(targetFile, attribs)
            Some targetFile
        else None)
    |> Seq.toList


// remove scc bindings from project files
newFiles
|> Seq.filter (fun file ->
    file.EndsWith(".csproj") || file.EndsWith(".fsproj") || file.EndsWith(".wixproj"))
|> Seq.iter (fun file ->
    printfn "removing scc from %s" file
    let lines =
        File.ReadAllLines(file)
        |> Array.filter (fun line -> not (line.Contains("<Scc") && line.Contains("</Scc")))
    File.WriteAllLines(file, lines))


// remove scc bindings from solution files
newFiles
|> Seq.filter (fun file -> file.EndsWith(".sln"))
|> Seq.iter (fun file ->
    printfn "removing scc from %s" file
    let lines = File.ReadAllLines(file)
    let lines =
        let skip = ref false
        [ for line in lines do
            if !skip = false then
                if line.Contains(@"GlobalSection(SourceCodeControl)") || line.Contains(@"GlobalSection(TeamFoundationVersionControl)")
                then skip := true
                else yield line
            else
                if line.Contains("EndGlobalSection") then
                    skip := false ]
    File.WriteAllLines(file, lines))
