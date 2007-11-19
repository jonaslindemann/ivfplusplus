@echo off

set SCRIPT_PATH=%~dp0

python -V 1>NUL 2>NUL
if errorlevel 1 goto needpython

python "%SCRIPT_PATH%\ivfappgen.py" %1 %2 %3 %4 %5
goto end

:needpython
echo ------------------------------------------------
echo Python is required for using this application 
echo ------------------------------------------------
pause
:end



