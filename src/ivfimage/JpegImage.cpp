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

#include <ivfimage/JpegImage.h>

using namespace ivf;

extern "C" {
#ifdef WIN32
#include <jpeglib.h>
#else
#include <jpeglib.h>
#endif
}

JpegImage::JpegImage()
{

}

JpegImage::~JpegImage()
{

}

bool JpegImage::read()
{
	struct jpeg_decompress_struct cinfo;
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride;
	struct jpeg_error_mgr pub;
#ifdef WIN32
	if (fopen_s(&infile, getFileName().c_str(), "rb") != 0) {
		fprintf(stderr, "can't open %s\n", getFileName().c_str());
		return 0;
	}
#else
	infile = fopen(getFileName().c_str(), "rb");
	if ( infile == nullptr) {
		fprintf(stderr, "can't open %s\n", getFileName().c_str());
		return 0;
	}
#endif 	

    cinfo.err = jpeg_std_error(&pub);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);

	try {
		jpeg_read_header(&cinfo, TRUE);
	}
	catch (...) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return false;
	}

	jpeg_start_decompress(&cinfo);

	this->setChannels(cinfo.output_components);
	this->setSize(cinfo.output_width, cinfo.output_height);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		int i, j;

		for (i=0; i<(int)cinfo.output_width; i++)
			for (j=0; j<(int)cinfo.output_components; j++)
				this->setValue(i, cinfo.output_height - cinfo.output_scanline, j, (GLubyte)buffer[0][i*cinfo.output_components+j]);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return 1;
}
