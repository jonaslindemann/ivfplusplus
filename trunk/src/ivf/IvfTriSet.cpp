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

#include <ivf/IvfTriSet.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIvfTriSet::CIvfTriSet()
{
	m_useColor = false;
	m_drawInterval = 1;
}

CIvfTriSet::~CIvfTriSet()
{
	
}

void CIvfTriSet::createGeometry()
{
	CIvfIndex* coordIdx;
	CIvfIndex* colorIdx;
	CIvfIndex* normalIdx;
	CIvfIndex* textureIdx;
	long i, j;
	
	glPushAttrib(GL_COLOR_MATERIAL);
	
	if (m_useColor)
		glEnable(GL_COLOR_MATERIAL);
	
	
	for (i=0; i<(int)m_coordIndexSet.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		
		coordIdx = m_coordIndexSet[i];
		if (m_useColor)
		{
			if (i<(int)m_colorIndexSet.size())
				colorIdx = m_colorIndexSet[i];
		}
		normalIdx = m_normalIndexSet[i];
		
		if (i<(int)m_textureIndexSet.size())
			textureIdx = m_textureIndexSet[i];
		else
			textureIdx = NULL;

		j=0;
		while (j<coordIdx->getSize())		
		{
/*
		for (j=0; j<coordIdx->getSize(); j+=3)
		{
			if (  (((j+1)/3)%m_drawInterval)==0)
			{
*/
				if (getUseVertexNormals())
					glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j)]->getComponents());
				else
					glNormal3dv(m_normalSet[normalIdx->getIndex(j/3)]->getComponents());
				
				if (m_useColor)
					glColor3fv(m_colorSet[colorIdx->getIndex(j)]->getColor());
				
				if (textureIdx!=NULL)
					glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j)]->getComponents());
				
				glVertex3dv(m_coordSet[coordIdx->getIndex(j)]->getComponents());
				
				if (getUseVertexNormals())
					glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j+1)]->getComponents());
				else
					glNormal3dv(m_normalSet[normalIdx->getIndex(j/3)]->getComponents());
				
				if (m_useColor)
					glColor3fv(m_colorSet[colorIdx->getIndex(j+1)]->getColor());
				
				if (textureIdx!=NULL)
					glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j+1)]->getComponents());
				
				glVertex3dv(m_coordSet[coordIdx->getIndex(j+1)]->getComponents());
				
				if (getUseVertexNormals())
					glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j+2)]->getComponents());
				else
					glNormal3dv(m_normalSet[normalIdx->getIndex(j/3)]->getComponents());
				
				if (m_useColor)
					glColor3fv(m_colorSet[colorIdx->getIndex(j+2)]->getColor());
				
				if (textureIdx!=NULL)
					glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j+2)]->getComponents());
				
				glVertex3dv(m_coordSet[coordIdx->getIndex(j+2)]->getComponents());
/*
			}
*/
			j+=m_drawInterval*3;
		}
		glEnd();
	}
	
	glPopAttrib();
}

void CIvfTriSet::setUseColor(bool flag)
{
	m_useColor = flag;
}

bool CIvfTriSet::getUseColor()
{
	return m_useColor;
}

void CIvfTriSet::calcNormal(CIvfIndex *idx)
{
	long i;
	CIvfVec3d* p1;
	CIvfVec3d* p2;
	CIvfVec3d* p3;
	CIvfVec3d u;
	CIvfVec3d v;
	CIvfIndex* normalIdx;
	//double n[3];
	
	normalIdx = new CIvfIndex();
	
	for (i=0; i<idx->getSize(); i+=3)
	{
		CIvfVec3d* normal = new CIvfVec3d();
		
		p1 = m_coordSet[idx->getIndex(i)];
		p2 = m_coordSet[idx->getIndex(i+1)];
		p3 = m_coordSet[idx->getIndex(i+2)];
		
		u.setFromPoints(*p1,*p2);
		v.setFromPoints(*p1,*p3);

		*normal = u*v;
		normal->normalize();
		
		/*
		normalizedcross(u.getComponents(), v.getComponents(), n);
		normal->setComponents(n[0], n[1], n[2]);
		*/

		m_normalSet.push_back(normal);
		
		normalIdx->add(m_normalSet.size()-1);
		
		// Add normal idx to each vertex
		
		m_vertexNormalIndexSet[idx->getIndex(i)]->add(m_normalSet.size()-1);
		m_vertexNormalIndexSet[idx->getIndex(i+1)]->add(m_normalSet.size()-1);
		m_vertexNormalIndexSet[idx->getIndex(i+2)]->add(m_normalSet.size()-1);
	}
	
	m_normalIndexSet.push_back(normalIdx);
}


void CIvfTriSet::updateVertexNormals()
{
	long i, j;
	CIvfVec3d* vertexNormal;
	CIvfVec3d* normal;
	CIvfVec3d averageNormal;
	double sx, sy, sz;
	double nx, ny, nz;
	
	if (this->getUseVertexNormals())
	{
		for (i=0; i<(int)m_vertexNormalIndexSet.size(); i++)
		{
			vertexNormal = m_vertexNormalSet[i];
			
			sx = 0; sy = 0; sz = 0;
			
			for (j=0; j<m_vertexNormalIndexSet[i]->getSize(); j++)
			{
				normal = m_normalSet[m_vertexNormalIndexSet[i]->getIndex(j)];
				normal->getComponents(nx, ny, nz);
				sx += nx;
				sy += ny;
				sz += nz;
			}
			
			nx = sx/m_vertexNormalIndexSet[i]->getSize();
			ny = sy/m_vertexNormalIndexSet[i]->getSize();
			nz = sz/m_vertexNormalIndexSet[i]->getSize();
			
			vertexNormal->setComponents(nx, ny, nz);
			vertexNormal->normalize();
		}
	}
}

void CIvfTriSet::setDrawInterval(int interval)
{
	m_drawInterval = interval;
}
