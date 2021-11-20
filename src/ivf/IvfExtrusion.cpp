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

#include <ivf/IvfExtrusion.h>

#include <ivf/ivfconfig.h>

// ------------------------------------------------------------
void ivfSetGLEJoinStyle(int style)
{
	gleSetJoinStyle(style);
}

// ------------------------------------------------------------
int ivfGetGLEJoinStyle()
{
	return gleGetJoinStyle();
}

// ------------------------------------------------------------
CExtrusion::CExtrusion ()
:CShape()
{
	m_sectionCoords = NULL;
	m_sectionNormals = NULL;
	m_selectSectionCoords = NULL;
	m_selectSectionNormals = NULL;
	m_spineColors = NULL;
	m_spineCoords = NULL;
	m_twist = NULL;
	m_upVector[0] = 0.0;
	m_upVector[1] = 1.0;
	m_upVector[2] = 0.0;
	m_useColors = false;
	m_selectScale = 1.05;
	m_useTwist = false;
	m_spineStart = 0;
	m_spineEnd = 0;
	m_spineSize = 0;
    m_textureMode = 0;

	//m_coords = new GLdouble[m_nCoords][3];
}

// ------------------------------------------------------------
CExtrusion::~CExtrusion ()
{

	// Clean up

	if (m_sectionCoords!=NULL)
		delete [] m_sectionCoords;
	if (m_sectionNormals!=NULL)
		delete [] m_sectionNormals;
	if (m_spineCoords!=NULL)
		delete [] m_spineCoords;
	if (m_spineColors!=NULL)
		delete [] m_spineColors;
	if (m_selectSectionCoords!=NULL)
		delete [] m_selectSectionCoords;
	if (m_selectSectionNormals!=NULL)
		delete [] m_selectSectionNormals;
	if (m_twist!=NULL)
		delete [] m_twist;
}

// ------------------------------------------------------------
void CExtrusion::doCreateGeometry()
{
	if ( (m_spineStart>=0)&&(m_spineEnd>0) )
		m_spineDelta = m_spineEnd-m_spineStart + 1;
	else
		m_spineDelta = m_spineSize;

	if (m_sectionCoords==NULL)
		return;

	if (m_sectionNormals==NULL)
		return;

	if (m_spineCoords==NULL)
		return;

	if (m_useColors)
		if (m_spineColors==NULL)
			return;

	if (m_useColors)
	{
		if (!m_useTwist)
			gleExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineDelta,
			&m_spineCoords[m_spineStart],
			&m_spineColors[m_spineStart]);
		else
			gleTwistExtrusion(m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineDelta,
			&m_spineCoords[m_spineStart],
			&m_spineColors[m_spineStart],
			m_twist);
	}
	else
	{
		if (!m_useTwist)
			gleExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineDelta,
			&m_spineCoords[m_spineStart],
			NULL);
		else
			gleTwistExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineDelta,
			&m_spineCoords[m_spineStart],
			NULL,
			m_twist);
	}
}

// ------------------------------------------------------------
void CExtrusion::setSpineSize(int size)
{
	// Delete previous spine coords/colors if any

	if (m_spineCoords!=NULL)
		delete [] m_spineCoords;
	if (m_spineColors!=NULL)
		delete [] m_spineColors;
	if (m_useTwist)
		if (m_twist!=NULL)
			delete [] m_twist;

	// Set new size

	m_spineSize = size;

	// Allocate memory for new spine

	m_spineCoords = new gleDouble[m_spineSize][3];
	m_spineColors = new float[m_spineSize][3];
	if (m_useTwist)
		m_twist = new gleDouble[m_sectionSize];

	// initialize spine color array

	CMaterial* material;
	float color[3], alfa;

	material = this->getMaterial();
	if (material!=NULL)
		material->getDiffuseColor(color[0], color[1], color[2], alfa);
	else
	{
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
	}

	int i;
	for (i = 0; i<m_spineSize; i++)
	{
		m_spineColors[i][0] = color[0];
		m_spineColors[i][1] = color[1];
		m_spineColors[i][2] = color[2];
		if (m_useTwist)
			m_twist[i] = 0.0;
	}
}

// ------------------------------------------------------------
void CExtrusion::setSectionSize(int size)
{
	// Delete previous section/normal arrays

	if (m_sectionCoords!=NULL)
		delete [] m_sectionCoords;
	if (m_sectionNormals!=NULL)
		delete [] m_sectionNormals;
	if (m_selectSectionCoords!=NULL)
		delete [] m_selectSectionCoords;
	if (m_selectSectionNormals!=NULL)
		delete [] m_selectSectionNormals;

	// Set section size

	m_sectionSize = size;

	// Allocate memory for new section

	m_sectionCoords = new gleDouble[m_sectionSize][2];
	m_sectionNormals = new gleDouble[m_sectionSize][2];
	m_selectSectionCoords = new gleDouble[m_sectionSize][2];
	m_selectSectionNormals = new gleDouble[m_sectionSize][2];
}

int CExtrusion::getSpineSize()
{
	return m_spineSize;
}

int CExtrusion::getSectionSize()
{
	return m_sectionSize;
}

// ------------------------------------------------------------
void CExtrusion::setSectionCoord(int pos, double x, double y)
{
	if (m_sectionCoords!=NULL)
	{
		if ((pos>=0)&&(pos<m_sectionSize))
		{
			m_sectionCoords[pos][0] = x;
			m_sectionCoords[pos][1] = y;
			m_selectSectionCoords[pos][0] = x * m_selectScale;
			m_selectSectionCoords[pos][1] = y * m_selectScale;
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::setSectionNormal(int pos, double x, double y)
{
	if (m_sectionNormals!=NULL)
	{
		if ((pos>=0)&&(pos<m_sectionSize))
		{
			m_sectionNormals[pos][0] = x;
			m_sectionNormals[pos][1] = y;
			m_selectSectionNormals[pos][0] = x;
			m_selectSectionNormals[pos][1] = y;
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::setSpineCoord(int pos, double x, double y, double z)
{
	if (m_spineCoords!=NULL)
	{
		if ((pos>=0)&&(pos<m_spineSize))
		{
			m_spineCoords[pos][0] = x;
			m_spineCoords[pos][1] = y;
			m_spineCoords[pos][2] = z;
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::getSectionCoord(int pos, double &x, double &y)
{
	if (m_sectionCoords!=NULL)
	{
		if ((pos>=0)&&(pos<m_sectionSize))
		{
			x = m_sectionCoords[pos][0];
			y = m_sectionCoords[pos][1];
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::getSectionNormal(int pos, double &x, double &y)
{
	if (m_sectionNormals!=NULL)
	{
		if ((pos>=0)&&(pos<m_sectionSize))
		{
			x = m_sectionNormals[pos][0];
			y = m_sectionNormals[pos][1];
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::getSpineCoord(int pos, double &x, double &y, double &z)
{
	if (m_spineCoords!=NULL)
	{
		if ((pos>=0)&&(pos<m_spineSize))
		{
			x = m_spineCoords[pos][0];
			y = m_spineCoords[pos][1];
			z = m_spineCoords[pos][2];
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::setSpineColor(int pos, float red, float green, float blue)
{
	if (m_spineColors!=NULL)
	{
		if ((pos>=0)&&(pos<m_spineSize))
		{
			m_spineColors[pos][0] = red;
			m_spineColors[pos][1] = green;
			m_spineColors[pos][2] = blue;
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::getSpineColor(int pos, float &red, float &green, float &blue)
{
	if (m_spineColors!=NULL)
	{
		if ((pos>=0)&&(pos<m_spineSize))
		{
			red = m_spineColors[pos][0];
			green = m_spineColors[pos][1];
			blue = m_spineColors[pos][2];
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::setUseColor(bool flag)
{
	m_useColors = flag;
}

// ------------------------------------------------------------
bool CExtrusion::getUseColor()
{
	return m_useColors;
}


// ------------------------------------------------------------
void CExtrusion::setUpVector(double x, double y, double z)
{
	m_upVector[0] = x;
	m_upVector[1] = y;
	m_upVector[2] = z;
}

// ------------------------------------------------------------
void CExtrusion::doCreateSelect()
{
    gleTextureMode(m_textureMode);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	if (m_useColors)
	{
		if (!m_useTwist)
			gleExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineSize,
			m_spineCoords,
			m_spineColors);
		else
			gleTwistExtrusion(m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineSize,
			m_spineCoords,
			m_spineColors,
			m_twist);
	}
	else
	{
		if (!m_useTwist)
			gleExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineSize,
			m_spineCoords,
			NULL);
		else
			gleTwistExtrusion (m_sectionSize,
			m_sectionCoords,
			m_sectionNormals,
			m_upVector,
			m_spineSize,
			m_spineCoords,
			NULL,
			m_twist);
	}
	glPopAttrib();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    gleTextureMode(0);
}

// ------------------------------------------------------------
void CExtrusion::setSelectScale(double scale)
{
	m_selectScale = scale;
}

// ------------------------------------------------------------
void CExtrusion::setTextureMode(int mode)
{
    m_textureMode = mode;
	gleTextureMode(mode);
}

// ------------------------------------------------------------
void CExtrusion::setUseTwist(bool flag)
{
	m_useTwist = flag;
	if (m_spineCoords!=NULL)
	{
		if (m_useTwist)
		{
			if (m_twist!=NULL)
				delete [] m_twist;

			m_twist = new gleDouble[m_spineSize];
			for (int i=0; i<m_spineSize; i++)
				m_twist[i] = 0.0;
		}
		else
		{
			if (m_twist!=NULL)
				delete [] m_twist;

			m_twist=NULL;
		}
	}
}

// ------------------------------------------------------------
bool CExtrusion::getUseTwist()
{
	return m_useTwist;
}

// ------------------------------------------------------------
void CExtrusion::setSpineTwist(int pos, double twist)
{
	if (m_twist!=NULL)
	{
		if ((pos>=0)&&(pos<m_spineSize))
		{
			m_twist[pos] = twist;
		}
	}
}

// ------------------------------------------------------------
void CExtrusion::getSpineTwist(int pos, double &twist)
{
	if (m_twist!=NULL)
	{
		if ((pos>=0)&&(pos<m_sectionSize))
		{
			twist = m_twist[pos];
		}
	}
}

void CExtrusion::setStartEnd(int startIdx, int endIdx)
{
	m_spineStart = startIdx;
	m_spineEnd = endIdx;
}

void CExtrusion::setJoinStyle(int style)
{
	ivfSetGLEJoinStyle(style);
}

void CExtrusion::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=NULL)
	{
		int i;
		double maxSize[3];
		double x, y, z;
		double radius;

		maxSize[0] = maxSize[1] = maxSize[2] = -1e300;

		for (i=1; i<(m_spineSize-1); i++)
		{
			x = m_spineCoords[i][0];
			y = m_spineCoords[i][1];
			z = m_spineCoords[i][2];

			if (fabs(x)>maxSize[0])
				maxSize[0] = x;
			if (fabs(y)>maxSize[1])
				maxSize[1] = y;
			if (fabs(z)>maxSize[2])
				maxSize[2] = z;
		}

		radius = sqrt(
			pow(maxSize[0],2) +
			pow(maxSize[1],2) +
			pow(maxSize[2],2));

		getBoundingSphere()->setRadius(radius);
	}
}
