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

#include <ivffile/IvfDxfReader.h>

#include <ivf/IvfPolySet.h>

CIvfDxfReader::CIvfDxfReader()
{
	m_doubleFaces = false;
}

CIvfDxfReader::~CIvfDxfReader()
{

}

void CIvfDxfReader::read()
{
	// Initiate max min calc

	resetSize();

	// Initialize variables

	m_faceCount = 0;

	// Open file

	m_inputFile.open(getFileName(),std::ios::in);

	// Check file
	
	if (!m_inputFile)
	{
		std::cout << "DxfFileReader: File invalid." << std::endl;
		return;
	}
	else
		std::cout << "DxfFileReader: File ok." << std::endl;

	// Read and check header

	if (!checkHeader(m_inputFile))
	{
		std::cout << "DxfFileReader: Invalid  header." << std::endl;
		m_inputFile.close();
		return;
	}
	else
		std::cout << "DxfFileReader: Header ok." << std::endl;

	// Parse additional header

	// Read data

	if (!readData(m_inputFile))
	{
		std::cout << "DxfFileReader: Invalid data." << std::endl;
		return;
	}
	else
		std::cout << "DxfFileReader: data ok." << std::endl;


	m_inputFile.close();
}

bool CIvfDxfReader::checkHeader(std::istream &in)
{
	int pos;

	getLine(m_row);
	getLine(m_row);
	pos = m_row.find("SECTION");
	if (pos>=0)
		return true;
	else
		return false;
	
}

bool CIvfDxfReader::readData(std::istream &in)
{
	CIvfPolySet* polySet = new CIvfPolySet();
	this->setShape(polySet);

	m_idx = new CIvfIndex();
	m_idx->setTopology(IVF_IDX_QUADS);

	if (!findKey("ENTITIES", in))
		return false;

	while (!in.eof())
		read3DFace(in);
	
	std::cout << "DxfFileReader: Correcting normals." << std::endl;

	correctFaceNormals();
	
	polySet->addCoordIndex(m_idx);
	std::cout << "DxfFileReader: " << m_faceCount/3 << " faces read." << std::endl;
	
	return true;
}

bool CIvfDxfReader::findKey(const char *heading, std::istream &in)
{
	int pos;

	while (!in.eof())
	{
		getLine(in, m_row);
		pos = m_row.find(heading);
		if (pos>=0)
			return true;
	}
	return false;
}

void CIvfDxfReader::read3DFace(std::istream &in)
{
	double x, y, z;
	double scaleFactor = getScaling();

	std::vector<long> indices;

	CIvfPolySet* polySet = (CIvfPolySet*) this->getShape();


	if (findKey("3DFACE", in))
	{
		findKey("10", in); in >> x;
		findKey("20", in); in >> y;
		findKey("30", in); in >> z;
		
		polySet->addCoord(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		maxMinCalc(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		m_idx->add(m_faceCount++);
		
		findKey("11", in); in >> x;
		findKey("21", in); in >> y;
		findKey("31", in); in >> z;
		
		polySet->addCoord(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		maxMinCalc(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		m_idx->add(m_faceCount++);
		
		findKey("12", in); in >> x;
		findKey("22", in); in >> y;
		findKey("32", in); in >> z;
		
		polySet->addCoord(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		maxMinCalc(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		m_idx->add(m_faceCount++);
		
		findKey("13", in); in >> x;
		findKey("23", in); in >> y;
		findKey("33", in); in >> z;
		
		polySet->addCoord(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		maxMinCalc(x * scaleFactor, y * scaleFactor, z * scaleFactor);
		m_idx->add(m_faceCount++);
	}
}

void CIvfDxfReader::setDoubleFaces(bool flag)
{
	m_doubleFaces = flag;
}

void CIvfDxfReader::correctFaceNormals()
{
}
