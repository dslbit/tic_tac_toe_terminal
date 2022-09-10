@echo off

if not exist ..\build mkdir ..\build
pushd ..\build
	cl -nologo -FC -Z7 -Od -Oi ..\code\tic_tac_toe.c -link -subsystem:console,6.00
popd
