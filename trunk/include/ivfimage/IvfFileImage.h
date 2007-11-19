//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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

#ifndef _CIvfFileImage_h_
#define _CIvfFileImage_h_

#include <ivf/IvfImage.h>

IvfSmartPointer(CIvfFileImage);

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
class IVFIMAGE_API CIvfFileImage : public CIvfImage {
private:
	char* m_fileName;
public:
	/** CIvfFileImage constructor */
	CIvfFileImage();

	/** CIvfFileImage constructor */
	CIvfFileImage(const char* name);

	/** CIvfFileImage destructor */
	virtual ~CIvfFileImage();

	IvfClassInfo("CIvfFileImage",CIvfImage);

	/** Set filename of file to load */
	void setFileName(const char* name);

	/** Return filename of file to load */
	const char* getFileName();

	/**
	 * Read file into image
	 *
	 * Derived classes should override this method.
	 */
	virtual bool read();
};
/** \example textures.cpp */

#endif 
