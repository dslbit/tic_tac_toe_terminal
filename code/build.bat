@echo off

rem -MT > static link libs
rem -GS- > no crt buffer checks
set msvc_compiler_flags_vs2012=-nologo -FC -Z7 -MT -Oi -O2 -GS- -W4 -WX -c ..\code\tic_tac_toe.c
set msvc_linker_flags_vs2012=-nologo -nodefaultlib -incremental:no -opt:ref -opt:icf -fixed -subsystem:console,6.00 tic_tac_toe.obj msvcrt.lib

rem ucrt.lib > universal crt to solve standard input/output
rem libcmt.lib > init crt to solve for c version "unresolved external symbol _fltused"
set msvc_compiler_flags_vs2022=-nologo -FC -Z7 -MT -Oi -O2 -GS- -W4 -WX -c ..\code\tic_tac_toe.c
set msvc_linker_flags_vs2022=-nologo -nodefaultlib -incremental:no -opt:ref -opt:icf -fixed -subsystem:console,6.00 tic_tac_toe.obj libcmt.lib ucrt.lib

if not exist ..\build mkdir ..\build
pushd ..\build
	cl %msvc_compiler_flags_vs2022%
	link %msvc_linker_flags_vs2022%
popd
