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

#include <ivfimage/FileImage.h>

namespace ivf {

IvfSmartPointer(SgiImage);

/**
 * SGI file loader 
 *
 * This class loads a sgi image. The loader
 * can handle both uncompressed and RLE encoded
 * sgi/rgb files.
 *
 * @author Jonas Lindemann
 */
class IVFIMAGE_API SgiImage : public FileImage {
private:
	bool m_alphaChannel;

	unsigned char getbyte(FILE* inf);
	unsigned long getlong(FILE* inf);
	unsigned short getshort(FILE *inf);
	void expandrow(unsigned char *optr, unsigned char *iptr, int z);
	void convertLong(GLuint *array, unsigned int length);
public:
	/** SgiImage constructor */
	SgiImage();

	/** SgiImage constructor */
	SgiImage(const char* name);

	/** SgiImage destructor */
	virtual ~SgiImage();

	IvfClassInfo("SgiImage",FileImage);
	IvfStdFactory(SgiImage);

	/** Reads sgi image */
	virtual bool read();

	/** 
	 * Alpha channel flag
	 *
	 * If set to true the alpha channel will be read, if
	 * it exists. (default = false)
	 */
	void setAlphaChannel(bool flag);
};

}