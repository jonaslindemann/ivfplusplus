//
// Copyright 1999-2003 by John Aspras
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
// Written by John Aspras
//

#pragma once

#include <ivfimage/FileImage.h>

namespace ivf {

IvfSmartPointer(CTgaImage);

/**
 * TGA image loader class
 *
 * This class implements a TGA image loader
 */
class IVFEXT_API CTgaImage : public CFileImage {
private:
	bool m_alphaChannel;

typedef	struct
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;


typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;

Texture texture;
TGAHeader tgaheader;									// TGA header
TGA tga;											    // TGA image data

bool LoadTGA(Texture * texture, char * filename);		// Load a TGA file

bool LoadUncompressedTGA(Texture *, char *, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(Texture *, char *, FILE *);		// Load a Compressed file

public:
	/** CTgaImage constructor */
	CTgaImage();

	/** CTgaImage constructor */
	CTgaImage(const std::string& name);

	/** CTgaImage destructor */
	virtual ~CTgaImage();

	IvfClassInfo("CTgaImage",CFileImage);

	/** Reads Tga image */
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