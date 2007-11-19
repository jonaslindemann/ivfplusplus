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

#ifndef _CIvfImage_h_
#define _CIvfImage_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfGL.h> // OpenGL stuff

IvfSmartPointer(CIvfImage);

/**
 * Image class
 *
 * The CIvfImage class implements a OpenGL compatible 2D image.
 * The class is used as a base class for the image loaders in 
 * the ivfimage library.
 */
class IVF_API CIvfImage : public CIvfBase {
private:
	int m_size[2];
	int m_channels;
	double m_ratio;
	GLubyte* m_imageMap;
	GLint m_internalFormat;
public:
	/** CIvfImage constructor */
	CIvfImage();

	/** CIvfImage destructor */
	virtual ~CIvfImage();

	IvfClassInfo("CIvfImage",CIvfBase);

	/** 
	 * Clear image
	 *
	 * Fills entire image with black(=0) and if alpha channel
	 * present it is filled with 255
	 */
	void clear();

	/** 
	 * Validate image coordinate
	 *
	 * @return true if @param x and @param y are valid
	 * image coordinates
	 */
	bool valid(int x, int y);

	/** Set an image pixel */
	void setPixel(int x, int y, GLubyte red, GLubyte green, GLubyte blue);

	/** Set an image pixel by specifying channel and value */
	void setValue(int x, int y, int channel, GLubyte value);

	/** Set alpha value of image pixel */
	void setPixelAlpha(int x, int y, GLubyte alpha);

	/** Get pixel values */
	void getPixel(int x, int y, GLubyte &red, GLubyte &green, GLubyte &blue);

	/** Return pixel alpha values */
	GLubyte getPixelAlpha(int x, int y);

	/** Fill image with specified color */
	void fillColor(GLubyte red, GLubyte green, GLubyte blue);

	/** Fill image alpha channel with value */
	void fillColorAlpha(GLubyte alpha);

	/** Fill a rectangular area of image with color */
	void fillRect(int x1, int y1, int x2, int y2, GLubyte red, GLubyte green, GLubyte blue);

	/** Fill a rectangular area of image width specified alpha value */
	void fillRectAlpha(int x1, int y1, int x2, int y2, GLubyte alpha);

	/** Draw a frame with specified color */
	void drawFrame(int x1, int y1, int x2, int y2, GLubyte red, GLubyte green, GLubyte blue);

	/** Draw a frame width specified alpha value */
	void drawFrameAlpha(int x1, int y1, int x2, int y2, GLubyte alpha);

	/** 
	 * Replace a color in the image with another 
	 *
	 * @param srcRed - red color component of color to replace.
	 * @param srcGreen - green color component of color to replace.
	 * @param srgBlue - blue color component of color to replace.
	 * @param tgtRed - red color component of replacing color.
	 * @param tgtGreen - green color component of replacing color.
	 * @param tgtBlue - blue color component of replacing color.
	 */
	void replaceColor(GLubyte srcRed, GLubyte srcGreen, GLubyte srcBlue, GLubyte tgtRed, GLubyte tgtGreen, GLubyte tgtBlue);

	/** 
	 * Creates an alpha mask using a color component in the image 
	 *
	 * @param treshold - color component treshold value.
	 * @param component - color component to compare with.
	 * @param over - alpha value if component>treshold.
	 * @param under - alpha value if component<treshold.
	 */
	void createAlphaMask(int treshold, int component, int over, int under);

	/** Assigns alpha channel values from mean color values. */ 
	void createAlphaFromColor();

	/** 
	 * Set number of channels in image 
	 *
	 * If image is allocated calling setChannels() will
	 * deallocate it. setChannels() should be called before
	 * setSize(), to avoid unnesseccary allocation.
	 */
	void setChannels(int channels);

	/** Returns the number of channels in the image */
	int getChannels();

	/** 
	 * Sets the internal format of the image 
	 * 
	 * This method should not be neccessary to call because
	 * the internal format is automatically set.
	 */
	void setInternalFormat(GLint format);

	/** Returns the internal image format for use with OpenGL */
	GLint getInternalFormat();

	/** 
	 * Sets the image size 
	 * 
	 * If channels are not set a 3 channel image
	 * will be created.
	 */
	void setSize(int width, int height);

	/** Return height of image */
	int getHeight();

	/** Return width of image */
	int getWidth();

	/** Return image ratio */
	double getRatio();

	void copyFromImage(CIvfImage* image, int srcStartX, int srcEndX, int srcStartY, int srcEndY, int destX, int destY);

	/** Sets the image map used by the image class */
	void setImageMap(void* image);

	/** 
	 * Return pointer to image map 
	 *
	 * This pointer can be used with OpenGL functions
	 */
	GLubyte* getImageMap();

	/** Returns the image format. */
	GLint getFormat();
};



#endif 

