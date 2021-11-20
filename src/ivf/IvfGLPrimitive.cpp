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

CGLPrimitive::CGLPrimitive()
{
	m_useVertexNormals = false;
	m_materialSet = new CMaterialSet();
}

CGLPrimitive::~CGLPrimitive()
{
	this->clear();
}

void CGLPrimitive::addCoord(double x, double y, double z)
{
	CVec3d* point = new CVec3d();
	CIndex* vertexIndex = new CIndex();
	CVec3d* vertexNormal = new CVec3d();
	point->setComponents(x, y, z);
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void CGLPrimitive::addCoord(CVec3d &coord)
{
	CVec3d* point = new CVec3d();
	(*point) = coord;
	CIndex* vertexIndex = new CIndex();
	CVec3d* vertexNormal = new CVec3d();
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void CGLPrimitive::addColor(float red, float green, float blue)
{
	CColor* color = new CColor();
	color->setColor(red, green, blue);
	m_colorSet.push_back(color);
}

void CGLPrimitive::addNormal(double n1, double n2, double n3)
{
	CVec3d* vector = new CVec3d();
	vector->setComponents(n1, n2, n3);
	vector->normalize();
	m_normalSet.push_back(vector);
}

void CGLPrimitive::addTextureCoord(double s, double t)
{
	CVec3d* point = new CVec3d();
	point->setComponents(s, t, 0.0);
	m_textureCoordSet.push_back(point);
}

void CGLPrimitive::addMaterial(CMaterial *material)
{
	m_materialSet->addMaterial(material);
}

long CGLPrimitive::getCoordSetSize()
{
	return m_coordSet.size();
}

long CGLPrimitive::getColorSetSize()
{
	return m_colorSet.size();
}

long CGLPrimitive::getNormalSetSize()
{
	return m_normalSet.size();
}

long CGLPrimitive::getTextureCoordSetSize()
{
	return m_textureCoordSet.size();;
}

int CGLPrimitive::getMaterialSetSize()
{
	return m_materialSet->getSize();
}

void CGLPrimitive::getCoord(long pos, double &x, double &y, double &z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->getComponents(x, y, z);
}

void CGLPrimitive::getColor(long pos, float &red, float &green, float &blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->getColor(red, green, blue);
}

void CGLPrimitive::getNormal(long pos, double &n1, double &n2, double &n3)
{
	if ( (pos>=0)&&(pos<(int)m_normalSet.size()) )
		m_normalSet[pos]->getComponents(n1, n2, n3);
}

CMaterial* CGLPrimitive::getMaterialAt(int pos)
{
	if ( (pos>=0)&&(pos<m_materialSet->getSize()) )
		return m_materialSet->getMaterial(pos);
	else
		return NULL;
}

void CGLPrimitive::getTextureCoord(long pos, double &s, double &t)
{
	double trash;

	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->getComponents(s, t, trash);
}

void CGLPrimitive::addCoordIndex(CIndex* index)
{
	m_coordIndexSet.push_back(CIndexPtr(index));
	this->calcNormal(index);
}

void CGLPrimitive::addColorIndex(CIndex* index)
{
	m_colorIndexSet.push_back(CIndexPtr(index));
}

void CGLPrimitive::addNormalIndex(CIndex* index)
{
	m_normalIndexSet.push_back(CIndexPtr(index));
}

void CGLPrimitive::addTextureIndex(CIndex* index)
{
	m_textureIndexSet.push_back(CIndexPtr(index));
}


CIndex* CGLPrimitive::getCoordIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_coordIndexSet.size()) )
		return m_coordIndexSet[pos];
	else
		return NULL;
}

CIndex* CGLPrimitive::getColorIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorIndexSet.size()) )
		return m_colorIndexSet[pos];
	else
		return NULL;
}

CIndex* CGLPrimitive::getNormalIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_normalIndexSet.size()) )
		return m_normalIndexSet[pos];
	else
		return NULL;
}

CIndex* CGLPrimitive::getTextureIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_textureIndexSet.size()) )
		return m_textureIndexSet[pos];
	else
		return NULL;
}

void CGLPrimitive::setUseVertexNormals(bool flag)
{
	m_useVertexNormals = flag;

	this->updateVertexNormals();
}

void CGLPrimitive::updateVertexNormals()
{

}

bool CGLPrimitive::getUseVertexNormals()
{
	return m_useVertexNormals;
}


void CGLPrimitive::calcNormal(CIndex *idx)
{

}

void CGLPrimitive::clearCoord()
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

void CGLPrimitive::clearCoordIndex()
{
	m_coordIndexSet.clear();
}

void CGLPrimitive::clearTextureCoord()
{
	long i;
	for (i=0; i<(int)m_coordSet.size(); i++)
		delete m_textureCoordSet[i];
}

void CGLPrimitive::clear()
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

void CGLPrimitive::setCoord(int pos, double x, double y, double z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->setComponents(x, y, z);
}

void CGLPrimitive::setColor(int pos, float red, float green, float blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->setColor(red, green, blue);
}

void CGLPrimitive::setTextureCoord(int pos, double s, double t)
{
	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->setComponents(s, t, 0.0);
}

void CGLPrimitive::refresh()
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

void CGLPrimitive::invertNormals()
{
	int i;
	for (i=0; i<(int)m_normalSet.size(); i++)
	{
		CVec3d* normal = m_normalSet[i];
		normal->negate();
	}
}


void CGLPrimitive::addColor(float red, float green, float blue, float alfa)
{
	CColor* color = new CColor();
	color->setColor(red, green, blue);
	color->setAlfa(alfa);
	m_colorSet.push_back(color);
}

void CGLPrimitive::setColor(int pos, float red, float green, float blue, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setColor(red, green, blue);
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void CGLPrimitive::setAlfa(int pos, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void CGLPrimitive::getColor(int pos, float &red, float &green, float &blue, float &alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->getColor(red, green, blue);
		alfa = m_colorSet[pos]->getAlfa();
	}
}

float CGLPrimitive::getAlfa(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		return m_colorSet[pos]->getAlfa();
	else
		return 0.0f;
}

void CGLPrimitive::doUpdateBoundingSphere()
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
			CVec3d* point = m_coordSet[i];
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

void CGLPrimitive::addMaterialIndex(int idx)
{
	m_materialIndexSet.push_back(idx);
}

int CGLPrimitive::getMaterialIndex(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_materialIndexSet.size()) )
		return m_materialIndexSet[pos];
	else
		return -1;
}

void CGLPrimitive::clearMaterialIndex()
{
	m_materialIndexSet.clear();
}

void CGLPrimitive::setMaterialSet(CMaterialSet *materialSet)
{
	m_materialSet = materialSet;
}

CMaterialSet* CGLPrimitive::getMaterialSet()
{
	return m_materialSet;
}

void CGLPrimitive::clearMaterial()
{
	m_materialSet->clear();
}


int CGLPrimitive::getCoordIndexSetSize()
{
	return m_coordIndexSet.size();
}
