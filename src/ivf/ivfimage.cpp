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

#include <ivf/IvfImage.h>

CImage::CImage()
{
	m_imageMap = NULL;
	m_size[0] = -1;
	m_size[1] = -1;
	m_channels = 3;
	m_internalFormat = GL_RGB;
}

CImage::~CImage()
{
	if (m_imageMap!=NULL)
		delete [] m_imageMap;
}

void CImage::setChannels(unsigned int channels)
{
	m_channels = channels;

	switch (m_channels) {
	case 1:
		m_internalFormat = GL_LUMINANCE;
		break;
	case 2:
		m_internalFormat = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		m_internalFormat = GL_RGB;
		break;
	case 4:
		m_internalFormat = GL_RGBA;
		break;
	default:
		m_internalFormat = GL_RGB;
		break;
	}

	this->setSize(m_size[0], m_size[1]);
}

unsigned int CImage::getChannels()
{
	return m_channels;
}

void CImage::setSize(int width, int height)
{
	if ((width>0)&&(height>0))
	{
		m_size[0] = width;
		m_size[1] = height;
		m_ratio = width/height;

		if (m_imageMap!=NULL)
			delete [] m_imageMap;

		m_imageMap = new GLubyte[m_size[0]*m_size[1]*m_channels];
	}
}

int CImage::getWidth()
{
	return m_size[0];
}

int CImage::getHeight()
{
	return m_size[1];
}

void CImage::setImageMap(void* image)
{
	if (m_imageMap!=NULL)
		delete [] m_imageMap;

	m_imageMap =(unsigned char*) image;
}

GLubyte* CImage::getImageMap()
{
	return m_imageMap;
}

void CImage::setPixel(int x, int y, GLubyte red, GLubyte green, GLubyte blue)
{
	setValue(x, y, 0, red);
	setValue(x, y, 1, green);
	setValue(x, y, 2, blue);
}

void CImage::setPixelAlpha(int x, int y, GLubyte alpha)
{
	setValue(x, y, 3, alpha);
}

bool CImage::valid(int x, int y)
{
	return ((x>=0)&&(x<m_size[0])&&(y>=0)&&(y<m_size[1]));
}

void CImage::getPixel(int x, int y, GLubyte &red, GLubyte &green, GLubyte &blue)
{
	if (valid(x, y))
	{
		red = m_imageMap[x*m_channels + m_size[0]*y*m_channels + 0];
		green = m_imageMap[x*m_channels + m_size[0]*y*m_channels + 1];
		blue = m_imageMap[x*m_channels + m_size[0]*y*m_channels + 2];
	}
}

GLubyte CImage::getPixelAlpha(int x, int y)
{
	if (valid(x, y))
	{
		return m_imageMap[x*m_channels + m_size[0]*y*m_channels + 3]; 
	}
	else
		return 0;
}

void CImage::clear()
{
	fillColor(0,0,0);
	if (m_channels==4)
		fillColorAlpha(255);
}

void CImage::fillColor(GLubyte red, GLubyte green, GLubyte blue)
{
	int i, j;

	for (i=0; i<m_size[0]; i++)
		for (j=0; j<m_size[1]; j++)
			setPixel(i, j, red, green, blue);
}

void CImage::fillColorAlpha(GLubyte alpha)
{
	int i, j;

	for (i=0; i<m_size[0]; i++)
		for (j=0; j<m_size[1]; j++)
			setPixelAlpha(i, j, alpha);
}

void CImage::fillRect(int x1, int y1, int x2, int y2, GLubyte red, GLubyte green, GLubyte blue)
{
	if ( (valid(x1, y1))&&(valid(x2, y2)) )
	{
		int i, j;
		
		for (i=x1; i<=x2; i++)
			for (j=y1; j<=y2; j++)
				setPixel(i, j, red, green, blue);		
	}
}

void CImage::fillRectAlpha(int x1, int y1, int x2, int y2, GLubyte alpha)
{
	if ( (valid(x1, y1))&&(valid(x2, y2)) )
	{
		int i, j;
		
		for (i=x1; i<=x2; i++)
			for (j=y1; j<=y2; j++)
				setPixelAlpha(i, j, alpha);		
	}
}

double CImage::getRatio()
{
	return m_ratio;
}


void CImage::drawFrame(int x1, int y1, int x2, int y2, GLubyte red, GLubyte green, GLubyte blue)
{
	if ( (valid(x1, y1))&&(valid(x2, y2)) )
	{
		int i;
		
		for (i=x1; i<=x2; i++)
		{
			setPixel(i, y1, red, green, blue);		
			setPixel(i, y2, red, green, blue);
		}

		for (i=y1; i<=y2; i++)
		{
			setPixel(x1, i, red, green, blue);		
			setPixel(x2, i, red, green, blue);
		}
	}
}

void CImage::drawFrameAlpha(int x1, int y1, int x2, int y2, GLubyte alpha)
{
	if ( (valid(x1, y1))&&(valid(x2, y2)) )
	{
		int i;
		
		for (i=x1; i<=x2; i++)
		{
			setPixelAlpha(i, y1, alpha);		
			setPixelAlpha(i, y2, alpha);
		}

		for (i=y1; i<=y2; i++)
		{
			setPixelAlpha(x1, i, alpha);		
			setPixelAlpha(x2, i, alpha);
		}
	}
}



GLint CImage::getFormat()
{
	switch (m_channels) {
	case 1:
		return GL_LUMINANCE;
		break;
	case 2:
		return GL_LUMINANCE_ALPHA;
		break;
	case 3:
		return GL_RGB;
		break;
	case 4:
		return GL_RGBA;
		break;
	default:
		return GL_RGB;
		break;
	}
}

void CImage::setValue(int x, int y, int channel, GLubyte value)
{
	if (valid(x, y))
	{
		m_imageMap[x*m_channels + m_size[0]*y*m_channels + channel] = value;
	}
}

void CImage::createAlphaMask(int treshold, int component, int over, int under)
{
	if (m_channels>3)
	{
		int i, j;
		GLubyte red, green, blue;
		
		for (i=0; i<=m_size[0]; i++)
			for (j=0; j<=m_size[1]; j++)
			{
				getPixel(i, j, red, green, blue);
				
				switch (component) {
				case 0:
					if (red>=treshold)
						setPixelAlpha(i, j, over);
					else
						setPixelAlpha(i, j, under);
					break;
				case 1:
					if (green>=treshold)
						setPixelAlpha(i, j, over);
					else
						setPixelAlpha(i, j, under);
					break;
				case 2:
					if (blue>=treshold)
						setPixelAlpha(i, j, over);
					else
						setPixelAlpha(i, j, under);
					break;
				case -1:
					if ((red>=treshold)&&(green>=treshold)&&(blue>=treshold))
						setPixelAlpha(i, j, over);
					else
						setPixelAlpha(i, j, under);
					break;
				default:
					if (red>=treshold)
						setPixelAlpha(i, j, over);
					else
						setPixelAlpha(i, j, under);
					break;
				}
			}
	}
}

void CImage::replaceColor(GLubyte srcRed, GLubyte srcGreen, GLubyte srcBlue, GLubyte tgtRed, GLubyte tgtGreen, GLubyte tgtBlue)
{
	int i, j;
	GLubyte red, green, blue;
	
	for (i=0; i<=m_size[0]; i++)
		for (j=0; j<=m_size[1]; j++)
		{
			getPixel(i, j, red, green, blue);
			
			if ((red==srcRed)&&(green==srcGreen)&&(blue==srcBlue))
				setPixel(i, j, tgtRed, tgtGreen, tgtBlue);
		}
}

void CImage::copyFromImage(CImage* image, int srcStartX, int srcEndX, int srcStartY, int srcEndY, int destX, int destY)
{
	int i, j;
	GLubyte red, green, blue;

	for (j=srcStartY; j<=srcEndY; j++)
		for (i=srcStartX; i<=srcEndX; i++)
		{
			if (image->valid(i, j))
			{
				image->getPixel(i, j, red, green, blue);
				setPixel(destX+i-srcStartX, destY+j-srcStartY, red, green, blue);
			}
		}
}


GLint CImage::getInternalFormat()
{
	return m_internalFormat;
}

void CImage::setInternalFormat(GLint format)
{
	m_internalFormat = format;
}

void CImage::createAlphaFromColor()
{
	if (m_channels>3)
	{
		int i, j;
		GLubyte red, green, blue;
		
		for (i=0; i<=m_size[0]; i++)
			for (j=0; j<=m_size[1]; j++)
			{
				getPixel(i, j, red, green, blue);
				double alpha = ((double)red + (double)green + (double)blue)/3.0;
				setPixelAlpha(i, j, (GLubyte)alpha);
			}
	}
}
