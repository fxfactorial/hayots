open Lwt.Infix

class merlin = object
  val handle =
    Lwt_process.open_process
      ("opam", [|"opam"; "config";"exec"; "--"; "ocamlmerlin"|])
end

class ocamldebug binary_path = object
  val handle =
    Lwt_process.open_process
      ("opam", [|"opam"; "config";"exec"; "--";"ocamldebug"; binary_path|])
end

class utop = object
  val handle =
    Lwt_process.open_process ("opam", [|"opam"; "config";"exec"; "--"; "utop"|])

  method read_all =
    Lwt_io.read_lines handle#stdout
    |> Lwt_stream.iter_s (fun s -> Lwt_io.printlf "%s" s)

end
