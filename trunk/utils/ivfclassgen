#!/usr/bin/env python

import os
import sys
import string

from Tkinter import *

import tkFileDialog
import tkMessageBox

customIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/%(BaseClass)s.h>

IvfSmartPointer(C%(Classname)s);

class C%(Classname)s: public C%(BaseClass)s {
private:

public:
	C%(Classname)s();
	virtual ~C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", C%(BaseClass)s);
};

#endif
"""

customImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s::C%(Classname)s()
{
	// Add construction code here
}

C%(Classname)s::~C%(Classname)s()
{
	// Add clean up code here
}
"""

baseIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfBase.h>

IvfSmartPointer(C%(Classname)s);

class C%(Classname)s: public CIvfBase {
private:

public:
	C%(Classname)s();
	virtual ~C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", CIvfBase);
};

#endif
"""

baseImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s::C%(Classname)s()
{
	// Add construction code here
}

C%(Classname)s::~C%(Classname)s()
{
	// Add clean up code here
}
"""

shapeIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(C%(Classname)s);

class C%(Classname)s: public CIvfShape {
private:

public:
	C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", CIvfShape);
protected:
	virtual void createGeometry();
};

#endif
"""

shapeImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s::C%(Classname)s()
{
	// Add construction code here
}

void C%(Classname)s::createGeometry()
{
	// Rendering code

	glBegin(GL_QUADS);
	glNormal3d( 0.0, 0.0, 1.0);
	glVertex3d( 1.0, 1.0, 0.0);
	glVertex3d(-1.0, 1.0, 0.0);
	glVertex3d(-1.0,-1.0, 0.0);
	glVertex3d( 1.0,-1.0, 0.0);
	glEnd();
}
"""

quadSetIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfQuadSet.h>

IvfSmartPointer(C%(Classname)s);

class C%(Classname)s: public CIvfQuadSet {
private:
	double m_width;
	double m_height;

	void update();
public:
	C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", CIvfQuadSet);

	void setSize(double width, double height);
};

#endif
"""

quadSetImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s::C%(Classname)s()
{
	// Construct geometry here

	m_width = 1.0;
	m_height = 1.0;

	double ox = -m_width/2.0;
	double oy = -m_height/2.0;
	double oz = 0.0;

	// Add vertex coordinates

	addCoord(ox, oy, oz);
	addCoord(ox + m_width, oy, oz);
	addCoord(ox + m_width, oy + m_height, oz);
	addCoord(ox, oy + m_height, oz);

	// Add texture coordinates

	addTextureCoord(0.0, 0.0);
	addTextureCoord(1.0, 0.0);
	addTextureCoord(1.0, 1.0);
	addTextureCoord(0.0, 1.0);

	// Define vertex topology

	CIvfIndexPtr idx;

	idx = new CIvfIndex();
	idx->add(1, 2, 3, 0);

	addCoordIndex(idx);

	// Define texture topology

	idx = new CIvfIndex();
	idx->add(0, 1, 2, 3);

	addTextureIndex(idx);
}

void C%(Classname)s::setSize(double width, double height)
{
	m_width = width;
	m_height = height;

	update();
	updateBoundingSphere();
}

void C%(Classname)s::update()
{
	double ox = -m_width/2.0;
	double oy = -m_height/2.0;
	double oz = 0.0;

	setCoord(0, ox, oy, oz);
	setCoord(1, ox + m_width, oy, oz);
	setCoord(2, ox + m_width, oy, oz + m_height);
	setCoord(3, ox, oy, oz + m_height);
}
"""

triSetIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfTriSet.h>

IvfSmartPointer(C%(Classname)s);

class C%(Classname)s: public CIvfTriSet {
private:
	double m_width;
	double m_height;

	void update();
public:
	C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", CIvfTriSet);

	void setSize(double width, double height);
};

#endif
"""

triSetImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s::C%(Classname)s()
{
	// Construct geometry here

	m_width = 1.0;
	m_height = 1.0;

	double ox = -m_width/2.0;
	double oy = -m_height/2.0;
	double oz = 0.0;

	// Add vertex coordinates

	addCoord(ox, oy, oz);
	addCoord(ox + m_width, oy, oz);
	addCoord(ox + m_width, oy + m_height, oz);
	addCoord(ox, oy + m_height, oz);

	// Add texture coordinates

	addTextureCoord(0.0, 0.0);
	addTextureCoord(1.0, 0.0);
	addTextureCoord(1.0, 1.0);
	addTextureCoord(0.0, 1.0);

	// Define vertex topology

	CIvfIndexPtr idx;

	idx = new CIvfIndex();
	idx->add(0, 1, 2);
	idx->add(2, 3, 0);

	addCoordIndex(idx);

	// Define texture topology

	idx = new CIvfIndex();
	idx->add(0, 1, 2);
	idx->add(2, 3, 0);

	addTextureIndex(idx);
}

void C%(Classname)s::setSize(double width, double height)
{
	m_width = width;
	m_height = height;

	update();
	updateBoundingSphere();
}

void C%(Classname)s::update()
{
	double ox = -m_width/2.0;
	double oy = -m_height/2.0;
	double oz = 0.0;

	setCoord(0, ox, oy, oz);
	setCoord(1, ox + m_width, oy, oz);
	setCoord(2, ox + m_width, oy, oz + m_height);
	setCoord(3, ox, oy, oz + m_height);
}
"""

compositeIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfComposite.h>

IvfSmartPointer(C%(Classname)s);

class IVF_API C%(Classname)s: public CIvfComposite {
private:

public:
	C%(Classname)s();

	IvfClassInfo(\"C%(Classname)s\", CIvfComposite);
};

#endif
"""

compositeImplementationTempl = """#include \"%(Classname)s.h\"

#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>

C%(Classname)s::C%(Classname)s()
{
	// Add construction code here

	CIvfCubePtr cube = new CIvfCube();

	this->addChild(cube);

	CIvfSpherePtr sphere = new CIvfSphere();
	sphere->setRadius(0.5);
	sphere->setPosition(0.0, 0.5, 0.0);

	this->addChild(sphere);
}
"""

singletonIncludeTempl = """#ifndef _C%(Classname)s_h_
#define _C%(Classname)s_h_

#include <ivf/IvfBase.h>
#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(C%(Classname)s);

class C%(Classname)s: public CIvfBase {
private:
	static C%(Classname)s* m_instance;
	static CIvfSingletonDestroyer<C%(Classname)s> m_destroyer;
public:
	/** Returns the %(Classname)s singleton */
	static C%(Classname)s* getInstance();

	IvfClassInfo(\"C%(Classname)s\", CIvfBase);
protected:
	/** Protected constructor (do not use) */
	C%(Classname)s();
	friend class CIvfSingletonDestroyer<C%(Classname)s>;
};
#endif
"""

singletonImplementationTempl = """#include \"%(Classname)s.h\"

C%(Classname)s* C%(Classname)s::m_instance = 0;
CIvfSingletonDestroyer<C%(Classname)s> C%(Classname)s::m_destroyer;

C%(Classname)s* C%(Classname)s::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new C%(Classname)s(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

C%(Classname)s::C%(Classname)s()
{
	// Add protected construction code here
}
"""

class AppWindow(Frame):
	def __init__(self, parent):
		self._window = parent
		self._window.title("Ivf++ Class Generator")
		self._window.geometry("400x330")
		
		# Define variables
		
		self._outputDir = StringVar()
		self._outputDir.set(".")
		
		self._classType = IntVar()
		self._classType.set(1)
		
		self._className = StringVar()
		self._className.set("IvfShiny")
		
		self._baseClass = StringVar()
		self._baseClass.set("Base")
		
		self.initGUI()

		self._window.protocol('WM_DELETE_WINDOW', self._window.destroy)	
		
	def initGUI(self):
		frame = Frame(self._window, bd=5)
		frame.pack(expand=YES, fill=BOTH)

		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)
		
		label = Label(row, text="Class name", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtAppName = Entry(row, width=5, textvariable=self._className)
		self.edtAppName.pack(side=LEFT,fill=BOTH,expand=YES)
		
		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)
		
		label = Label(row, text="Base class (Custom)", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtAppName = Entry(row, width=5, textvariable=self._baseClass)
		self.edtAppName.pack(side=LEFT,fill=BOTH,expand=YES)

		row = Frame(frame, bd=2)
		row.pack(expand=NO, fill=BOTH)

		label = Label(row, text="Output dir", width=20, anchor=W, justify=RIGHT)
		label.pack(side=LEFT)
		self.edtTargetDir = Entry(row, width=5, textvariable=self._outputDir)
		self.edtTargetDir.pack(side=LEFT,fill=BOTH,expand=YES)
		self.btnSelectTargetDir = Button(row, text="...", command = self.onSelectOutputDir)
		self.btnSelectTargetDir.pack(side=LEFT)

		label = Label(frame, text="Class type", anchor=SW, justify=RIGHT, height=2)
		label.pack(expand=NO, fill=X, padx=4)
		
		row = Frame(frame, bd=2, relief=GROOVE)
		row.pack(expand=NO, fill=BOTH, padx=4, pady=4)

		Radiobutton(row, text="CIvfBase derived class", value=1, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="CIvfShape derived class", value=2, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="CIvfQuadSet derived class", value=3, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="CIvfTriSet derived class", value=4, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="CIvfComposite derived class", value=5, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="Singleton class", value=6, variable=self._classType).pack(anchor=W)
		Radiobutton(row, text="Custom derived class", value=7, variable=self._classType).pack(anchor=W)
		
		row = Frame(frame, bd=2)
		row.pack()
		
		#col = Frame(row, bd=0)
		#col.pack()
		
		self.btnGenerate = Button(row, text="Generate", command = self.onGenerate)
		self.btnGenerate.pack(side=LEFT, pady=4)
		
		self.btnClose = Button(row, text="Close", command = self.onClose)
		self.btnClose.pack(side=LEFT, expand=NO, pady=4)
			
	def onSelectOutputDir(self):
		newDir = tkFileDialog.askdirectory(initialdir = self._outputDir.get())
		if newDir!="":
			self._outputDir.set(newDir)
			
	def onGenerate(self):
		
		classType = ""
		className = ""
		baseClassName = ""
		
		if self._classType.get() == 1:
			classType = "base"
		elif self._classType.get() == 2:
			classType = "shape"
		elif self._classType.get() == 3:
			classType = "quadset"
		elif self._classType.get() == 4:
			classType = "triset"
		elif self._classType.get() == 5:
			classType = "composite"
		elif self._classType.get() == 6:
			classType = "singleton"
		elif self._classType.get() == 7:
			classType = "custom"

		currDir = os.getcwd()
		os.chdir(self._outputDir.get())
			
		className = self._className.get()
		baseClassName = self._baseClass.get()

		createClass(classType, className, baseClassName)
		
		os.chdir(currDir)
		
		tkMessageBox.showinfo(title="Application Generation", message="Application has been generated.")
	
	def onClose(self):
		self._window.destroy()
		
def	showGui():
	
	root = Tk()
	
	appWindow = AppWindow(root)
	
	root.mainloop()


def createEmptyFile(fileName):
	f = open(fileName, 'w')
	f.write('Please add more...\n')
	f.close();

def createClass(classType, className, baseClassName):
	includeFile = open(className+'.h', 'w')

	classNameTuple = {'Classname':className}
	baseClassNameTuple = {'Classname':className, 'BaseClass':baseClassName}

	if classType=='base':
		includeFile.write(baseIncludeTempl % classNameTuple)
		
	if classType=='shape':
		includeFile.write(shapeIncludeTempl % classNameTuple)

	if classType=='quadset':
		includeFile.write(quadSetIncludeTempl % classNameTuple)

	if classType=='triset':
		includeFile.write(triSetIncludeTempl % classNameTuple)

	if classType=='composite':
		includeFile.write(compositeIncludeTempl % classNameTuple)

	if classType=='singleton':
		includeFile.write(singletonIncludeTempl % classNameTuple)
		
	if classType=='custom':
		includeFile.write(customIncludeTempl % baseClassNameTuple)

	includeFile.close()

	sourceFile = open(className+'.cpp', 'w')

	if classType=='base':
		sourceFile.write(baseImplementationTempl % classNameTuple)

	if classType=='shape':
		sourceFile.write(shapeImplementationTempl % classNameTuple)

	if classType=='quadset':
		sourceFile.write(quadSetImplementationTempl % classNameTuple)

	if classType=='triset':
		sourceFile.write(triSetImplementationTempl % classNameTuple)

	if classType=='composite':
		sourceFile.write(compositeImplementationTempl % classNameTuple)

	if classType=='singleton':
		sourceFile.write(singletonImplementationTempl % classNameTuple)
		
	if classType=='custom':
		sourceFile.write(customImplementationTempl % baseClassNameTuple)

	sourceFile.close()

def checkArguments():
	result = ['shape', 'IvfExtension', '']

	if len(sys.argv)<2:
		print 'Ivf Classgen 0.6 - Ivf++ Class Template Generator'
		print 'Copyright (C) 2006 Division of Structural Mechanics'
		print 'Usage:'
		print '\tivfclassgen classtype|gui [classname] [baseclass]'
		print
		print 'values for classtype are:'
		print
		print '\tbase\t\tCIvfBase derived class'
		print '\tshape\t\tCIvfShape derived class'
		print '\tquadset\t\tCIvfQuadSet derived class'
		print '\ttriset\t\tCIvfTriSet derived class'
		print '\tcomposite\tCIvfComposite derived class'
		print '\tsingleton\tsingleton class'
		print '\tcustom\t\tCustom derived class'
		print 
		print '\tgui\t\tUser interface for generating classes'
		print
		sys.exit(0)

	if len(sys.argv)>1:
		if sys.argv[1]=='shape':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='composite':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='quadset':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='triset':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='base':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='singleton':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='custom':
			result[0] = sys.argv[1]
		elif sys.argv[1]=='gui':
			result[0] = sys.argv[1]
		else:
			print 'Class type '+sys.argv[1]+' not supported.'
			sys.exit(0)

	if len(sys.argv)>2:
		result[1] = sys.argv[2]
		
	if result[0] == 'custom':
		if len(sys.argv)!=4:
			print 'Custom classes need a base class.'
			sys.exit(0)
		else:
			result[2] = sys.argv[3]

	return result

if __name__ == "__main__":

	args = checkArguments()

	classType = args[0]
	
	if classType!="gui":
	
		className = args[1]
		baseClassName = args[2]
	
		createClass(classType, className, baseClassName)
		
	else:
		showGui()
	