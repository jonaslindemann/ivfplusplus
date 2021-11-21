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

#include <ivfimage/SgiImage.h>

using namespace ivf;

CSgiImage::CSgiImage()
{
	m_alphaChannel = false;
}

CSgiImage::CSgiImage(const char *name)
:CFileImage(name)
{
	m_alphaChannel = false;
}

CSgiImage::~CSgiImage()
{

}

void CSgiImage::expandrow(unsigned char *optr, unsigned char *iptr, int z)
{
	unsigned char pixel, count;

	//optr += z;
	while(1) {
		pixel = *iptr++;
		if ( !(count = (pixel & 0x7f)) )
			return;

		if(pixel & 0x80)
		{
			while(count--)
			{
				*optr = *iptr++;
				optr++;
				//optr+=4;
			}
		}
		else
		{
			pixel = *iptr++;
			while(count--)
			{
				*optr = pixel;
				optr++;
				//optr+=4;
			}
		}
	}
}

void CSgiImage::convertLong(GLuint *array, unsigned int length)
{
    unsigned long b1, b2, b3, b4;
    unsigned char *ptr;

    ptr = (unsigned char *)array;
    while (length--) {
	b1 = *ptr++;
	b2 = *ptr++;
	b3 = *ptr++;
	b4 = *ptr++;
	*array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
    }
}

bool CSgiImage::read()
{
	unsigned char buf[80];
	unsigned long i, j, k;
	FILE* inf;
#ifdef WIN32
	errno_t err;
#endif

	//
	// Open file
	//

	if (this->getFileName()=="")
		return false;

#ifdef WIN32
	err = fopen_s(&inf, this->getFileName().c_str(), "rb");

	if (err)
		return false;
#else
	inf = fopen(this->getFileName().c_str(), "rb");
	
	if (inf==nullptr)
		return false;
#endif

	unsigned short magic;
	unsigned char storage;
	unsigned char bpc;
	unsigned short dimension;
	unsigned short xSize;
	unsigned short ySize;
	unsigned short zSize;
	unsigned long pixMin;
	unsigned long pixMax;
	unsigned long colorMap;
	unsigned char dummy;
	unsigned short shortValue;
	unsigned char byteValue;
	size_t readBytes;

	//
	// Read file header
	//

	magic = getshort(inf);

	if (magic!=474)
	{
		fclose(inf);
		return false;
	}

	storage = getbyte(inf);
	bpc = getbyte(inf);
	dimension = getshort(inf);
	xSize = getshort(inf);
	ySize = getshort(inf);
	zSize = getshort(inf);
	pixMin = getlong(inf);
	pixMax = getlong(inf);
	readBytes = fread(buf,4,1,inf);
	readBytes = fread(buf,80,1,inf);
	colorMap = getlong(inf);
	for (i=0; i<404; i++)
		dummy = getbyte(inf);

	//
	// Intialize image map
	//

	if (m_alphaChannel)
		this->setChannels(zSize+1);
	else
		this->setChannels(zSize);

	this->setSize(xSize, ySize);

	//
	// Read image
	//

	using namespace std;

	if (storage==0)
	{
		//
		// Storage VERBATIM
		//

		for (j=0; j<zSize; j++)
		{
			for (i=0; i<ySize; i++)
			{
				for(k=0; k<xSize; k++)
				{
					if (bpc==1)
					{
						byteValue = getbyte(inf);
						this->setValue(k, i, j, (GLubyte)byteValue);
						if (feof(inf))
						{
							fclose(inf);
							return false;
						}
					}
					else
					{
						shortValue = getshort(inf);
						this->setValue(k, i, j, (GLubyte)shortValue);
						if (feof(inf))
						{
							fclose(inf);
							return false;
						}
					}
				}
			}
		}
	}
	else
	{
		//
		// Storage RLE
		//

		unsigned long *starttab;
		unsigned long *lengthtab;

		unsigned long tablen = ySize*zSize*sizeof(unsigned long);
		unsigned long entries = ySize*zSize;

		starttab = (unsigned long *)malloc(tablen);
		lengthtab = (unsigned long *)malloc(tablen);

		fseek(inf, 512, SEEK_SET);

		for (i=0; i<entries; i++)
			starttab[i] = getlong(inf);

		for (i=0; i<entries; i++)
			lengthtab[i] = getlong(inf);

		unsigned long rowSize = xSize*sizeof(unsigned char);
		unsigned char *inputRow = (unsigned char*)malloc(rowSize*256);
		unsigned char *outputRow = (unsigned char*)malloc(rowSize*256);

		unsigned long x, y, z;

		for (y=0; y<ySize; y++)
		{
			for (z=0; z<zSize; z++)
			{
				fseek(inf, (unsigned long)starttab[y+z*ySize], SEEK_SET);

				unsigned long length = lengthtab[y+z*ySize];

				for (i=0; i<length; i++)
					inputRow[i] = getbyte(inf);

				this->expandrow(outputRow, inputRow, z);

				for (x=0; x<xSize; x++)
					this->setValue(x, y, z, (GLubyte)outputRow[x]);
			}
		}

		free(inputRow);
		free(outputRow);
		free(starttab);
		free(lengthtab);

	}


	fclose(inf);

	return true;
}


unsigned short CSgiImage::getshort(FILE *inf)
{
	unsigned char buf[2];
	size_t s;

	s = fread(buf,2,1,inf);
	return (buf[0]<<8)+(buf[1]<<0);
}

unsigned long CSgiImage::getlong(FILE *inf)
{
	unsigned char buf[4];
	size_t s;

	s = fread(buf,4,1,inf);
	return (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+(buf[3]<<0);
}

unsigned char CSgiImage::getbyte(FILE *inf)
{
	unsigned char buf[1];
	size_t s;

	s = fread(buf,1,1,inf);
	return buf[0]<<0;
}

void CSgiImage::setAlphaChannel(bool flag)
{
	m_alphaChannel = flag;
}
