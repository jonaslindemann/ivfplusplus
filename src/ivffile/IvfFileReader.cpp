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

#include <ivffile/IvfFileReader.h>

using namespace std;

// ------------------------------------------------------------
CIvfFileReader::CIvfFileReader()
{
	m_scaleFactor = 1.0;
	m_reverseFace = true;
}

// ------------------------------------------------------------
CIvfFileReader::~CIvfFileReader()
{

}

// ------------------------------------------------------------
void CIvfFileReader::read()
{

}

// ------------------------------------------------------------
void CIvfFileReader::resetSize()
{
	m_min[0] = 1e300;
	m_min[1] = 1e300;
	m_min[2] = 1e300;

	m_max[0] = -1e300;
	m_max[1] = -1e300;
	m_max[2] = -1e300;
}

// ------------------------------------------------------------
void CIvfFileReader::maxMinCalc(double x, double y, double z)
{
	if (x>m_max[0])
		m_max[0] = x;
	if (y>m_max[1])
		m_max[1] = y;
	if (z>m_max[2])
		m_max[2] = z;
	if (x<m_min[0])
		m_min[0] = x;
	if (y<m_min[1])
		m_min[1] = y;
	if (z<m_min[2])
		m_min[2] = z;
}

// ------------------------------------------------------------
void CIvfFileReader::getSize(double *min, double *max)
{
	min[0] = m_min[0];
	min[1] = m_min[1];
	min[2] = m_min[2];
	max[0] = m_max[0];
	max[1] = m_max[1];
	max[2] = m_max[2];
}

// ------------------------------------------------------------
void CIvfFileReader::getLine(string &row)
{
	m_inputFile.getline(m_rawRow, 256);
	row = m_rawRow;
}

// ------------------------------------------------------------
bool CIvfFileReader::find(const char *keyword, string &row)
{
	int pos = row.find(keyword);
	if (pos>=0)
	{
		if (pos>0)
		{
			if (row[pos-1]!=32)
				return false;
		}
		return true;
	}
	else
		return false;
}

// ------------------------------------------------------------
bool CIvfFileReader::findPos(const char *keyword, string &row, int &pos)
{
	pos = row.find(keyword);
	if (pos>=0)
	{
		if (pos>0)
		{
			if (row[pos-1]!=32)
				return false;
		}
		return true;
	}
	else
		return false;
}

// ------------------------------------------------------------
void CIvfFileReader::getLine(istream &in, string &row)
{
	in.getline(m_rawRow, 256);
	row = m_rawRow;
}

// ------------------------------------------------------------
void CIvfFileReader::setScaling(double scaleFactor)
{
	m_scaleFactor = scaleFactor;
}

// ------------------------------------------------------------
void CIvfFileReader::setReverseFace(bool flag)
{
	m_reverseFace = flag;
}

// ------------------------------------------------------------
bool CIvfFileReader::getReverseFace()
{
	return m_reverseFace;
}

// ------------------------------------------------------------
double CIvfFileReader::getScaling()
{
	return m_scaleFactor;
}

// ------------------------------------------------------------
void CIvfFileReader::getMidpoint(double &x, double &y, double &z)
{
	x = (m_max[0] - m_min[0])/2.0;
	y = (m_max[1] - m_min[1])/2.0;
	z = (m_max[2] - m_min[2])/2.0;
}

