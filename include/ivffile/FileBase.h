//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivf/Base.h>
#include <ivf/Shape.h>

namespace ivf {

IvfSmartPointer(FileBase);

/**
 * Base class for file readers and writers
 *
 * The class implements basic file name handling and
 * functions for retrieving and setting shapes.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API FileBase : public Base {
private:
	std::string	m_fileName;
	ShapePtr m_shape;
protected:
	std::fstream m_inputFile;
public:
	/** FileBase constructor */
	FileBase();

	/** FileBase destructor */
	virtual ~FileBase();

	IvfClassInfo("FileBase",Base);
	IvfStdFactory(FileBase);

	/** Sets the filename of the file to be read/written */
	void setFileName(const std::string& fileName);

	/** Returns the filename of the file to be read/written */
	const std::string getFileName();

	/** Sets the shape to be exported to file */
	void setShape(Shape* shape);

	/** Returns the shape read */
	Shape* getShape();
};
/** \example ac3dreader.cpp
 * This example shows how to read a AC3D file from file
 * and view in Ivf++
 */

}