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

IvfSmartPointer(CBmpImage);

/**
 * BMP image loader class
 *
 * This class implements a loader for Windows BMP image
 * files.
 */
class IVFEXT_API CBmpImage : public CFileImage {
private:
	bool m_alphaChannel;
	unsigned char *imageData;
    int width_ret;
    int height_ret;
    int numComponents_ret;



struct bmpheader {
    short FileType; //always MB
    short siz[2]; // a dword for whole file size
    short Reserved1, Reserved2; //reserved for future purposes
    short offset[2]; //offset to image in bytes
};

struct BMPInfo {
    long width;   //width of the image in pixels
    long height;    //   height of the image in pixels
    short planes;       //:word: number of planes (always 1)
    short Colorbits;       //word: number of bits used to describe color in each pixel
    long compression;  //compression used
    long ImageSize;    //image size in bytes
    long XpixPerMeter; //pixels per meter in X
    long YpixPerMeter; //pixels per meter in Y
    long ColorUsed;   //number of colors used
    long Important;   //number of "important" colors
    long os2stuff[6]; // allows os2.1 with 64 bytes to be read.  Dont know what these are yet.
};

/* byte order workarounds *sigh* */
void swapbyte(long *i);
void swapbyte(unsigned long *i);
void swapbyte(float *i);
void swapbyte(unsigned short *i);
void swapbyte(short *i);

unsigned char * bmp_load(const char *filename,
                         int *width_ret,
                         int *height_ret,
                         int *numComponents_ret);

public:
	/** CBmpImage constructor */
	CBmpImage();

	/** CBmpImage constructor */
	CBmpImage(const std::string& name);

	/** CBmpImage destructor */
	virtual ~CBmpImage();

	IvfClassInfo("CBmpImage",CFileImage);

	/** Reads Bmp image */
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