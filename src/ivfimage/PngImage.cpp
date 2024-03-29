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

#include <ivfimage/PngImage.h>

using namespace ivf;

#ifdef IVF_HAVE_PNG
#ifdef WIN32
#include <png.h>
#else
#include <png.h>
#endif
#endif

PngImage::PngImage()
{

}

PngImage::~PngImage()
{

}

bool PngImage::read()
{
#ifdef IVF_HAVE_PNG

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	int bit_depth, color_type;
	FILE *fp;
#ifdef WIN32
	if (fopen_s(&fp, getFileName().c_str(), "rb") !=0 )
		return false;
#else
	fp = fopen(getFileName().c_str(), "rb");
	if (fp==nullptr)
		return false;
#endif

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		nullptr, nullptr, nullptr);

	if (png_ptr == nullptr)
	{
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
		return false;
	}

	/* Set up the input control if you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* The easiest way to read the image: */
	png_bytep* row_pointers;

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
	row_pointers = png_get_rows(png_ptr, info_ptr);

    width            = png_get_image_width(png_ptr, info_ptr);
    height           = png_get_image_height(png_ptr, info_ptr);
    bit_depth        = png_get_bit_depth(png_ptr, info_ptr);
    color_type       = png_get_color_type(png_ptr, info_ptr);

	bool supported = true;

	switch (color_type) {
	case PNG_COLOR_TYPE_GRAY:
		setChannels(1);
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		setChannels(2);
		break;
	case PNG_COLOR_TYPE_PALETTE:
		supported = false;
		break;
	case PNG_COLOR_TYPE_RGB:
		setChannels(3);
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		setChannels(4);
		break;
	case PNG_COLOR_MASK_PALETTE:
		supported = false;
		break;
	default:
		supported = false;
		break;
	}

	if (!supported)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);
		return false;
	}

	setSize(width, height);

	unsigned int i, j, k;

	for (i=0; i<height; i++)
		for (j=0; j<width; j++)
			for (k=0; k<getChannels(); k++)
				setValue(j, height-i-1, k, row_pointers[i][j*getChannels()+k]);


	/* At this point you have read the entire image */

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);

	/* close the file */
	fclose(fp);

	/* that's it */
	return true;
#else
	return false;
#endif
}
