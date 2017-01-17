open Lwt.Infix

external init_ide : int -> unit = "hayots_ml_init_gui"

let send_message selector sock_handle =
  Lwt_unix.write
    sock_handle
    selector
    0
    (String.length selector) >>= fun wr ->
  Lwt.return_unit

let rec forever () =
  let _ = Lwt.wait () in
  forever ()

let () =
  let (parent_sock, gui_sock) =
    Lwt_unix.socketpair Lwt_unix.PF_UNIX Lwt_unix.SOCK_STREAM 0
  in
  (
    Lwt_io.flush_all () >>= fun () ->
    let pid = Lwt_unix.fork () in
    (* Odd inversion, Cocoa can only run in the master process *)
    match pid with
    (* Parent process *)
    | pid when pid > 0 ->
      (* Never returns *)
      init_ide (Obj.magic (Lwt_unix.unix_file_descr gui_sock))
      |> Lwt.return
    | pid when pid = 0 ->
      Lwt_unix.sleep 2.0 >>= fun () ->
      send_message "ping ping" parent_sock >>= fun () ->
      forever ()

    | _ -> assert false
  )

  |> Lwt_main.run
