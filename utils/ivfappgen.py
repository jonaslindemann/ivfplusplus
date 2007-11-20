#!/usr/bin/python

import os
import sys
import string

from Tkinter import *

import tkFileDialog
import tkMessageBox

cmakefileSource = """PROJECT(%(Appname)s)

SET(IVFWIN32 %(win32Flag)s )
SET(IVFSHARED %(sharedFlag)s )

IF(IVFWIN32)
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/. )
ELSE(IVFWIN32)
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
ENDIF(IVFWIN32)

SET( FREETYPE_INCLUDE /usr/include/freetype2 )
SET( IVF_ROOT_DIR "%(ivfRootDir)s" )
SET( IVF_DEPEND_DIR "%(ivfDependDir)s" )

MACRO(IVFAPP_SETUP)
	IF(IVFWIN32)
		INCLUDE_DIRECTORIES(${IVF_ROOT_DIR}/include ${IVF_ROOT_DIR}/include/vc ${IVF_DEPEND_DIR}/include )
		IF(IVFSHARED)
			ADD_DEFINITIONS(-DIVF_DLL -DFL_DLL)
			LINK_DIRECTORIES(${IVF_ROOT_DIR}/lib/shared ${IVF_DEPEND_DIR}/lib )
			IF(IVFDEBUG)
				SET(IVFAPP_DEBUG_LIBS ivfd ivfimaged ivf3duid ivfctld ivffiled ivffontd ivfgled ivfextd ivfmathd ivfuid ivffltkd ivfwidgetd )
				SET(EXT_DEBUG_LIBS fltkdlld freetypeMT_D_imp ftgl_dynamic_MTD_d gled fltkjpegd fltkpngd fltkzd opengl32 glu32 gle wsock32 comctl32 )
			ELSE(IVFDEBUG)
				SET(IVFAPP_LIBS ivf ivfimage ivf3dui ivfctl ivffile ivffont ivfgle ivfext ivfmath ivfui ivffltk ivfwidget opengl32 glu32 gle wsock32 comctl32 )
				SET(EXT_LIBS fltk fltkgl freetypeMT_imp ftgl_dynamic_MTD gle fltkjpeg fltkpng fltkz opengl32 glu32 gle wsock32 comctl32 )
			ENDIF(IVFDEBUG)
		ELSE(IVFSHARED)
			LINK_DIRECTORIES(${IVF_ROOT_DIR}/lib/static ${IVF_DEPEND_DIR}/lib )
			IF(IVFDEBUG)
				SET(IVFAPP_DEBUG_LIBS ivfd ivfimaged ivf3duid ivfctld ivffiled ivffontd ivfgled ivfextd ivfmathd ivfuid ivffltkd ivfwidgetd )
				SET(EXT_DEBUG_LIBS fltkd fltkgld freetype221MT_D ftgl_static_MTD_d gled fltkjpegd fltkpngd fltkzd opengl32 glu32 gle wsock32 comctl32 )
			ELSE(IVFDEBUG)
				SET(IVFAPP_LIBS ivf ivfimage ivf3dui ivfctl ivffile ivffont ivfgle ivfext ivfmath ivfui ivffltk ivfwidget opengl32 glu32 gle wsock32 comctl32 )
				SET(EXT_LIBS fltk fltkgl freetype221MT ftgl_static_MTD gle fltkjpeg fltkpng fltkz opengl32 glu32 gle wsock32 comctl32 )
			ENDIF(IVFDEBUG)
		ENDIF(IVFSHARED)
	ELSE(IVFWIN32)
		INCLUDE_DIRECTORIES(${IVF_ROOT_DIR}/include ${FREETYPE_INCLUDE})
		SET(IVFAPP_LIBS ivf ivfimage ivf3dui ivfctl ivffile ivffont ivfgle ivfext ivfmath ivfui ivffltk ivfwidget )
		SET(EXT_LIBS fltk fltk_gl freetype ftgl gle jpeg png GL GLU )
	ENDIF(IVFWIN32)

	FILE(GLOB SOURCE_FILES %(sourceDir)s*.cpp)
	FILE(GLOB INCLUDE_FILES %(sourceDir)s*.h)

	IF(IVFWIN32)
		IF(IVFDEBUG)
			ADD_EXECUTABLE( ${IVFAPP_NAME}d ${SOURCE_FILES} ${INCLUDE_FILES})
			TARGET_LINK_LIBRARIES( ${IVFAPP_NAME}d ${IVFAPP_DEBUG_LIBS} ${EXT_DEBUG_LIBS})
			STRING(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
			INSTALL_TARGETS( . ${IVFAPP_NAME}d )
		ELSE(IVFDEBUG)
			ADD_EXECUTABLE(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
			TARGET_LINK_LIBRARIES( ${IVFAPP_NAME} ${IVFAPP_LIBS} ${EXT_LIBS})
			INSTALL_TARGETS( . ${IVFAPP_NAME} )
		ENDIF(IVFDEBUG)		
	ELSE(IVFWIN32)
		ADD_EXECUTABLE(${IVFAPP_NAME} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES(${IVFAPP_NAME} ${IVFAPP_LIBS} ${EXT_LIBS}) 
		INSTALL_TARGETS( . ${IVFAPP_NAME} )
	ENDIF(IVFWIN32)

ENDMACRO(IVFAPP_SETUP)

SET(IVFAPP_NAME %(Appname)s)
IVFAPP_SETUP()
"""

makefileLinuxSource = """.PHONY: all
all:
	cd %(build_dir)s && $(MAKE)
	cd ..
	
clean:
	-rm -rf %(build_dir)s
	-rm -rf bin
"""

configureCommandLinuxSource = """#!/bin/bash

BUILD_DIR=%(build_dir)s

if [ ! -d $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi

pushd $BUILD_DIR
cmake ..
popd
"""

configureCommandIdeSource = """@echo off

cmake --version 1>NUL 2>NUL
if errorlevel 1 goto needcmake

if exist %(build_dir)s_release (
	rmdir /Q /S %(build_dir)s_release
)
if exist %(build_dir)s_debug (
	rmdir /Q /S %(build_dir)s_debug
)
if exist bin (
	rmdir /Q /S bin
)
mkdir %(build_dir)s_release
pushd %(build_dir)s_release
cmake %(generator)s -DIVFDEBUG=OFF -DCMAKE_BUILD_TYPE=Release  ..
popd

mkdir %(build_dir)s_debug
pushd %(build_dir)s_debug
cmake %(generator)s -DIVFDEBUG=ON -DCMAKE_BUILD_TYPE=Debug  ..
popd

goto end

:needcmake
echo ------------------------------------------------
echo CMAKE is required for building this application.
echo ------------------------------------------------
:end
"""

configureCommandNmakeSource = """@echo off

cmake --version 1>NUL 2>NUL
if errorlevel 1 goto needcmake

cl 1>NUL 2>NUL
if errorlevel 1 goto needvsvars

if exist win32_build (
	rmdir /Q /S %(build_dir)s
)
if exist bin (
	rmdir /Q /S bin
)
mkdir %(build_dir)s
pushd %(build_dir)s
cmake %(generator)s ..
popd
goto end

:needcmake
echo ------------------------------------------------
echo CMAKE is required for building this application.
echo ------------------------------------------------
goto end
:needvsvars
echo -------------------------------------------
echo Required Visual Studio environment not set.
echo Please runs vsvars32.bat located in
echo [VS-dir]\Common[X]\Tools
echo -------------------------------------------
:end
"""

buildCommandNmakeSource = """@echo off

cl 1>NUL 2>NUL
if errorlevel 1 goto needvsvars

if not exist %(build_dir)s (
	call %(configure_command)s
)

pushd %(build_dir)s
nmake /F Makefile
popd
goto end

:needvsvars
echo -------------------------------------------
echo Required Visual Studio environment not set.
echo Please runs vsvars32.bat located in
echo [VS-dir]\Common[X]\Tools
echo -------------------------------------------
:end
"""

buildCommandIdeSource = """@echo off

if not exist %(build_dir)s (
	call %(configure_command)s
)

pushd %(build_dir)s
%(appname)s.sln
popd
"""


cleanCommandSource = """@echo off
if exist %(build_dir)s_debug (
	rmdir /Q /S %(build_dir)s_debug
)
if exist %(build_dir)s_release (
	rmdir /Q /S %(build_dir)s_release
)
if exist bin (
	rmdir /Q /S bin
)
if exist lib (
	rmdir /Q /S lib
)
"""

stdcppSource = """#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	cout << \"Hello, world!\" << endl;
}
"""

stdivfSource = """// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfScene.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(C%(Appname)sWindow);

class C%(Appname)sWindow: public CIvfWindow,
	CIvfInitEvent,
	CIvfKeyboardEvent
{
private:
	CIvfCameraPtr		m_camera;
	CIvfScenePtr		m_scene;
	CIvfLightPtr		m_light;

	CIvfMouseViewHandlerPtr m_mouseHandler;
	CIvfSceneHandlerPtr	m_sceneHandler;

public:
	C%(Appname)sWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

C%(Appname)sWindow::C%(Appname)sWindow(int X, int Y, int W, int H)
	:CIvfWindow(X, Y, W, H)
{
	addInitEvent(this);
	addKeyboardEvent(this);
}

void C%(Appname)sWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new CIvfScene();
	m_scene->setView(m_camera);

	// Create a materials

	CIvfMaterialPtr redMaterial = new CIvfMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	CIvfMaterialPtr greenMaterial = new CIvfMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	CIvfMaterialPtr blueMaterial = new CIvfMaterial();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	CIvfMaterialPtr yellowMaterial = new CIvfMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create objects

	CIvfCubePtr cube = new CIvfCube();
	cube->setMaterial(redMaterial);
	cube->setPosition(2.0, 0.0, 2.0);
	m_scene->addChild(cube);

	CIvfSpherePtr sphere = new CIvfSphere();
	sphere->setMaterial(greenMaterial);
	sphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(sphere);

	CIvfCylinderPtr cylinder = new CIvfCylinder();
	cylinder->setMaterial(blueMaterial);
	cylinder->setPosition(-2.0, 0.0, -2.0);
	m_scene->addChild(cylinder);

	CIvfConePtr cone = new CIvfCone();
	cone->setMaterial(yellowMaterial);
	cone->setPosition(2.0, 0.0, -2.0);
	cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_scene->addChild(cone);

	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);

	// Initialize lighting

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_light->enable();

	// Create event handlers

	m_mouseHandler = new CIvfMouseViewHandler(this, m_camera);
	m_mouseHandler->activate();

	m_sceneHandler = new CIvfSceneHandler(this, m_scene);
	m_sceneHandler->activate();
}

void C%(Appname)sWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case '1' :
		break;
	case '2' :
		break;
	case '3' :
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv)
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	C%(Appname)sWindowPtr window = new C%(Appname)sWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("%(Appname)s");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
"""

simpleivfSource = """// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(C%(Appname)sWindow);

class C%(Appname)sWindow: public CIvfWindow,
	CIvfInitEvent,
	CIvfInitContextEvent,
	CIvfResizeEvent,
	CIvfRenderEvent,
	CIvfMouseDownEvent,
	CIvfMouseMoveEvent,
	CIvfMouseUpEvent,
	CIvfKeyboardEvent
{
private:

public:
	C%(Appname)sWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onInitContext(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

C%(Appname)sWindow::C%(Appname)sWindow(int X, int Y, int W, int H)
	:CIvfWindow(X, Y, W, H)
{
	addInitEvent(this);
	addInitContextEvent(this);
	addKeyboardEvent(this);
	addResizeEvent(this);
	addRenderEvent(this);
	addMouseDownEvent(this);
	addMouseMoveEvent(this);
	addMouseUpEvent(this);
}

void C%(Appname)sWindow::onInit(int width, int height)
{
	// Add OpenGL initialisation code here.
	// OpenGL Rendering context is valid here.
}

void C%(Appname)sWindow::onInitContext(int width, int height)
{
	// Add initialisation code needed when
	// a new context has been created.

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}

void C%(Appname)sWindow::onResize(int width, int height)
{
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
}

void C%(Appname)sWindow::onRender()
{
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(-1.0, -1.0 );
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2d( 1.0, -1.0 );
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2d( 1.0,  1.0 );
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex2d(-1.0,  1.0 );
	glEnd();
}

void C%(Appname)sWindow::onMouseDown(int x, int y)
{
	cout << "Mouse down at " << x << ", " << y << endl;
}

void C%(Appname)sWindow::onMouseUp(int x, int y)
{
	cout << "Mouse up at " << x << ", " << y << endl;
}

void C%(Appname)sWindow::onMouseMove(int x, int y)
{
	cout << "Mouse move " << x << ", " << y << endl;
}

void C%(Appname)sWindow::onKeyboard(int key, int x, int y)
{
	cout << "Key " << key << " pressed." << endl;
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv)
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	C%(Appname)sWindowPtr window = new C%(Appname)sWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("%(Appname)s");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
"""

def createAppSource(appType,appName,sourceFile,sourceDir):

	if sourceDir == "":
		prefix = ""
	else:
		prefix = "./"+sourceDir+"/"
		if not os.path.exists(prefix):
			os.mkdir(prefix)
		
	if os.path.exists(prefix+sourceFile):
		print "Source file already exists skipping."
		return

	sourceFile = open(prefix+sourceFile, 'w')

	if appType == 'stdivf':
		appNameTuple = {'Appname':string.capitalize(appName)}
		sourceFile.write(stdivfSource % appNameTuple)

	if appType == 'simpleivf':
		appNameTuple = {'Appname':string.capitalize(appName)}
		sourceFile.write(simpleivfSource % appNameTuple)

	sourceFile.close();
	
def createCMakeFile(appName, win32Build, debugLibs, sharedLibs, ivfRootDir, ivfDependDir, sourceDir = "src"):
	
	templateParams = {}
	
	if win32Build:
		templateParams["win32Flag"] = "ON"
	else:
		templateParams["win32Flag"] = "OFF"
		
	if debugLibs:
		templateParams["debugFlag"] = "ON"
	else:
		templateParams["debugFlag"] = "OFF"
		
	if sharedLibs:
		templateParams["sharedFlag"] = "ON"
	else:
		templateParams["sharedFlag"] = "OFF"
		
	templateParams["Appname"] = appName
	templateParams["ivfRootDir"] = ivfRootDir
	templateParams["ivfDependDir"] = ivfDependDir
	if sourceDir!="":
		templateParams["sourceDir"] = sourceDir+"/"
	else:
		templateParams["sourceDir"] = ""
	
	cmakeFile = file("CMakeLists.txt", "w")
	cmakeFile.write(cmakefileSource % templateParams)
	cmakeFile.close()
	
def createBuildScriptsLinux(appName):
	
	configureScript = "configure"
	makefileName = "Makefile"
		
	buildDir = "build"
	
	configureFile = file(configureScript, "w")
	configureFile.write(configureCommandLinuxSource% { "build_dir":buildDir })
	configureFile.close()
	
	makeFile = file(makefileName, "w")
	makeFile.write(makefileLinuxSource % { "build_dir":buildDir })
	makeFile.close()

def createBuildScripts(nmakeBuild, debugLibs, sharedLibs, appName):
	
	if nmakeBuild:
		generatorSwitch = '-G"NMake Makefiles"'
	else:
		generatorSwitch = ""
		
	configureScript = "configure"
	buildScript = "build"
	cleanScript = "clean"
		
	buildDir = "build"
	
#	if debugLibs:
#		buildDir = buildDir + "_debug"
#		configureScript = configureScript + "_debug"
#		buildScript = buildScript + "_debug"
#		cleanScript = cleanScript + "_debug"
	
	if sharedLibs:
		buildDir = buildDir + "_dll"
		configureScript = configureScript + "_dll"
		buildScript = buildScript + "_dll"
		cleanScript = cleanScript + "_dll"
		
	if nmakeBuild:
		buildDir = buildDir + "_nmake"
		configureScript = configureScript + "_nmake"
		buildScript = buildScript + "_nmake"
		cleanScript = cleanScript + "_nmake"

	configureScript = configureScript + ".cmd"
	buildScript = buildScript + ".cmd"
	cleanScript = cleanScript + ".cmd"
		
	configureFile = file(configureScript, "w")
	if nmakeBuild:
		configureFile.write(configureCommandNmakeSource % { "generator":generatorSwitch, "build_dir":buildDir })
	else:
		configureFile.write(configureCommandIdeSource % { "generator":generatorSwitch, "build_dir":buildDir })
	configureFile.close()
	
	cleanFile = file(cleanScript, "w")
	cleanFile.write(cleanCommandSource % { "build_dir":buildDir })
	cleanFile.close()

	buildFile = file(buildScript, "w")
	if nmakeBuild:
		buildFile.write(buildCommandNmakeSource % { "build_dir":buildDir, "configure_command":configureScript })
	else:
		buildFile.write(buildCommandIdeSource % { "build_dir":buildDir, "configure_command":configureScript, "appname":appName })
	buildFile.close()

def checkArguments():
	result = ['stdcpp', 'app', False]

	if len(sys.argv)<2:
		print 'Ivf Appgen 0.5 - Ivf++ Application Template Generator'
		print 'Copyright (C) 2006 Division of Structural Mechanics'
		print 'Usage:'
		print '\tivfappgen apptype|gui [appname] [nmake]'
		print
		print 'values for apptype are:'
		print
		print '\tstdivf\t\tStandard Ivf++ application'
		print '\tsimpleivf\tSkeleton Ivf++ application'
		print '\tbuidlsys\tCreate Ivf++ build system for existing source'
		print 
		print '\tgui\t\tShow a user interface for application generation'
		print
		sys.exit(0)

	if len(sys.argv)>1:
		if sys.argv[1]=='stdcpp':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='stdivf':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='stdfltk':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='simpleivf':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='buildsys':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='gui':
			result[0] = sys.argv[1]
		else:
			print 'Application type '+sys.argv[1]+' not supported.'
			sys.exit(0)

	if len(sys.argv)>2:
		result[1] = sys.argv[2]

	if len(sys.argv)>3:
		if sys.argv[3] == "nmake":
			result[2] = True
		else:
			result[2] = False

	return result

class AppWindow(Frame):
	def __init__(self, parent):
		self._window = parent
		self._window.title("Ivf++ Application Generator")
		if sys.platform == "win32":
			self._window.geometry("400x450")
		else:
			self._window.geometry("400x260")
		
		# Define variables
		
		self._ivfRootDir = StringVar()
		self._ivfDependDir = StringVar()
		if sys.platform == "win32":
			self._ivfRootDir.set("c:/program files/structural mechanics/ivf")
			self._ivfDependDir.set("c:/program files/structural mechanics/ivf-depend")
		else:
			self._ivfRootDir.set("/usr/local")
			self._ivfDependDir.set("/usr/local")
			
		self._appTargetDir = StringVar()
		self._appTargetDir.set(".")
		self._appName = StringVar()
		self._appName.set("Example")
		
		self._appSourceDir = StringVar()
		self._appSourceDir.set("src")
		
		self._appSourceGeneration = IntVar()
		self._appSourceGeneration.set(1)
		
		self._appType = IntVar()
		self._appType.set(1)
		
		self._sharedLib = IntVar()
		self._sharedLib.set(0)
		self._debugLib = IntVar()
		self._debugLib.set(0)

		self._nmakeBuild = IntVar()
		self._nmakeBuild.set(0)
		
		self.initGUI()

		self._window.protocol('WM_DELETE_WINDOW', self._window.destroy)	
		
	def initGUI(self):
		frame = Frame(self._window, bd=5)
		frame.pack(expand=YES, fill=BOTH)

		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)
		
		label = Label(row, text="Application name", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtAppName = Entry(row, width=5, textvariable=self._appName)
		self.edtAppName.pack(side=LEFT,fill=BOTH,expand=YES)
		
		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)

		label = Label(row, text="Source dir", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtSourceDir = Entry(row, width=5, textvariable=self._appSourceDir)
		self.edtSourceDir.pack(side=LEFT,fill=BOTH,expand=YES)

		if sys.platform == "win32":

			row = Frame(frame, bd=2)
			row.pack(expand=NO, fill=BOTH)
			
			label = Label(row, text="Install dir", width=20, anchor=W, justify=RIGHT)
			label.pack(side=LEFT)
			self.edtInstallDir = Entry(row, width=5, textvariable=self._ivfRootDir)
			self.edtInstallDir.pack(side=LEFT,fill=BOTH,expand=YES)
			self.btnSelectInstallDir = Button(row, text="...", command = self.onSelectInstallDir)
			self.btnSelectInstallDir.pack(side=LEFT)
	
			row = Frame(frame, bd=2)
			row.pack(expand=NO, fill=BOTH)
	
			label = Label(row, text="Dependency dir", width=20, anchor=W, justify=RIGHT)
			label.pack(side=LEFT)
			self.edtDependencyDir = Entry(row, width=5, textvariable=self._ivfDependDir)
			self.edtDependencyDir.pack(side=LEFT,fill=BOTH,expand=YES)
			self.btnSelectDependDir = Button(row, text="...", command = self.onSelectDependDir)
			self.btnSelectDependDir.pack(side=LEFT)
		
		if sys.platform == "linux2":

			row = Frame(frame, bd=2)
			row.pack(expand=NO, fill=BOTH)
			
			label = Label(row, text="Ivf++ install prefix", width=20, anchor=W, justify=RIGHT)
			label.pack(side=LEFT)
			self.edtInstallDir = Entry(row, width=5, textvariable=self._ivfRootDir)
			self.edtInstallDir.pack(side=LEFT,fill=BOTH,expand=YES)
			self.btnSelectInstallDir = Button(row, text="...", command = self.onSelectInstallDir)
			self.btnSelectInstallDir.pack(side=LEFT)
	
			row = Frame(frame, bd=2)
			row.pack(expand=NO, fill=BOTH)
	
			label = Label(row, text="Dependency install prefix", width=20, anchor=W, justify=RIGHT)
			label.pack(side=LEFT)
			self.edtDependencyDir = Entry(row, width=5, textvariable=self._ivfDependDir)
			self.edtDependencyDir.pack(side=LEFT,fill=BOTH,expand=YES)
			self.btnSelectDependDir = Button(row, text="...", command = self.onSelectDependDir)
			self.btnSelectDependDir.pack(side=LEFT)

		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)

		label = Label(row, text="Target dir", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtTargetDir = Entry(row, width=5, textvariable=self._appTargetDir)
		self.edtTargetDir.pack(side=LEFT,fill=BOTH,expand=YES)
		self.btnSelectTargetDir = Button(row, text="...", command = self.onSelectTargetDir)
		self.btnSelectTargetDir.pack(side=LEFT)

		label = Label(frame, text="Ivf++ Application type", anchor=SW, justify=RIGHT, height=2)
		label.pack(expand=NO, fill=X, padx=4)
		
		row = Frame(frame, bd=2, relief=GROOVE)
		row.pack(expand=NO, fill=BOTH, padx=4, pady=4)

		Radiobutton(row, text="Standard, using handlers classes", value=1, variable=self._appType).pack(anchor=W)
		Radiobutton(row, text="Simple, no handler classes", value=2, variable=self._appType).pack(anchor=W)
		
		if sys.platform == "win32":

			label = Label(frame, text="Ivf++ library options", anchor=SW, justify=RIGHT, height=1)
			label.pack(expand=NO, fill=X, padx=4)
			
			row = Frame(frame, bd=2, relief=GROOVE)
			row.pack(expand=NO, fill=BOTH, padx=4, pady=4)
	
			Checkbutton(row, text="Link against DLL-library", variable=self._sharedLib).pack(anchor=W)
			Checkbutton(row, text="Debug library", variable=self._debugLib).pack(anchor=W)		
	
			label = Label(frame, text="Ivf++ build options", anchor=SW, justify=RIGHT, height=1)
			label.pack(expand=NO, fill=X, padx=4)
			
			row = Frame(frame, bd=2, relief=GROOVE)
			row.pack(expand=NO, fill=BOTH, padx=4, pady=4)
	
			Checkbutton(row, text="NMake build", variable=self._nmakeBuild).pack(anchor=W)

		label = Label(frame, text="Other options", anchor=SW, justify=RIGHT, height=1)
		label.pack(expand=NO, fill=X, padx=4)
			
		row = Frame(frame, bd=2, relief=GROOVE)
		row.pack(expand=NO, fill=BOTH, padx=4, pady=4)

		Checkbutton(row, text="Generate source", variable=self._appSourceGeneration).pack(anchor=W)

		row = Frame(frame, bd=2)
		row.pack()
		
		#col = Frame(row, bd=0)
		#col.pack()
		
		self.btnGenerate = Button(row, text="Generate", command = self.onGenerate)
		self.btnGenerate.pack(side=LEFT, pady=4)
		
		self.btnClose = Button(row, text="Close", command = self.onClose)
		self.btnClose.pack(side=LEFT, expand=NO, pady=4)
		
	def onSelectInstallDir(self):
		newDir = tkFileDialog.askdirectory(initialdir = self._ivfRootDir.get())
		if newDir!="":
			self._ivfRootDir.set(newDir)
	
	def onSelectDependDir(self):
		newDir = tkFileDialog.askdirectory(initialdir = self._ivfDependDir.get())
		if newDir!="":
			self._ivfDependDir.set(newDir)
	
	def onSelectTargetDir(self):
		newDir = tkFileDialog.askdirectory(initialdir = self._appTargetDir.get())
		if newDir!="":
			self._appTargetDir.set(newDir)
			
	def onGenerate(self):

		currDir = os.getcwd()
		os.chdir(self._appTargetDir.get())
		
		appType = ""
		appName = ""
		appSources = ""
		
		if self._appType.get() == 1:
			appType = "stdivf"
		elif self._appType.get() == 2:
			appType = "simpleivf"
		else:
			appType = "stdivf"
			
		appName = self._appName.get()
		appSources = ['main.cpp']
			
		if sys.platform == "win32":
			win32Build = True
		else:
			win32Build = False
			
		if self._sharedLib.get() == 1:
			sharedLibs = True
		else:
			sharedLibs = False
			
		if self._debugLib.get() == 1:
			debugLibs = True
		else:
			debugLibs = False
		
		if self._nmakeBuild.get() == 1:
			nmakeBuild = True
		else:
			nmakeBuild = False

		ivfRootDir = self._ivfRootDir.get()
		ivfDependDir = self._ivfDependDir.get()
		sourceDir = self._appSourceDir.get()
		generateSource = True
		
		if self._appSourceGeneration.get() == 1:
			generateSource = True
		else:
			generateSource = False
		
		generateApp(appType, appName, appSources, win32Build, sharedLibs, debugLibs, nmakeBuild, ivfRootDir, ivfDependDir, sourceDir, generateSource)
		
		tkMessageBox.showinfo(title="Application Generation", message="Application has been generated.")

		os.chdir(currDir)
	
	def onClose(self):
		self._window.destroy()
		
def	showGui():
	
	root = Tk()
	
	appWindow = AppWindow(root)
	
	root.mainloop()
	
def haveExistingSources():
	
	dirList = os.listdir(".")
	
	for item in dirList:
		if os.path.isfile(item):
			path, ext = os.path.splitext(item)
			if ext == ".cpp" or ext == ".h":
				return True
	
	return False
	
def generateApp(appType, appName, appSources, win32Build, sharedLibs, debugLibs, nmakeBuild, ivfRootDir, ivfDependDir, sourceDir="src", generateSource=True):
	
	if haveExistingSources():
		sourceDir = "."
		generateSource = False

	if generateSource:	
		print 'Creating sourcefiles.'
		createAppSource(appType, appName, appSources[0], sourceDir)
		
	print 'Creating CMake file.'
	createCMakeFile(appName, win32Build, debugLibs, sharedLibs, ivfRootDir, ivfDependDir, sourceDir)
	print 'Creating build scripts.'
	if sys.platform == "win32":
		createBuildScripts(nmakeBuild, debugLibs, sharedLibs, appName)
	elif sys.platform == "linux2":
		createBuildScriptsLinux(appName)

if __name__ == "__main__":
	
	ivfRootDir = ""
	ivfDependDir = ""

	if sys.platform == "win32":
		ivfRootDir = "c:/program files/structural mechanics/ivf"
		ivfDependDir = "c:/program files/structural mechanics/ivf-depend"
	else:
		ivfRootDir = "/usr/local"
		ivfDependDir = "/usr/local"
		

	args = checkArguments()
	
	appType = args[0]
	appName = args[1]
	appSources = ['main.cpp']

	if appType == "gui":
		showGui()
		sys.exit(0)
		
	win32Build = False
	
	if sys.platform == "win32":
		win32Build = True
	
	sharedLibs = False
	debugLibs = False
	
	generateSource = True
	
	if appType == "buildsys":
		generateSource = False

	nmakeBuild = args[2]
	
	generateApp(appType, appName, appSources, win32Build, sharedLibs, debugLibs, nmakeBuild, ivfRootDir, ivfDependDir)	
	
	print 'Done.'

