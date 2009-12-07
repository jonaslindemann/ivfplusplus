#!/bin/env python

import os, sys, string, shutil

from ivfversion import *

class SourcePackage:
	def __init__(self, version):
		self.pkgVersion = version
		self.pkgLocation = ""
		self.pkgTarballs = []
		self.pkgDir = ""
		self.pkgURLs = []
		self.pkgConfigOptions = ""
		self.pkgBuildDir = "./depend"
		self.pkgConfigCmd = "configure"
		self.pkgMakeDir = "."
		self.pkgConfigInMakeDir = True
		self.pkgIncludeDir = ""
		self.pkgLibDir = ""
		self.pkgPatches = []
		self.pkgDirPatches = []


	def setLocation(self, location):
		self.pkgLocation = location

	def getLocation(self):
		return self.pkgLocation

	def setVersion(self, version):
		self.pkgVersion = version

	def getVersion(self):
		return self.pkgVersion

	def setConfigOptions(self, options):
		self.pkgConfigOptions = options

	def addConfigOptions(self, options):
		self.pkgConfigOptions = self.pkgConfigOptions + " " + options

	def getConfigOptions(self):
		return self.pkgConfigOptions

	def setDir(self, packageDir):
		if packageDir.find("""%s""")!=-1:
			self.pkgDir = packageDir % self.pkgVersion
		else:
			self.pkgDir = packageDir

	def setBuildDir(self, buildDir):
		self.pkgBuildDir = buildDir

	def setConfigCommand(self, configCmd):
		self.pkgConfigCmd = configCmd

	def setMakeDir(self, makeDir):
		self.pkgMakeDir = makeDir

	def setConfigInMakeDir(self, flag):
		self.pkgConfigInMakeDir = flag

	def setIncludeDir(self, includeDir):
		self.pkgIncludeDir = includeDir

	def setLibDir(self, libDir):
		self.pkgLibDir = libDir

	def addTarball(self, tarball):
		versionTarball = tarball % self.pkgVersion
		self.pkgTarballs.append(versionTarball)
		self.pkgURLs.append(os.path.join(self.pkgLocation, versionTarball))

	def addPatch(self, patchLoc, patchFile):
		self.pkgPatches.append([os.path.abspath(patchLoc), patchFile])
		
	def addDirPatch(self, patchLoc, patchDir):
		self.pkgDirPatches.append([os.path.abspath(patchLoc), patchDir])

	def download(self):

		i = 0

		for tarball in self.pkgTarballs:
			if not os.path.isfile(self.pkgBuildDir+"/"+tarball):
				print "----------------------------------------"
				print "Downloading: ", tarball
				print "----------------------------------------"
				os.system("wget %s" % self.pkgURLs[i])
				os.rename(tarball, self.pkgBuildDir+"/"+tarball)
			i=i+1

	def extract(self):

		if not os.path.exists(self.pkgBuildDir+"/"+self.pkgDir):
			for tarball in self.pkgTarballs:
				print "----------------------------------------"
				print "Extracting: ", tarball
				print "----------------------------------------"
				if os.path.isfile(self.pkgBuildDir+"/"+tarball):
					os.system("tar xzf %s -C %s" % (self.pkgBuildDir+"/"+tarball, self.pkgBuildDir))

	def patch(self):
		
		for patch in self.pkgPatches:
			patchLoc = patch[0]
			patchFile = patch[1]

			os.system("patch -N -s %s %s" % (self.pkgBuildDir+"/"+self.pkgDir+"/"+patchFile, patchLoc))
			
		for patch in self.pkgDirPatches:
			patchLoc = patch[0]
			patchDir = patch[1]
			
			os.system("pushd %s;patch -p0 < %s;popd" % (self.pkgBuildDir+"/"+self.pkgDir+"/"+patchDir, patchLoc))

	def build(self):
		print "----------------------------------------"
		print "Building: ", self.pkgDir
		print "----------------------------------------"
		currDir = os.getcwd()
		os.chdir(self.pkgBuildDir+"/"+self.pkgDir)
		os.chdir(self.pkgMakeDir)
		os.system("make")
		os.chdir(currDir)

	def install(self):
		print "----------------------------------------"
		print "Installing: ", self.pkgDir
		print "----------------------------------------"
		currDir = os.getcwd()
		os.chdir(self.pkgBuildDir+"/"+self.pkgDir)
		os.chdir(self.pkgMakeDir)
		os.system("make install")
		os.chdir(currDir)

	def configure(self):
		print "----------------------------------------"
		print "Configuring: ", self.pkgDir
		print "----------------------------------------"
		currDir = os.getcwd()
		os.chdir(self.pkgBuildDir+"/"+self.pkgDir)
		if self.pkgConfigInMakeDir:
			os.chdir(self.pkgMakeDir)

		envVariables = ""

		if self.pkgIncludeDir!="":
			envVariables = envVariables + "CFLAGS=-I%s " % self.pkgIncludeDir

		if self.pkgLibDir!="":
			envVariables = envVariables + "LDFLAGS=-L%s " % self.pkgLibDir

		print "Executing: ", envVariables+" ./"+self.pkgConfigCmd+" "+self.pkgConfigOptions
		print "Current dir: ", os.getcwd()
		os.system(envVariables+" ./"+self.pkgConfigCmd+" "+self.pkgConfigOptions)
		os.chdir(currDir)

class FltkPackage(SourcePackage):
	def __init__(self, version):
		SourcePackage.__init__(self, version)
		self.setLocation("http://ftp.rz.tu-bs.de/pub/mirror/ftp.easysw.com/ftp/pub/fltk/%s" % version)
		# self.setLocation("http://mirror.nu6.org//ftp.easysw.com/pub/fltk/%s" % version)
		self.setDir("fltk-%s")
		self.addTarball("fltk-%s-source.tar.gz")
		self.setConfigOptions("--enable-shared")
		
class GlePackage(SourcePackage):
	def __init__(self, version):
		SourcePackage.__init__(self, version)
		self.setLocation("http://heanet.dl.sourceforge.net/sourceforge/gle")
		self.addTarball("gle-%s.tar.gz")
		self.setDir("gle-%s")
		self.addPatch("./patches/gle_remove_glut_test.patch", "configure")
		self.addPatch("./patches/gle_no_examples.patch", "Makefile.in")
		self.addDirPatch("./patches/gle_apple_source_fixes.patch", "src")

class FTGLPackage(SourcePackage):
	def __init__(self, version):
		SourcePackage.__init__(self, version)
		self.setLocation("http://mesh.dl.sourceforge.net/sourceforge/ftgl")
		#self.setLocation("http://ftp.sunet.se/pub/os/Linux/distributions/gentoo/distfiles")
		self.addTarball("ftgl-%s.tar.gz")
		self.setDir("ftgl-%s")
		#self.setMakeDir("unix")
		self.setConfigOptions("--enable-shared")
			
	def extract(self):
		SourcePackage.extract(self)
		self.pkgDir = self.pkgDir.replace("-rc", "~rc")
		print self.pkgDir
		
	def install(self):
		self.pkgDir = self.pkgDir.replace("-rc", "~rc")
		SourcePackage.install(self)
		
		

class IvfBuildSystem:
	def __init__(self):
		self._nmake = False
		self._buildType = "static_debug"
		self._update = False
		self._dependBuildType = "all"
		self._dependPrefixDir = ""
		self._nmakeAvailable = False
		self._cmakeAvailable = False
		self._doxygenAvailable = False
		self._ivfPrefix = ""
		
	def setBuildType(self, buildType):
		self._buildType = buildType
		
	def setUseNMake(self, flag):
		self._nmake = flag
		
	def setUpdateBuild(self, flag):
		self._update = flag
		
	def setDependBuildType(self, buildType):
		self._dependBuildType = buildType
		
	def setDependPrefixDir(self, prefixDir):
		self._dependPrefixDir = prefixDir
	
	def setPrefixDir(self, prefixDir):
		self._ivfPrefixDir = prefixDir
		
	def depends(self):
		
		print
		print "Ivf++ dependency build."
		print
		
		# Check arguments
	
		if not os.path.isdir("./depend"):
			os.mkdir("./depend")
	
		dependPackages= []
		packages = []
	
		glePackage = GlePackage("3.1.0")
		glePackage.setIncludeDir(os.path.abspath("./depend/freeglut-2.4.0/include"))
		libDir = os.path.abspath("./depend/freeglut-2.4.0/src")
		glePackage.setLibDir(os.path.join(libDir, ".libs"))
		fltkPackage = FltkPackage("1.1.9")
		ftglPackage = FTGLPackage("2.1.3-rc5")
	
		#dependPackages.append(freeglutPackage)
		dependPackages.append(glePackage)
		dependPackages.append(fltkPackage)
		dependPackages.append(ftglPackage)
	
		#packages.append(freeglutPackage)
		packages.append(glePackage)
		packages.append(fltkPackage)
		packages.append(ftglPackage)
	
		if self._dependBuildType=="clean":
			if os.path.isdir("./depend"):
				os.system("rm -rf ./depend")
				sys.exit(0)
	
		if self._dependBuildType=="download" or self._dependBuildType=="all":
	
			# Download and extract all packages
	
			for package in packages:
				package.download()
				package.extract()
				package.patch()
	
		if self._dependBuildType=="build" or self._dependBuildType=="all":
	
			# Download and extract all packages
		
			for package in packages:
				package.download()
				package.extract()
				package.patch()
			
			# Build all dependencies
		
			for package in dependPackages:
				if self._dependPrefixDir=="":
					package.addConfigOptions("--prefix=%s" % os.path.abspath("./depend"))
				else:
					package.addConfigOptions("--prefix=%s" % self._dependPrefixDir)
				package.configure()
				package.build()
	
		if self._dependBuildType=="install" or self._dependBuildType=="all":
	
			for package in dependPackages:
				package.install()
			
	def getBuildDir(self):
		
		return "%s_%s_build" % (sys.platform, self._buildType)

	def getCMakeCmd(self):
		
		if self._buildType == "shared_debug":
			shared = "ON"
			debug = "Debug"
			
		if self._buildType == "shared_release":
			shared = "ON"
			debug = "Release"
	
		if self._buildType == "static_debug":
			shared = "OFF"
			debug = "Debug"
	
		if self._buildType == "static_release":
			shared = "OFF"
			debug = "Release"
			
		cmakeTemplate = ""
	
		if sys.platform == "win32":
			if self._nmake:
				cmakeTemplate = 'cmake -G"NMake Makefiles" -DCMAKE_CONFIGURATION_TYPES:STRING=%s -DCMAKE_BUILD_TYPE:STRING=%s -DBUILD_SHARED_LIBS:BOOL=%s ..'
			else:	
				cmakeTemplate = 'cmake -DCMAKE_CONFIGURATION_TYPES:STRING=%s -DCMAKE_BUILD_TYPE:STRING=%s -DBUILD_SHARED_LIBS:BOOL=%s ..'
			return cmakeTemplate % (debug, debug, shared)
		else:
			cmakeTemplate = "cmake -DCMAKE_BUILD_TYPE:STRING=%s -DBUILD_SHARED_LIBS:BOOL=%s .."
			return cmakeTemplate % (debug, shared)

	def configure(self):
		
		print
		print "Configuring Ivf++ build."
		print
		
		buildDir = self.getBuildDir()
			
		currDir = os.getcwd()
			
		if not self._update:
			if os.path.exists("./"+buildDir):
				shutil.rmtree("./"+buildDir)
			os.mkdir("./"+buildDir)
			
		os.chdir(buildDir)	
			
		os.system(self.getCMakeCmd())
			
		os.chdir(currDir)
	
		if sys.platform == "win32":
			
			if self._nmake:
				
				if os.path.exists("./bin"):
					shutil.rmtree("./bin")
				shutil.copytree("./data", "./bin")
			
			else:
	
				if sys.argv[2] == "shared_debug":
					if not os.path.exists("./bin"):
						os.mkdir("./bin")
					if os.path.exists("./bin/debug"):
						shutil.rmtree("./bin/debug")
					shutil.copytree("./data", "./bin/debug")
			
				if sys.argv[2] == "shared_release":
					if not os.path.exists("./bin"):
						os.mkdir("./bin")
					if os.path.exists("./bin/release"):
						shutil.rmtree("./bin/release")
					shutil.copytree("./data", "./bin/release")
			
				if sys.argv[2] == "static_debug":
					if not os.path.exists("./bin"):
						os.mkdir("./bin")
					if os.path.exists("./bin/debug"):
						shutil.rmtree("./bin/debug")
					shutil.copytree("./data", "./bin/debug")
			
				if sys.argv[2] == "static_release":
					if not os.path.exists("./bin"):
						os.mkdir("./bin")
					if os.path.exists("./bin/release"):
						shutil.rmtree("./bin/release")
					shutil.copytree("./data", "./bin/release")
				
		else:
			if os.path.exists("./bin"):
				shutil.rmtree("./bin")
			shutil.copytree("./data", "./bin")
			
	
		#os.system("cp -Rp ./data/* ./bin")
		
		if sys.platform == "win32":
			if self._nmake:
				print
				print 'Now run "ivfbuild build", to build Ivf++.'
			else:
				print
				print 'Please use the ivf.sln and examples.sln solutions in'
				print 'the win32_build directory to build Ivf++ with the'
				print 'Visual Studio IDE.'
		else:
			print
			print 'Now run "./ivfbuild build", to build Ivf++.'
			
				
		

	def build(self):
		
		print
		print "Building Ivf++."
		print 
	
		currDir = os.getcwd()
		
		if sys.platform == "linux2":
		
			if os.path.exists("./"+self.getBuildDir()):
				os.chdir(self.getBuildDir())
				os.system("make")
				os.chdir(currDir)
				print 'Now run "./ivfbuild install" as root to install Ivf++.'
			else:
				print "Please configure Ivf++ first. (ivfbuild configure ...)"
				sys.exit(-1)
				
		if sys.platform == "darwin":
		
			if os.path.exists("./"+self.getBuildDir()):
				os.chdir(self.getBuildDir())
				os.system("make")
				os.chdir(currDir)
				print 'Now run "./ivfbuild install" as root to install Ivf++.'
			else:
				print "Please configure Ivf++ first. (ivfbuild configure ...)"
				sys.exit(-1)

		if sys.platform == "win32":
			
			# The default build is for VS IDE solution files. These must be
			# built from the IDE. The second build type is NMAKE which can be
			# built automatically.
			
			if os.path.exists("./"+self.getBuildDir()):
				if os.path.exists("./"+self.getBuildDir()+"/ivf.sln"):
					print "Please use Visual Studio .NET IDE to build the solution files, ivf.sln and"
					print "examples.sln to build Ivf++ and associated examples."
				elif os.path.exists("./"+self.getBuildDir()+"/Makefile"):
					
					# Check for correct VS setup
					
					if not os.environ.has_key("VSINSTALLDIR"):
						print "Please run the vsvars32.bat to setup the Visual Studio build environment."
						sys.exit(-1)
						
					# Setup include and library search paths
					
					ivfIncludes = currDir+"\\include;"+currDir+"\\include\\vc"
					ivfDependIncludes = currDir+"\\..\\ivf-depend\\include"
					ivfLibs = currDir+"\\lib"
					ivfDependLibs = currDir+"\\..\\ivf-depend\\lib"
					
					if os.environ.has_key("INCLUDE"):
						os.environ["INCLUDE"] = os.environ["INCLUDE"]+";"+ivfIncludes+";"+ivfDependIncludes
					else:
						print "Please run the vsvars32.bat to setup the Visual Studio build environment."
						sys.exit(-1)
					if os.environ.has_key("LIB"):
						os.environ["LIB"] = os.environ["LIB"]+";"+ivfLibs+";"+ivfDependLibs
					else:
						print "Please run the vsvars32.bat to setup the Visual Studio build environment."
						sys.exit(-1)

					print os.environ["INCLUDE"]
					print os.environ["INCLUDE"]
						
					# Execute the build.
						
					os.chdir("./"+self.getBuildDir())
					os.system("nmake /f Makefile")
					os.chdir(currDir)
				else:
					print "Something went wrong during configuration. Please try again."
					sys.exit(-1)
			else:
				print "Please configure Ivf++ first. (ivfbuild configure ...)"
				sys.exit(-1)
				
	def install(self):
		
		print
		print "Installing Ivf++."
		print 
	
		currDir = os.getcwd()
		
		if sys.platform == "linux2":
		
			if os.path.exists("./"+self.getBuildDir()):
				os.chdir(self.getBuildDir())
				os.system("make install")
				os.chdir(currDir)
			else:
				print "Please build Ivf++ first. (./ivfbuild build ...)"
				sys.exit(-1)
				
		if sys.platform == "win32":
			
			print "Install option not available on Windows."
			
	def findBuildDirs(self):
		
		dirItems = os.listdir(".")
		
		buildDirs = []
		
		for dirItem in dirItems:
			if os.path.isdir(dirItem):
				if dirItem.find("_build")!=-1:
					buildDirs.append(dirItem)
					
		return buildDirs

	def distClean(self):
		
		self.clean()

		if os.path.exists("./dist"):
			shutil.rmtree("./dist")
		

	def clean(self):
		
		print 
		print "Cleaing build files."
		print
		
		buildDirs = self.findBuildDirs()
		
		for buildDir in buildDirs:
			if os.path.exists("./"+buildDir):
				shutil.rmtree("./"+buildDir)
		
		if os.path.exists("./depend"):
			shutil.rmtree("./depend")
	
		if os.path.exists("./bin"):
			shutil.rmtree("./bin")
	
		if os.path.exists("./lib"):
			shutil.rmtree("./lib")
			
		if os.path.exists("./doc/html"):
			shutil.rmtree("./doc/html")
			
	def docs(self):
		
		# Is Doxygen availble
		
		if not self._doxygenAvailable:
			print "Doxygen not available to generate documentation."
			sys.exit(-1)

		currDir = os.getcwd()

		os.chdir("doc")
		os.system("makedoc")

		os.chdir(currDir)
			
	def makeDist(self):
		
		self.clean()
		os.system("dos2unix *.py")
		os.system("dos2unix utils/*.py")
		currDir = os.getcwd()
		distDir = os.path.basename(currDir)
		os.chdir("..")
		if os.path.exists(IvfVersionString % (IvfMajor, IvfMinor, IvfRelease)):
			shutil.rmtree(IvfVersionString % (IvfMajor, IvfMinor, IvfRelease))
		os.rename(distDir, IvfVersionString % (IvfMajor, IvfMinor, IvfRelease))
		os.system("tar cvzf %s.tar.gz %s" % (IvfVersionString % (IvfMajor, IvfMinor, IvfRelease), IvfVersionString % (IvfMajor, IvfMinor, IvfRelease)))
		os.rename(IvfVersionString % (IvfMajor, IvfMinor, IvfRelease), distDir)
		shutil.move("%s.tar.gz" % (IvfVersionString % (IvfMajor, IvfMinor, IvfRelease)), "ivf"	)
		os.chdir(currDir)
			
	def existsInPath(self, filename):
		p = os.environ['PATH']
	
		pathlist = p.split (os.pathsep)
	
		for path in pathlist:
			f = os.path.join(path, filename)
			if os.path.isfile(f):
				return True
		
		return False
			
	def checkRequirements(self):
		
		print
		print "Checking build system requirements."
		print 
		
		self._cmakeAvailable = False
		self._nmakeAvailable = False
		self._doxygenAvailable = False
		
		if sys.platform == "win32":
			self._cmakeAvailable = self.existsInPath("cmake.exe")
		else:
			self._cmakeAvailable = self.existsInPath("cmake")
		
		if self._cmakeAvailable:
			print "CMake, found."
		else:
			print "CMake, not found."
			print
			print "CMake must be installed to build Ivf++"
			print
			sys.exit(-1)
		
		if sys.platform == "win32":
			self._nmakeAvailable = self.existsInPath("nmake.exe")
			
			if self._nmakeAvailable:
				print "NMake, found. Win32 make file build supported."
			else:
				print "NMake, not found. Please run the vsvars32.bat to enable NMake."
				
		if sys.platform == "win32":
			self._doxygenAvailable = self.existsInPath("doxygen.exe")
		else:
			self._doxygenAvailable = self.existsInPath("doxygen")
			
		if self._doxygenAvailable:
			print "Doxygen, found. Documenation generation available."
		else:
			print "Doxygen, not found. Documentation generation disabled."
				
		print	
		
		
					
def showUsage():
	print "ivfbuild depends|configure|update|build|docs|install|clean|distclean|dist [..more options..]"

if __name__ == "__main__":
	
	ivfBuildSystem = IvfBuildSystem()
	ivfBuildSystem.checkRequirements()
		
	if len(sys.argv)==1:
		showUsage()
		sys.exit(-1)

	if sys.argv[1] == "depends":

		if len(sys.argv)==2:
			print "ivfuild depends download|build|install|clean|all prefix"
			sys.exit(-1)
			
		if sys.platform == "win32":
			print "Dependency building is currently not supported on WIN32."
			print "Please use the prebuild dependency package instead."
			sys.exit(-1)
		else:
			if sys.argv[2] == "download":
				ivfBuildSystem.setDependBuildType(sys.argv[2])
			elif sys.argv[2] == "build":
				ivfBuildSystem.setDependBuildType(sys.argv[2])
			elif sys.argv[2] == "install":
				ivfBuildSystem.setDependBuildType(sys.argv[2])
			elif sys.argv[2] == "clean":
				ivfBuildSystem.setDependBuildType(sys.argv[2])
			elif sys.argv[2] == "all":
				ivfBuildSystem.setDependBuildType(sys.argv[2])
			else:
				print "Build type ", sys.argv[2], " not supported."
				sys.exit(-1)
				
			if len(sys.argv)==4:
				ivfBuildSystem.setDependPrefixDir(sys.argv[3])
			
		ivfBuildSystem.depends()
		
	if sys.argv[1] == "configure":
	
		if len(sys.argv)==2:
			print "ivfuild configure shared_debug|shared_release|static_debug|static_release [nmake|prefix]"
			sys.exit(-1)
			
		if sys.argv[2] == "shared_debug":
			ivfBuildSystem.setBuildType(sys.argv[2])
		elif sys.argv[2] == "shared_release":
			ivfBuildSystem.setBuildType(sys.argv[2])
		elif sys.argv[2] == "static_debug":
			ivfBuildSystem.setBuildType(sys.argv[2])
		elif sys.argv[2] == "static_release":
			ivfBuildSystem.setBuildType(sys.argv[2])
		else:
			print "Build type ", sys.argv[2], " not supported."
			sys.exit(-1)
			
		if len(sys.argv)==4:
			if sys.platform == "win32":
				if sys.argv[3] == "nmake":
					ivfBuildSystem.setUseNMake(True)
			else:
				pass
				
		ivfBuildSystem.configure()

	if sys.argv[1] == "update":
		ivfBuildSystem.setUpdateBuild(True)
		ivfBuildSystem.configure()
	
	if sys.argv[1] == "build":
		ivfBuildSystem.build()
		
	if sys.argv[1] == "docs":
		ivfBuildSystem.docs()
	
	if sys.argv[1] == "clean":
		ivfBuildSystem.clean()

	if sys.argv[1] == "distclean":
		ivfBuildSystem.distClean()

	if sys.argv[1] == "install":
		ivfBuildSystem.install()

	if sys.argv[1] == "dist":
		ivfBuildSystem.makeDist()
		
