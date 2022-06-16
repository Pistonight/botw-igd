# botw-igd
In-game Debugger for BOTW

## Development Env Setup
You need a Linux/WSL environment
#### devkitPro
Follow the instructions at https://devkitpro.org/wiki/Getting_Started to install the switch toolchain. You need the `switch-dev` package.

Restart the shell and make sure the env variable `DEVKITPRO` is set properly (usually `/opt/devkitpro`). The build won't run if this is not set.

#### Just
Just is a command line tool to run scripts. Follow the instructions here https://github.com/casey/just (Cargo install recommended)

#### Python 3
Python is used to run scripts in this project. Install python 3, then run `just pipinstall` to install the modules

#### Cpp Linter
`clang-format` is used to lint cpp code. Run `sudo apt install clang-format-12`

## Lib Repos
This project contains a copy for the headers it depends on from botw and other decomp projects so you can edit the headers for prototyping. A tool is used to copy files from/to external repos so you can easily contribute to the decomp projects.

#### Workspace Config
The workspace stores its variables in a `toml` file. By default, the values in `workspace.template.toml` is used. If you need to change something, make a copy of the file and rename it `workspace.toml`, then change the value. `workspace.toml` is git-ignored.

#### Clone Dependencies

#### Lib Tool
The lib tool is a script that detects difference between the cloned repos and the code inside this project.

If something in the dependency repo changes, run `just lib f` to list the changes and run `just lib fs` to copy the change from outside to this project

If something is changed in this project, run `just lib r` to list the changes and run `just lib rs` to copy those changes to the external repos

## Working with IPS Patch `(.patch.s)`
This project uses a python script to generate `.ips` patches from `.patch.s` files. The syntax is similar to assembly. Here is an example

```arm
.target main   ; This sets the target of the patch. 
               ; The target needs to be configured in workspace.toml under [ips.<target>] table
               ; In the table, "id" is required as it would become the name of the patch (the build id of the module)
               ; "offset" is only required if you are calling functions in botwigd (i.e. BL <func>), since the symbol needs to be converted to an offset
               ; see workspace.template.toml for examples
.asm           ; This starts the asm section, indicating that the following code
               ; should be treated as ARM assembly
               ; Other options are .bytes and .string

; You can have multiple .target or .asm or other types in the same file.

; Each line is in the form of <address> <content>
; For .asm patches, <content> is ARM assembly code
; For .bytes, <content> is the bytes encoded in base64
; For .string, <content> is a double-quoted string, which will be encoded with utf-8
0x00C66528 NOP                       ; This replaces the instruction at 0x00C66528 with NOP
                                     ; 0x00C66528 is the offset from the start of target (in this case, main)
           FMOV S10, S11             ; If no address is provided, the patch will be appended to the previous line
                                     ; So this will patch the instruction at 0x00C6652C                   
0x00C66534 FADD S11, S12, S9         ; You must specify address if you are jumping to another location
0x00C6656C BL ksys::igd::RenderDebugScreen     
; For BL, the symbol can be a botwigd symbol or an address, plus an offset
;    <symbol|address>+<offset>
; botwigd_symbol can be any function in botwigd. The compiler will try to search for it
; If you are jumping to botwigd, you must provide "offset" in workspace.toml so the branch can be computed
; Another option is to jump to an address, relative to start of target

; If you want to jump to another module (i.e. not target or botwigd), it's currently not supported directly.
; One workaround is to implement a botwigd function that calls the function you want to jump to, then patch the instruction to call the botwigd function
; Another way is to use address+offset directly

```
