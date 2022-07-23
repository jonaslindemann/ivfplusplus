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

#include <ivf/Image.h>

namespace ivf {

IvfSmartPointer(FileImage);

/**
 * Image file loader base class
 *
 * This class implements the basic interface for
 * derived image loading classes. Images are loaded by
 * instantiating an image loader, setting the filename
 * using the setFileName() method and then invoking the
 * read() method.
 *
 * @author Jonas Lindemann
 */
class IVFIMAGE_API FileImage : public Image {
private:
	std::string m_fileName;
public:
	/** FileImage constructor */
	FileImage();

	/** FileImage constructor */
	FileImage(const std::string& name);

	/** FileImage destructor */
	virtual ~FileImage();

	IvfClassInfo("FileImage",Image);
	IvfStdFactory(FileImage);

	/** Set filename of file to load */
	void setFileName(const std::string& name);

	/** Return filename of file to load */
	const std::string getFileName();

	/**
	 * Read file into image
	 *
	 * Derived classes should override this method.
	 */
	virtual bool read();

	bool load(const std::string& filename);
};
/** \example textures.cpp */

}