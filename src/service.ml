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

(* class nvim = object *)
(*   val handle = *)
(*     Lwt_process.open_process *)
(*       ("nvim", [|"nvim"; "--embed"; "-u"; "NONE"|]) *)

(*   val api_info = *)
(*     Lwt_process.pread ("nvim", [|"nvim"; "--api-info"|]) >>= fun s -> *)
(*     Lwt.return s *)

(*      (\* method write s = *\) *)
(*      (\*   Msgpack.MsgpackCore. *\) *)

(* end *)
