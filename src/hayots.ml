open Lwt.Infix

external init_ide : int -> unit = "hayots_ml_init_gui"

let rec forever () =
  let _ = Lwt.wait () in
  forever ()

class hayots sock = object

  val out_channel = Lwt_io.of_fd ~mode:Lwt_io.Output sock
  val in_channel = Lwt_io.of_fd ~mode:Lwt_io.Input sock

  initializer
    Gc.finalise
      (fun out_chan ->
         (Lwt_io.close out_chan >|= fun _ -> ()) |> Lwt.ignore_result)
      out_channel

  method send_message s = Lwt_io.(
      let length = String.length s in
      write_int out_channel length >>= fun () ->
      write out_channel s >>= fun () ->
      flush out_channel
    )

  method receive_reply = Lwt_io.(
      Lwt_io.read_char in_channel >>= fun chr ->
      let reply_length = Char.code chr in
      let buffer = Bytes.create reply_length in
      read_into_exactly in_channel buffer 0 reply_length >>= fun () ->
      Lwt_io.flush_all () >>= fun () ->
      Lwt.return buffer
    )

end

let () =
  let (parent_sock, gui_sock) =
    Lwt_unix.socketpair Lwt_unix.PF_UNIX Lwt_unix.SOCK_STREAM 0
  in
  (
    Lwt_io.flush_all () >>= fun () ->
    let pid = Lwt_unix.fork () in
    (* Cocoa can only run in the master process *)
    match pid with
    (* Parent process *)
    | pid when pid > 0 ->
      init_ide (Obj.magic (Lwt_unix.unix_file_descr gui_sock))
      |> Lwt.return
    | pid when pid = 0 ->
      (* Child needs to tell parent its PID to properly close child,
         give NSApplication a chance to properly setup *)
      Lwt_unix.sleep 1.0 >>= fun () ->
      let hayots = new hayots parent_sock in
      `Assoc [("child-pid", `Int (Unix.getpid ()))]
      |> Yojson.Basic.to_string
      |> hayots#send_message >>= fun () ->
      hayots#receive_reply >>= fun reply ->
      Lwt_io.printlf "Reply: %s" reply >>= fun () ->
      Lwt_io.flush_all () >>= fun () ->

      let utop = new Service.utop in

      utop#read_all >>= fun () ->
      Lwt_io.printl "After utop" >>= fun () ->

      forever ()
    | _ -> assert false
  )

  |> Lwt_main.run
