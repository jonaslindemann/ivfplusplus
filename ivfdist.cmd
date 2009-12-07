@echo off
cls

echo ...
echo ... Setting up environment ...
echo ...

echo ...
echo ... Building for MSVC %VS_VERSION% located at:
echo ... %VS_ENV_DIR%
echo ...

set IVF_VERSION=1.0.0
set INSTALL_DIR=.
set INSTALL_FILENAME=setup-ivf-%ARCH%-%COMPILER_NAME%-%VS_VERSION%-%IVF_VERSION%
set INNOSETUP_DIR="C:\Program Files\Inno Setup 5"
set DIST_INSTALL_DIR="d:\dev\ivf\dist_install"

echo ...
echo ... Setting up PATH ...
echo ...

set OLDPATH=%PATH%
set PATH=%PATH%;%VS_ENV_DIR%;%INNOSETUP_DIR%

if defined PLATFORM_SDK_DIR (
set PATH=%PATH%;%PLATFORM_SDK_DIR%\bin
)


echo ...
echo ... Setting up Visual Studio environment ...
echo ...

call vsvars32.bat

echo ...
echo ... Setting up Ivf++ build environment ...
echo ...

call ivfvars

echo ...
echo ... Cleaning dist directory ...
echo ...

if defined CLEAN_DIST_DIR (
rmdir /Q /S dist
)

if defined STATIC_RELEASE (
echo ...
echo ... Building Ivf++ static release library ...
echo ...
md dist
md dist\lib
md dist\lib\static
call ivfbuild clean
call ivfbuild configure static_release nmake
call ivfbuild build
del bin\*.ilk
xcopy /E /I /Y lib dist\lib\static
xcopy /E /I /Y bin dist\bin
)

if defined STATIC_DEBUG (
echo ...
echo ... Building Ivf++ static debug library ...
echo ...
md dist
md dist\lib
md dist\lib\static
call ivfbuild clean
call ivfbuild configure static_debug nmake
call ivfbuild build
del bin\*.ilk
del bin\*.pdb
xcopy /E /I /Y lib dist\lib\static
rem xcopy /E /I /Y bin dist\bin\static
)

if defined SHARED_RELEASE (
echo ...
echo ... Building Ivf++ shared release library ...
echo ...
md dist
md dist\lib
md dist\lib\shared
call ivfbuild clean
call ivfbuild configure shared_release nmake
call ivfbuild build
del bin\*.ilk
xcopy /E /I /Y lib dist\lib\shared
rem xcopy /E /I /Y bin dist\bin\static
)

if defined SHARED_DEBUG (
echo ...
echo ... Building Ivf++ shared debug library ...
echo ...
md dist
md dist\lib
md dist\lib\shared
call ivfbuild clean
call ivfbuild configure shared_debug nmake
call ivfbuild build
del bin\*.ilk
del bin\*.pdb
xcopy /E /I /Y lib dist\lib\shared
rem xcopy /E /I /Y bin dist\bin\static
)

if defined DOCUMENTATION (
echo ...
echo ... Building documentation ...
echo ...
call ivfbuild docs
mkdir dist\doc
mkdir dist\html
copy doc\ivfguide.pdf dist\doc
xcopy /E /I /Y doc\html dist\doc\html
)

if defined OTHER_INSTALL (
echo ...
echo ... Copying other distribution files ...
echo ...
copy AUTHORS dist
copy COPYING dist
copy THANKS dist
copy README.EXAMPLES.txt dist
xcopy /E /I /Y include dist\include
xcopy /E /I /Y utils dist\utils
xcopy /EXCLUDE:ivfdist_exclude.txt /E /I /Y examples dist\examples
)

if defined INSTALL_BUILD (
echo ...
echo ... Creating distribution install ...
echo ...
iscc %DIST_INSTALL_DIR%\ivf-bin-win32.iss /o%INSTALL_DIR% /f%INSTALL_FILENAME% /dIvfVersion=%IVF_VERSION% 
)

echo ...
echo ... Distribution build finished ...
echo ...
set %PATH%=%OLDPATH%
