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

#include <ivffile/IvfPolyReader.h>

#ifdef WIN32
#define strcpy strcpy_s
#endif

#define KW_ERROR       -1
#define KW_COMMENT     0
#define KW_ELEMENT     1
#define KW_PROPERTY    2
#define KW_END_HEADER  3
#define KW_VERTEX      4
#define KW_FACE        5
#define KW_EDGE        6
#define KW_LIST        7

#include <strstream>

#include <ivf/IvfPolySet.h>

using namespace std;

// ------------------------------------------------------------
CPolyReader::CPolyReader()
{
}

// ------------------------------------------------------------
CPolyReader::~CPolyReader()
{

}


// ------------------------------------------------------------
int CPolyReader::getKeyword(string &row)
{
	if (find("comment", row))
		return KW_COMMENT;

	if (find("property", row))
		return KW_PROPERTY;

	if (find("element", row))
		return KW_ELEMENT;

	if (find("end_header", row))
		return KW_END_HEADER;

	return KW_ERROR;
}

// ------------------------------------------------------------
int CPolyReader::getDatatype(string &row, int &pos, int &size)
{

	if (findPos("int8", row, pos))
	{
		size = 4;
		return PFR_INT8;
	}

	if (findPos("uint8", row, pos))
	{
		size = 5;
		return PFR_UINT8;
	}

	if (findPos("int16", row, pos))
	{
		size = 5;
		return PFR_INT16;
	}

	if (findPos("uint16", row, pos))
	{
		size = 6;
		return PFR_UINT16;
	}

	if (findPos("int32", row, pos))
	{
		size = 5;
		return PFR_INT32;
	}

	if (findPos("int", row, pos))
	{
		size = 3;
		return PFR_INT32;
	}

	if (findPos("uint32", row, pos))
	{
		size = 6;
		return PFR_UINT32;
	}

	if (findPos("uint", row, pos))
	{
		size = 4;
		return PFR_UINT32;
	}

	if (findPos("uchar", row, pos))
	{
		size = 5;
		return PFR_UINT32;
	}

	if (findPos("float32", row, pos))
	{
		size = 7;
		return PFR_FLOAT32;
	}

	if (findPos("float64", row, pos))
	{
		size = 7;
		return PFR_FLOAT64;
	}

	if (findPos("float", row, pos))
	{
		size = 5;
		return PFR_FLOAT64;
	}

	return KW_ERROR;
}

// ------------------------------------------------------------
int CPolyReader::getElementType(string &row)
{
	if (find("face", row))
		return KW_FACE;

	if (find("vertex", row))
		return KW_VERTEX;

	if (find("edge", row))
		return KW_EDGE;

	return KW_ERROR;
}

// ------------------------------------------------------------
bool CPolyReader::checkHeader(istream &in)
{
	getLine(m_row);

	if (find("ply", m_row))
	{
		getLine(m_row);
		if (find("format", m_row))
		{
			if (find("format", m_row))
			{
				if (find("1.0", m_row))
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

// ------------------------------------------------------------
bool CPolyReader::parseElement(string &row)
{
	using namespace std;

	char buff[256];
	int pos = row.find("element");
	string options = row.substr(pos+8);
	string parameters = "";
	istrstream in(buff);

	switch (getElementType(row)) {
	case KW_FACE:
		m_currentElementType = ET_FACE;
		m_elementOrder.push_back(m_currentElementType);
		pos = options.find("face");
		parameters = options.substr(pos+5);
		strcpy(buff,parameters.c_str());
		if (!(in >> m_nFaces))
			return false;
		else
			return true;
		break;
	case KW_VERTEX:
		m_currentElementType = ET_VERTEX;
		m_elementOrder.push_back(m_currentElementType);
		pos = options.find("vertex");
		parameters = options.substr(pos+7);
		strcpy(buff,parameters.c_str());
		if (!(in >> m_nVertices))
			return false;
		else
			return true;
		break;
	case KW_EDGE:
		m_currentElementType = ET_EDGE;
		m_elementOrder.push_back(m_currentElementType);
		pos = options.find("edge");
		parameters = options.substr(pos+5);
		strcpy(buff,parameters.c_str());
		if (!(in >> m_nEdges))
			return false;
		else
			return true;
		break;
	default:
		return false;
	}
}

// ------------------------------------------------------------
bool CPolyReader::parseComment(string &row)
{
	using namespace std;

	int pos = row.find("comment");
	string comment = row.substr(pos+8);
	return true;
}

// ------------------------------------------------------------
bool CPolyReader::parseProperty(string &row)
{
	using namespace std;

	char buff[256];
	int pos = row.find("property");
	int size;
	string options = row.substr(pos+9);
	string parameters;
	istrstream in(buff);
	int datatype;
	int proptype;
	int datatype1;
	int datatype2;

	if (!find("list",options))
	{
		datatype = getDatatype(options,pos,size);
		parameters = options.substr(pos+size+1);

		proptype = PFR_OTHER;

		if (find("x",parameters))
			proptype = PFR_X;
		if (find("y",parameters))
			proptype = PFR_Y;
		if (find("z",parameters))
			proptype = PFR_Z;
		if (find("red",parameters))
			proptype = PFR_RED;
		if (find("green",parameters))
			proptype = PFR_GREEN;
		if (find("blue",parameters))
			proptype = PFR_BLUE;

		switch (m_currentElementType) {
		case ET_FACE:
			m_faceDataType.push_back(datatype);
			m_facePropType.push_back(proptype);
			break;
		case ET_VERTEX:
			m_vertexDataType.push_back(datatype);
			m_vertexPropType.push_back(proptype);
			break;
		default:
			return false;
		}
	}
	else
	{
		pos = options.find("list");
		parameters = options.substr(pos+5);
		datatype1 = getDatatype(parameters,pos,size);
		parameters.replace(pos,size,"       ");
		datatype2 = getDatatype(parameters,pos,size);
		parameters = parameters.substr(pos+size+1);
		return find("vertex_indices",parameters);
	}

	return true;
}

// ------------------------------------------------------------
bool CPolyReader::parseHeader(istream &in)
{
	bool done;
	bool error;

	done = false;
	error = false;

	do
	{
		getLine(m_row);

		switch (getKeyword(m_row)) {
		case KW_ERROR:
			error = true;
			break;
		case KW_COMMENT:
			error = !parseComment(m_row);
			break;
		case KW_ELEMENT:
			error = !parseElement(m_row);
			break;
		case KW_PROPERTY:
			error = !parseProperty(m_row);
			break;
		case KW_END_HEADER:
			done = true;
			break;
		default:
			cout << "This should not happen." << endl;
			error = true;
			break;
		}
	}
	while ((!error)&&(!done));

	return !error;
}

bool CPolyReader::readVertices(istream &in)
{
	int i, j;
	long ivalue;
	double value;

	double x, y, z;

	CPolySet* polySet = (CPolySet*) this->getShape();

	cout << "PolyFileReader: Reading vertices." << endl;

	for (i=0; i<m_nVertices; i++)
	{
		x = y = z = 0.0;

		for (j=0; j<(int)m_vertexDataType.size(); j++)
		{
			if (m_vertexDataType[j]<PFR_UINT32)
			{
				if (!(in >> ivalue))
					return false;
				value = (double)ivalue;
			}
			else
			{
				if (!(in >> value))
					return false;
			}

			switch (m_vertexPropType[j]) {
			case PFR_X:
				x = value * getScaling();
				break;
			case PFR_Y:
				y = value * getScaling();
				break;
			case PFR_Z:
				z = value * getScaling();
				break;
			default:

				break;
			}
		}
		maxMinCalc(x, y, z);
		polySet->addCoord(x, y, z);
	}
	cout << "PolyFileReader: " << m_nVertices << " vertices read." << endl;
	return true;
}

bool CPolyReader::readFaces(istream &in)
{
	int i, j;
	int nIndices;
	int oldIndices;
	long ivalue;

	vector<long> indices;

	CPolySet* polySet = (CPolySet*) this->getShape();
	CIndex* idx = NULL;

	oldIndices = -1;

	cout << "PolyFileReader: Reading indices." << endl;

	for (i=0; i<m_nFaces; i++)
	{
		if (!(in >> nIndices))
			return false;

		if (nIndices!=oldIndices)
		{
			if (idx!=NULL)
				polySet->addCoordIndex(idx);

			idx = new CIndex();

			if (nIndices==3)
				idx->setTopology(IVF_IDX_TRIANGLES);
			else
				idx->setTopology(IVF_IDX_QUADS);
		}

		if (getReverseFace())
		{
			indices.clear();
			for (j=0; j<nIndices; j++)
			{
				if (!(in >> ivalue))
					return false;

				indices.push_back(ivalue);
			}

			for (j = nIndices-1; j>=0; j--)
				idx->add(indices[j]);
		}
		else
		{
			for (j=0; j<nIndices; j++)
			{
				if (!(in >> ivalue))
					return false;

				idx->add(ivalue);
			}
		}

		oldIndices = nIndices;
	}
	polySet->addCoordIndex(idx);
	cout << "PolyFileReader: " << m_nFaces << " indices read." << endl;
	return true;
}

bool CPolyReader::readEdges(istream &in)
{
	return true;
}

// ------------------------------------------------------------
bool CPolyReader::readData(istream &in)
{
	unsigned int i;
	int elementType;

	CShape* polySet = new CPolySet();
	this->setShape(polySet);

	for (i=0; i<m_elementOrder.size(); i++)
	{
		elementType = m_elementOrder[i];

		switch (elementType) {
		case ET_VERTEX:
			if (!readVertices(in))
				return false;
			break;
		case ET_FACE:
			if (!readFaces(in))
				return false;
			break;
		case ET_EDGE:
			if (!readEdges(in))
				return false;
			break;
		default:
			return false;
		}
	}
	return true;
}

// ------------------------------------------------------------
void CPolyReader::read()
{
	// Initiate max min calc

	resetSize();

	// Initialize variables

	m_currentElementType = ET_NONE;
	m_faceDataType.clear();
	m_facePropType.clear();
	m_vertexDataType.clear();
	m_vertexPropType.clear();
	m_elementOrder.clear();

	// Open file

	m_inputFile.open(getFileName().c_str(),ios::in);

	// Check file

	if (!m_inputFile)
	{
		cout << "PolyFileReader: File invalid." << endl;
		return;
	}
	else
		cout << "PolyFileReader: File ok." << endl;

	// Read and check header

	if (!checkHeader(m_inputFile))
	{
		cout << "PolyFileReader: Invalid  header." << endl;
		m_inputFile.close();
		return;
	}
	else
		cout << "PolyFileReader: Header ok." << endl;

	// Parse additional header

	if (!parseHeader(m_inputFile))
	{
		cout << "PolyFileReader: Invalid file information." << endl;
		return;
	}
	else
		cout << "PolyFileReader: File information ok." << endl;

	// Read data

	if (!readData(m_inputFile))
	{
		cout << "PolyFileReader: Invalid data." << endl;
		return;
	}
	else
		cout << "PolyFileReader: data ok." << endl;


	m_inputFile.close();
}



