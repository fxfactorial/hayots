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
      read_int in_channel >>= fun reply_length ->
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
      (* Never returns *)
      init_ide (Obj.magic (Lwt_unix.unix_file_descr gui_sock))
      |> Lwt.return
    | pid when pid = 0 ->
      Lwt_unix.sleep 1.5 >>= fun () ->
      let hayots = new hayots parent_sock in
      hayots#send_message {|{"command":"something"}|} >>= fun () ->
      Lwt_io.printlf "Sent message, now receive" >>= fun () ->
      hayots#receive_reply >>= fun reply ->
      Lwt_io.printlf "Reply: %s" reply >>= fun () ->
      Lwt_io.flush_all () >>= fun () ->
      forever ()
    | _ -> assert false
  )

  |> Lwt_main.run
