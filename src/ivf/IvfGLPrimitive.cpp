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

#include <ivf/IvfGLPrimitive.h>

#include <ivf/ivfconfig.h>

// ------------------------------------------------------------
// ------------------------------------------------------------

CIvfGLPrimitive::CIvfGLPrimitive()
{
	m_useVertexNormals = false;
	m_materialSet = new CIvfMaterialSet();
}

CIvfGLPrimitive::~CIvfGLPrimitive()
{
	this->clear();
}

void CIvfGLPrimitive::addCoord(double x, double y, double z)
{
	CIvfVec3d* point = new CIvfVec3d();
	CIvfIndex* vertexIndex = new CIvfIndex();
	CIvfVec3d* vertexNormal = new CIvfVec3d();
	point->setComponents(x, y, z);
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void CIvfGLPrimitive::addCoord(CIvfVec3d &coord)
{
	CIvfVec3d* point = new CIvfVec3d();
	(*point) = coord;
	CIvfIndex* vertexIndex = new CIvfIndex();
	CIvfVec3d* vertexNormal = new CIvfVec3d();
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void CIvfGLPrimitive::addColor(float red, float green, float blue)
{
	CIvfColor* color = new CIvfColor();
	color->setColor(red, green, blue);
	m_colorSet.push_back(color);
}

void CIvfGLPrimitive::addNormal(double n1, double n2, double n3)
{
	CIvfVec3d* vector = new CIvfVec3d();
	vector->setComponents(n1, n2, n3);
	vector->normalize();
	m_normalSet.push_back(vector);
}

void CIvfGLPrimitive::addTextureCoord(double s, double t)
{
	CIvfVec3d* point = new CIvfVec3d();
	point->setComponents(s, t, 0.0);
	m_textureCoordSet.push_back(point);
}

void CIvfGLPrimitive::addMaterial(CIvfMaterial *material)
{
	m_materialSet->addMaterial(material);
}

long CIvfGLPrimitive::getCoordSetSize()
{
	return m_coordSet.size();
}

long CIvfGLPrimitive::getColorSetSize()
{
	return m_colorSet.size();
}

long CIvfGLPrimitive::getNormalSetSize()
{
	return m_normalSet.size();
}

long CIvfGLPrimitive::getTextureCoordSetSize()
{
	return m_textureCoordSet.size();;
}

int CIvfGLPrimitive::getMaterialSetSize()
{
	return m_materialSet->getSize();
}

void CIvfGLPrimitive::getCoord(long pos, double &x, double &y, double &z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->getComponents(x, y, z);
}

void CIvfGLPrimitive::getColor(long pos, float &red, float &green, float &blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->getColor(red, green, blue);
}

void CIvfGLPrimitive::getNormal(long pos, double &n1, double &n2, double &n3)
{
	if ( (pos>=0)&&(pos<(int)m_normalSet.size()) )
		m_normalSet[pos]->getComponents(n1, n2, n3);
}

CIvfMaterial* CIvfGLPrimitive::getMaterial(int pos)
{
	if ( (pos>=0)&&(pos<m_materialSet->getSize()) )
		return m_materialSet->getMaterial(pos);
	else
		return NULL;
}

void CIvfGLPrimitive::getTextureCoord(long pos, double &s, double &t)
{
	double trash;

	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->getComponents(s, t, trash);
}

void CIvfGLPrimitive::addCoordIndex(CIvfIndex* index)
{
	m_coordIndexSet.push_back(CIvfIndexPtr(index));
	this->calcNormal(index);
}

void CIvfGLPrimitive::addColorIndex(CIvfIndex* index)
{
	m_colorIndexSet.push_back(CIvfIndexPtr(index));
}

void CIvfGLPrimitive::addNormalIndex(CIvfIndex* index)
{
	m_normalIndexSet.push_back(CIvfIndexPtr(index));
}

void CIvfGLPrimitive::addTextureIndex(CIvfIndex* index)
{
	m_textureIndexSet.push_back(CIvfIndexPtr(index));
}


CIvfIndex* CIvfGLPrimitive::getCoordIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_coordIndexSet.size()) )
		return m_coordIndexSet[pos];
	else
		return NULL;
}

CIvfIndex* CIvfGLPrimitive::getColorIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorIndexSet.size()) )
		return m_colorIndexSet[pos];
	else
		return NULL;
}

CIvfIndex* CIvfGLPrimitive::getNormalIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_normalIndexSet.size()) )
		return m_normalIndexSet[pos];
	else
		return NULL;
}

CIvfIndex* CIvfGLPrimitive::getTextureIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_textureIndexSet.size()) )
		return m_textureIndexSet[pos];
	else
		return NULL;
}

void CIvfGLPrimitive::setUseVertexNormals(bool flag)
{
	m_useVertexNormals = flag;

	this->updateVertexNormals();
}

void CIvfGLPrimitive::updateVertexNormals()
{

}

bool CIvfGLPrimitive::getUseVertexNormals()
{
	return m_useVertexNormals;
}


void CIvfGLPrimitive::calcNormal(CIvfIndex *idx)
{

}

void CIvfGLPrimitive::clearCoord()
{
	long i;
	for (i=0; i<(int)m_coordSet.size(); i++)
	{
		delete m_coordSet[i];
		delete m_vertexNormalSet[i];
		//delete m_vertexNormalIndexSet[i];
	}
	m_coordSet.clear();
	m_vertexNormalSet.clear();
	m_vertexNormalIndexSet.clear();
}

void CIvfGLPrimitive::clearCoordIndex()
{
	m_coordIndexSet.clear();
}

void CIvfGLPrimitive::clearTextureCoord()
{
	long i;
	for (i=0; i<(int)m_coordSet.size(); i++)
		delete m_textureCoordSet[i];
}

void CIvfGLPrimitive::clear()
{
	long i;
	for (i=0; i<(int)m_coordSet.size(); i++)
		delete m_coordSet[i];
	for (i=0; i<(int)m_colorSet.size(); i++)
		delete m_colorSet[i];
	for (i=0; i<(int)m_normalSet.size(); i++)
		delete m_normalSet[i];
	for (i=0; i<(int)m_vertexNormalSet.size(); i++)
		delete m_vertexNormalSet[i];
	for (i=0; i<(int)m_textureCoordSet.size(); i++)
		delete m_textureCoordSet[i];

	m_materialSet->deleteReference();
	if (!m_materialSet->referenced())
		m_materialSet->clear();
	m_materialSet->addReference();

	m_coordSet.clear();
	m_coordIndexSet.clear();
	m_colorSet.clear();

	m_colorIndexSet.clear();
	m_normalSet.clear();
	m_normalIndexSet.clear();
	m_vertexNormalSet.clear();
	m_vertexNormalIndexSet.clear();
	m_textureCoordSet.clear();
	m_textureIndexSet.clear();
}

void CIvfGLPrimitive::setCoord(int pos, double x, double y, double z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->setComponents(x, y, z);
}

void CIvfGLPrimitive::setColor(int pos, float red, float green, float blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->setColor(red, green, blue);
}

void CIvfGLPrimitive::setTextureCoord(int pos, double s, double t)
{
	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->setComponents(s, t, 0.0);
}

void CIvfGLPrimitive::refresh()
{
	int i;

	for (i=0; i<(int)m_normalSet.size(); i++)
		delete m_normalSet[i];
	//for (i=0; i<m_normalIndexSet.size(); i++)
	//	delete m_normalIndexSet[i];

	m_normalSet.clear();
	m_normalIndexSet.clear();

	for (i=0; i<(int)m_coordIndexSet.size(); i++)
		this->calcNormal(m_coordIndexSet[i]);

	if (m_useVertexNormals)
		this->updateVertexNormals();
}

void CIvfGLPrimitive::addColor(float red, float green, float blue, float alfa)
{
	CIvfColor* color = new CIvfColor();
	color->setColor(red, green, blue);
	color->setAlfa(alfa);
	m_colorSet.push_back(color);
}

void CIvfGLPrimitive::setColor(int pos, float red, float green, float blue, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setColor(red, green, blue);
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void CIvfGLPrimitive::setAlfa(int pos, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void CIvfGLPrimitive::getColor(int pos, float &red, float &green, float &blue, float &alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->getColor(red, green, blue);
		alfa = m_colorSet[pos]->getAlfa();
	}
}

float CIvfGLPrimitive::getAlfa(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		return m_colorSet[pos]->getAlfa();
	else
		return 0.0f;
}

void CIvfGLPrimitive::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=NULL)
	{
		int i;
		double maxSize[3];
		double minSize[3];
		double midPoint[3];
		double x, y, z;
		double radius;

		maxSize[0] = maxSize[1] = maxSize[2] = -1e300;
		minSize[0] = minSize[1] = minSize[2] = 1e300;

		for (i=0; i<(int)m_coordSet.size(); i++)
		{
			CIvfVec3d* point = m_coordSet[i];
			point->getComponents(x, y, z);
			if (x>maxSize[0])
				maxSize[0] = x;
			if (y>maxSize[1])
				maxSize[1] = y;
			if (z>maxSize[2])
				maxSize[2] = z;
			if (x<minSize[0])
				minSize[0] = x;
			if (y<minSize[1])
				minSize[1] = y;
			if (z<minSize[2])
				minSize[2] = z;
		}

		midPoint[0] = (maxSize[0] + minSize[0])/2.0;
		midPoint[1] = (maxSize[1] + minSize[1])/2.0;
		midPoint[2] = (maxSize[2] + minSize[2])/2.0;

		radius = sqrt(
			pow(maxSize[0]-midPoint[0],2) +
			pow(maxSize[1]-midPoint[1],2) +
			pow(maxSize[2]-midPoint[2],2));

		getBoundingSphere()->setRadius(radius);
	}
}

void CIvfGLPrimitive::addMaterialIndex(int idx)
{
	m_materialIndexSet.push_back(idx);
}

int CIvfGLPrimitive::getMaterialIndex(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_materialIndexSet.size()) )
		return m_materialIndexSet[pos];
	else
		return -1;
}

void CIvfGLPrimitive::clearMaterialIndex()
{
	m_materialIndexSet.clear();
}

void CIvfGLPrimitive::setMaterialSet(CIvfMaterialSet *materialSet)
{
	m_materialSet = materialSet;
}

CIvfMaterialSet* CIvfGLPrimitive::getMaterialSet()
{
	return m_materialSet;
}

void CIvfGLPrimitive::clearMaterial()
{
	m_materialSet->clear();
}


int CIvfGLPrimitive::getCoordIndexSetSize()
{
	return m_coordIndexSet.size();
}
