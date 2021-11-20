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

#ifndef _CIvfPolyReader_h_
#define _CIvfPolyReader_h_

#include <ivffile/IvfFileReader.h>

//namespace std {};
//using namespace std;

#define ET_NONE   -1 
#define ET_VERTEX  0
#define ET_FACE    1
#define ET_EDGE    2

#define PFR_UINT8   0
#define PFR_INT8    1
#define PFR_UINT16  2
#define PFR_INT16   3
#define PFR_INT32   4
#define PFR_UINT32  5
#define PFR_FLOAT32 6
#define PFR_FLOAT64 7

#define PFR_X       0
#define PFR_Y       1
#define PFR_Z       2
#define PFR_RED     3
#define PFR_GREEN   4
#define PFR_BLUE    5
#define PFR_OTHER   6

IvfSmartPointer(CPolyReader);

/**
 * Base class for a POLY reader
 *
 * This class implements a loader for POLY files. The class
 * can handle surfaces with 3 or 4 vertices.
 * It should load most POLY files, but no guarantees are made.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API CPolyReader : public CFileReader {
private:
	std::string m_row;
	int m_nFaces;
	int m_nVertices;
	int m_nEdges;
	int m_currentElementType;
	std::vector<int> m_vertexDataType;
	std::vector<int> m_vertexPropType;
	std::vector<int> m_faceDataType;
	std::vector<int> m_facePropType;
	std::vector<int> m_elementOrder;

	int getDatatype(std::string &row, int &pos, int &size);
	int getElementType(std::string &row);
	int getKeyword(std::string &row);
	bool checkHeader(std::istream &in);
	bool parseProperty(std::string &row);
	bool parseComment(std::string &row);
	bool parseElement(std::string &row);
	bool parseHeader(std::istream &in);
	bool readEdges(std::istream &in);
	bool readFaces(std::istream &in);
	bool readVertices(std::istream &in);
	bool readData(std::istream &in);
public:
	/** CPolyReader constructor */
	CPolyReader();

	/** CPolyReader destructor */
	virtual ~CPolyReader();

	IvfClassInfo("CPolyReader",CFileReader);

	/** Reads POLY file */
	virtual void read();

};

#endif 
