@echo off
python -V 1>NUL 2>NUL
if errorlevel 1 goto needpython
python ivfbuild.py %1 %2 %3 %4 %5
goto end
:needpython
echo ------------------------------------------
echo Python is required for Ivf++ build system.
echo ------------------------------------------
:end
