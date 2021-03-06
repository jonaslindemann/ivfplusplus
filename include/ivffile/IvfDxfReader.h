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

#ifndef _CIvfDxfReader_h_
#define _CIvfDxfReader_h_

#include <ivffile/IvfFileReader.h>

#include <ivf/IvfIndex.h>

/**
 * Base class for a DXF reader
 *
 * This class implements a loader for DXF files. The class
 * reads 3DFACE elements in a DXF file. The class cannot
 * correct for face orientation.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API CIvfDxfReader : public CIvfFileReader {
private:
	std::string m_row;
	int m_faceCount;
	CIvfIndex* m_idx;
	bool m_doubleFaces;
	std::vector<CIvfIndex*> m_faceIndex;
	void read3DFace(std::istream &in);
	bool findKey(const char* heading, std::istream &in);
	bool readData(std::istream &in);
	bool checkHeader(std::istream &in);
public:
	/** CIvfDxfReader constructor */
	CIvfDxfReader();

	/** CIvfDxfReader destructor */
	virtual ~CIvfDxfReader();

	void correctFaceNormals();

	/** Flag controlling if faces should be made double */
	void setDoubleFaces(bool flag);

	/** Reads the DXF file */
	virtual void read();
};

#endif 
