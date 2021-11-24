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

#include <ivf/config.h>
#include <ivf/FaceSet.h>

#include <ivfmath/Vec3d.h>

namespace std {};
using namespace std;
using namespace ivf;

FaceSet::FaceSet()
:Shape()
{
	m_nCoords = 0;
	m_nFaces = 0;
	m_nTextureCoords = 0;

	m_coords = nullptr;
	m_coordIndex = nullptr;
	m_textureCoordIndex = nullptr;
	m_textureCoords = nullptr;
	m_colors = nullptr;
	m_normals = nullptr;
	m_useColor = FALSE;
	m_useTexture = FALSE;

}

FaceSet::~FaceSet()
{
	if (m_coords!=nullptr)
		delete [] m_coords;
	if (m_coordIndex!=nullptr)
		delete [] m_coordIndex;
	if (m_colors!=nullptr)
		delete [] m_colors;
	if (m_normals!=nullptr)
		delete [] m_normals;
	if (m_textureCoords!=nullptr)
		delete [] m_textureCoords;
	if (m_textureCoordIndex!=nullptr)
		delete [] m_textureCoordIndex;
}

// ------------------------------------------------------------
void FaceSet::doCreateGeometry()
{
	int i,j;

	if (m_useColor==true)
	{
		glPushAttrib(GL_COLOR_MATERIAL);
		glEnable(GL_COLOR_MATERIAL);
	}

	for ( i = 0; i<m_nFaces; i++)
	{
		glBegin(GL_QUADS);
		for (j = 0; j<4; j++)
		{
			if (m_coordIndex[i][j]!=-1)
			{
				if (m_useTexture==TRUE)
					glTexCoord2d(
					   m_textureCoords[m_textureCoordIndex[i][j]][0],
					   m_textureCoords[m_textureCoordIndex[i][j]][1]);
				glNormal3dv(m_normals[i]);
				if (m_useColor == TRUE)
					glColor3fv(m_colors[ m_coordIndex[i][j] ]);
				glVertex3dv(m_coords[ m_coordIndex[i][j] ]);
			}
		}
		glEnd();
	}

	if (m_useColor==true)
		glPopAttrib();
}

// ------------------------------------------------------------
void FaceSet::setCoordSize(long coordSize)
{
	m_nCoords = coordSize;

	if (m_coords!=nullptr)
		delete [] m_coords;
	if (m_colors!=nullptr)
		delete [] m_colors;

	m_coords = new double[m_nCoords][3];
	m_colors = new float[m_nCoords][3];
}

// ------------------------------------------------------------
void FaceSet::setCoord(long n, double x, double y, double z)
{
	if ( (n>=0)&&(n<m_nCoords) )
	{
		m_coords[n][0] = x;
		m_coords[n][1] = y;
		m_coords[n][2] = z;
	}
}

// ------------------------------------------------------------
void FaceSet::getCoord(long n, double &x, double &y, double &z)
{
	if ( (n>=0)&&(n<m_nCoords) )
	{
		x = m_coords[n][0];
		y = m_coords[n][1];
		z = m_coords[n][2];
	}
}

// ------------------------------------------------------------
void FaceSet::setCoordIndexSize(long indexSize)
{
	if (indexSize>0)
	{
		m_nFaces = indexSize;

		if (m_coordIndex!=nullptr)
			delete [] m_coordIndex;
		if (m_normals!=nullptr)
			delete [] m_normals;
		if (m_textureCoordIndex!=nullptr)
			delete [] m_textureCoordIndex;

		m_coordIndex = new long[m_nFaces][4];
		m_normals = new double[m_nFaces][3];
		m_textureCoordIndex = new long[m_nFaces][4];
	}
}

// ------------------------------------------------------------
void FaceSet::setTextureCoordSize(long size)
{
	if (size>0)
	{
		m_nTextureCoords = size;

		if (m_textureCoords!=nullptr)
			delete [] m_textureCoords;

		m_textureCoords = new double[m_nTextureCoords][2];
	}
}

// ------------------------------------------------------------
long FaceSet::getTextureCoordSize()
{
	return m_nTextureCoords;
}

// ------------------------------------------------------------
void FaceSet::setCoordIndex(long n, long i1, long i2, long i3, long i4)
{
	if ( (n>=0)&&(n<m_nFaces) )
	{
		Vec3d v;
		Vec3d u;
		Vec3d normal;

		m_coordIndex[n][0] = i1;
		m_coordIndex[n][1] = i2;
		m_coordIndex[n][2] = i3;
		m_coordIndex[n][3] = i4;

		// Calculate normal

		u.setComponents(
			m_coords[ m_coordIndex[n][1] ][0] - m_coords[ m_coordIndex[n][0] ][0],
			m_coords[ m_coordIndex[n][1] ][1] - m_coords[ m_coordIndex[n][0] ][1],
			m_coords[ m_coordIndex[n][1] ][2] - m_coords[ m_coordIndex[n][0] ][2]
		);

		v.setComponents(
			m_coords[ m_coordIndex[n][3] ][0] - m_coords[ m_coordIndex[n][0] ][0],
			m_coords[ m_coordIndex[n][3] ][1] - m_coords[ m_coordIndex[n][0] ][1],
			m_coords[ m_coordIndex[n][3] ][2] - m_coords[ m_coordIndex[n][0] ][2]
		);

		normal = u * v;

		normal.getComponents(m_normals[n]);
	}
}

// ------------------------------------------------------------
void FaceSet::getCoordIndex(long n, long &i1, long &i2, long &i3, long &i4)
{
	if ( (n>=0)&&(n<m_nFaces) )
	{
		i1 = m_coordIndex[n][0];
		i2 = m_coordIndex[n][1];
		i3 = m_coordIndex[n][2];
		i4 = m_coordIndex[n][3];
	}
}

// ------------------------------------------------------------
void FaceSet::setColor(long n, float r, float g, float b)
{
	if ( (n>=0)&&(n<m_nCoords) )
	{
		m_colors[n][0] = r;
		m_colors[n][1] = g;
		m_colors[n][2] = b;
	}
}

// ------------------------------------------------------------
void FaceSet::getColor(long n, float &r, float &g, float &b)
{
	if ( (n>=0)&&(n<m_nCoords) )
	{
		r = m_colors[n][0];
		g = m_colors[n][1];
		b = m_colors[n][2];
	}
}

// ------------------------------------------------------------
void FaceSet::setTextureCoord(long n, double s, double t)
{
	if ( (n>=0)&&(n<m_nTextureCoords) )
	{
		m_textureCoords[n][0] = s;
		m_textureCoords[n][1] = t;
	}
}

// ------------------------------------------------------------
void FaceSet::getTextureCoord(long n, double &s, double &t)
{
	if ( (n>=0)&&(n<m_nTextureCoords) )
	{
		s = m_textureCoords[n][0];
		t = m_textureCoords[n][1];
	}
}

// ------------------------------------------------------------
void FaceSet::setTextureCoordIndex(long n, long i1, long i2, long i3, long i4)
{
	if ( (n>=0)&&(n<m_nFaces) )
	{
		m_textureCoordIndex[n][0] = i1;
		m_textureCoordIndex[n][1] = i2;
		m_textureCoordIndex[n][2] = i3;
		m_textureCoordIndex[n][3] = i4;
	}
}

// ------------------------------------------------------------
void FaceSet::getTextureCoordIndex(long n, long &i1, long &i2, long &i3, long &i4)
{
	if ( (n>=0)&&(n<m_nFaces) )
	{
		i1 = m_textureCoordIndex[n][0];
		i2 = m_textureCoordIndex[n][1];
		i3 = m_textureCoordIndex[n][2];
		i4 = m_textureCoordIndex[n][3];
	}
}

// ------------------------------------------------------------
void FaceSet::setUseColor(bool flag)
{
	m_useColor = flag;
}

// ------------------------------------------------------------
bool FaceSet::getUseColor()
{
	return m_useColor;
}

// ------------------------------------------------------------
void FaceSet::setUseTexture(bool flag)
{
	m_useTexture = flag;
}

// ------------------------------------------------------------
bool FaceSet::getUseTexture()
{
	return m_useTexture;
}

