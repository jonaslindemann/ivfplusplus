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

#include <ivffile/FileReader.h>

#include <ivf/Index.h>

namespace ivf {

IvfSmartPointer(DxfReader);

/**
 * Base class for a DXF reader
 *
 * This class implements a loader for DXF files. The class
 * reads 3DFACE elements in a DXF file. The class cannot
 * correct for face orientation.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API DxfReader : public FileReader {
private:
	std::string m_row;
	int m_faceCount;
	Index* m_idx;
	bool m_doubleFaces;
	std::vector<Index*> m_faceIndex;
	void read3DFace(std::istream &in);
	bool findKey(const char* heading, std::istream &in);
	bool readData(std::istream &in);
	bool checkHeader(std::istream &in);
public:
	/** DxfReader constructor */
	DxfReader();

	/** DxfReader destructor */
	virtual ~DxfReader();

	IvfClassInfo("DxfReader", FileReader);
	IvfStdFactory(DxfReader);

	void correctFaceNormals();

	/** Flag controlling if faces should be made double */
	void setDoubleFaces(bool flag);

	/** Reads the DXF file */
	virtual void read();
};

}