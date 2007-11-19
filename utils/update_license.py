#!/bin/env python

import os, sys

licenseTemplate = """//
// Copyright %s-%s by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

"""

def addLicense(filename, fromYear, toYear):
	
	sourceFile = file(filename, "rU")
	lines = sourceFile.readlines()
	sourceFile.close()

	sourceFile = file(filename, "w")
	sourceFile.write(licenseTemplate % (fromYear, toYear))

	for line in lines:
		sourceFile.write(line)

	sourceFile.close()
	

def updateLicense(filename, fromYear, toYear):

	sourceFile = file(filename, "rU")
	lines = sourceFile.readlines()
	sourceFile.close()

	newLines = []

	for line in lines:
		newLine = line
		if line.find("//")!=-1:
			if line.find("Copyright")!=-1 and line.find("by Structural Mechanics")!=-1:
				newLine = "// Copyright %s-%s by Structural Mechanics, Lund University.\n" % (fromYear, toYear)
		newLines.append(newLine)

	newSourceFile = file(filename, "w")
	for line in newLines:
		newSourceFile.write(line)
	newSourceFile.close()

def checkLicense(filename):
	
	sourceFile = file(filename, "r")
	lines = sourceFile.readlines()
	sourceFile.close()

	firstComment = True
	readMoreComments = True

	commentPart = []

	for line in lines:
		if line.find("//")!=-1:
			if firstComment:
				firstComment = False
			if readMoreComments:
				commentPart.append(line.strip())
		else:
			if not firstComment:
				readMoreComments = False

	if len(commentPart)==0:
		return False
	else:
		foundCopyright = False
		for line in commentPart:
			if line.find("Copyright")!=-1:
				foundCopyright = True
		return foundCopyright
				
def updateTree(top):
	for root, dirs, files in  os.walk(top):
		for name in files:
			rootName, ext = os.path.splitext(name)	
			if ext == ".h" or ext == ".cpp":
				filename = os.path.join(root, name)
				haveLicense = checkLicense(filename)		
				if haveLicense == True:
					print filename, " : Have license information."
					updateLicense(filename, "1999", "2007")
				else:
					print filename, " : Contains no license information."
					addLicense(filename, "1999", "2007")

updateTree('../include')
updateTree('../src')
