@echo off
cls
echo Configuring Ivf++ run-time environment.
call ivfenv
set PATH=%PATH%;%PYTHON_ROOT%;%IVF_ROOT%\lib;%IVF_ROOT%\lib\debug;%IVF_ROOT%\lib\release;%IVF_DEPEND_ROOT%\shared;%IVF_ROOT%\utils;%MINGW_ROOT%\bin;%MSYS_ROOT%\bin
