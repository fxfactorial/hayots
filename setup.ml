(* setup.ml generated for the first time by OASIS v0.4.8 *)

(* OASIS_START *)
(* DO NOT EDIT (digest: a426e2d026defb34183b787d31fbdcff) *)
(******************************************************************************)
(* OASIS: architecture for building OCaml libraries and applications          *)
(*                                                                            *)
(* Copyright (C) 2011-2016, Sylvain Le Gall                                   *)
(* Copyright (C) 2008-2011, OCamlCore SARL                                    *)
(*                                                                            *)
(* This library is free software; you can redistribute it and/or modify it    *)
(* under the terms of the GNU Lesser General Public License as published by   *)
(* the Free Software Foundation; either version 2.1 of the License, or (at    *)
(* your option) any later version, with the OCaml static compilation          *)
(* exception.                                                                 *)
(*                                                                            *)
(* This library is distributed in the hope that it will be useful, but        *)
(* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *)
(* or FITNESS FOR A PARTICULAR PURPOSE. See the file COPYING for more         *)
(* details.                                                                   *)
(*                                                                            *)
(* You should have received a copy of the GNU Lesser General Public License   *)
(* along with this library; if not, write to the Free Software Foundation,    *)
(* Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA              *)
(******************************************************************************)

let () =
  try
    Topdirs.dir_directory (Sys.getenv "OCAML_TOPLEVEL_PATH")
  with Not_found -> ()
;;
#use "topfind";;
#require "oasis.dynrun";;
open OASISDynRun;;

let setup_t = BaseCompat.Compat_0_4.adapt_setup_t setup_t
open BaseCompat.Compat_0_4
(* OASIS_STOP *)

let _ =
  BaseEnv.var_define
    "PWD"
    (fun () -> Unix.getcwd ())
    ()

let current = Unix.getcwd ()

(* Get the Code Editor *)
let _ = Sys.(
    if not (Sys.file_exists (current ^ "/" ^ "MGSFragaria.framework"))
        then begin
          ignore (
            command "git clone https://github.com/mugginsoft/Fragaria";
            command "xcodebuild -project Fragaria/Fragaria.xcodeproj \
                     -alltargets -configuration Release";
            command ("cp -R Fragaria/build/Release/MGSFragaria.framework " ^ current);
            command "rm -rf Fragaria"
          )
        end
  )

(* Get the msgpack objc lib *)
let _ = Sys.(
    if not (Sys.file_exists (current ^ "/" ^ "msgpack-objectivec"))
    then begin
      ignore (
        command "git clone https://github.com/fxfactorial/msgpack-objectivec";
        command "make -C msgpack-objectivec objc_library";
        command "cp msgpack-objectivec/libmsgpack-objc.dylib .";
        command "make -C msgpack-objectivec clean";
      )
    end
  )


let () = setup ();;
