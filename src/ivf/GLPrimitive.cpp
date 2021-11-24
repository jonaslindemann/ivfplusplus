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

#include <ivf/GLPrimitive.h>

#include <ivf/config.h>

using namespace ivf;

// ------------------------------------------------------------
// ------------------------------------------------------------

GLPrimitive::GLPrimitive()
{
	m_useVertexNormals = false;
	m_materialSet = new MaterialSet();
}

GLPrimitive::~GLPrimitive()
{
	this->clear();
}

void GLPrimitive::addCoord(double x, double y, double z)
{
	Vec3d* point = new Vec3d();
	Index* vertexIndex = new Index();
	Vec3d* vertexNormal = new Vec3d();
	point->setComponents(x, y, z);
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void GLPrimitive::addCoord(Vec3d &coord)
{
	Vec3d* point = new Vec3d();
	(*point) = coord;
	Index* vertexIndex = new Index();
	Vec3d* vertexNormal = new Vec3d();
	m_coordSet.push_back(point);
	m_vertexNormalIndexSet.push_back(vertexIndex);
	m_vertexNormalSet.push_back(vertexNormal);
}

void GLPrimitive::addColor(float red, float green, float blue)
{
	Color* color = new Color();
	color->setColor(red, green, blue);
	m_colorSet.push_back(color);
}

void GLPrimitive::addNormal(double n1, double n2, double n3)
{
	Vec3d* vector = new Vec3d();
	vector->setComponents(n1, n2, n3);
	vector->normalize();
	m_normalSet.push_back(vector);
}

void GLPrimitive::addTextureCoord(double s, double t)
{
	Vec3d* point = new Vec3d();
	point->setComponents(s, t, 0.0);
	m_textureCoordSet.push_back(point);
}

void GLPrimitive::addMaterial(Material *material)
{
	m_materialSet->addMaterial(material);
}

long GLPrimitive::getCoordSetSize()
{
	return static_cast<int>(m_coordSet.size());
}

long GLPrimitive::getColorSetSize()
{
	return static_cast<int>(m_colorSet.size());
}

long GLPrimitive::getNormalSetSize()
{
	return static_cast<int>(m_normalSet.size());
}

long GLPrimitive::getTextureCoordSetSize()
{
	return static_cast<int>(m_textureCoordSet.size());
}

int GLPrimitive::getMaterialSetSize()
{
	return m_materialSet->getSize();
}

void GLPrimitive::getCoord(long pos, double &x, double &y, double &z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->getComponents(x, y, z);
}

void GLPrimitive::getColor(long pos, float &red, float &green, float &blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->getColor(red, green, blue);
}

void GLPrimitive::getNormal(long pos, double &n1, double &n2, double &n3)
{
	if ( (pos>=0)&&(pos<(int)m_normalSet.size()) )
		m_normalSet[pos]->getComponents(n1, n2, n3);
}

Material* GLPrimitive::getMaterialAt(int pos)
{
	if ( (pos>=0)&&(pos<m_materialSet->getSize()) )
		return m_materialSet->getMaterial(pos);
	else
		return nullptr;
}

void GLPrimitive::getTextureCoord(long pos, double &s, double &t)
{
	double trash;

	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->getComponents(s, t, trash);
}

void GLPrimitive::addCoordIndex(Index* index)
{
	m_coordIndexSet.push_back(IndexPtr(index));
	this->calcNormal(index);
}

void GLPrimitive::addColorIndex(Index* index)
{
	m_colorIndexSet.push_back(IndexPtr(index));
}

void GLPrimitive::addNormalIndex(Index* index)
{
	m_normalIndexSet.push_back(IndexPtr(index));
}

void GLPrimitive::addTextureIndex(Index* index)
{
	m_textureIndexSet.push_back(IndexPtr(index));
}


Index* GLPrimitive::getCoordIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_coordIndexSet.size()) )
		return m_coordIndexSet[pos];
	else
		return nullptr;
}

Index* GLPrimitive::getColorIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorIndexSet.size()) )
		return m_colorIndexSet[pos];
	else
		return nullptr;
}

Index* GLPrimitive::getNormalIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_normalIndexSet.size()) )
		return m_normalIndexSet[pos];
	else
		return nullptr;
}

Index* GLPrimitive::getTextureIndex(long pos)
{
	if ( (pos>=0)&&(pos<(int)m_textureIndexSet.size()) )
		return m_textureIndexSet[pos];
	else
		return nullptr;
}

void GLPrimitive::setUseVertexNormals(bool flag)
{
	m_useVertexNormals = flag;

	this->updateVertexNormals();
}

void GLPrimitive::updateVertexNormals()
{

}

bool GLPrimitive::getUseVertexNormals()
{
	return m_useVertexNormals;
}


void GLPrimitive::calcNormal(Index *idx)
{

}

void GLPrimitive::clearCoord()
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

void GLPrimitive::clearCoordIndex()
{
	m_coordIndexSet.clear();
}

void GLPrimitive::clearTextureCoord()
{
	long i;
	for (i=0; i<(int)m_coordSet.size(); i++)
		delete m_textureCoordSet[i];
}

void GLPrimitive::clear()
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

void GLPrimitive::setCoord(int pos, double x, double y, double z)
{
	if ( (pos>=0)&&(pos<(int)m_coordSet.size()) )
		m_coordSet[pos]->setComponents(x, y, z);
}

void GLPrimitive::setColor(int pos, float red, float green, float blue)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		m_colorSet[pos]->setColor(red, green, blue);
}

void GLPrimitive::setTextureCoord(int pos, double s, double t)
{
	if ( (pos>=0)&&(pos<(int)m_textureCoordSet.size()) )
		m_textureCoordSet[pos]->setComponents(s, t, 0.0);
}

void GLPrimitive::refresh()
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

void GLPrimitive::invertNormals()
{
	int i;
	for (i=0; i<(int)m_normalSet.size(); i++)
	{
		Vec3d* normal = m_normalSet[i];
		normal->negate();
	}
}


void GLPrimitive::addColor(float red, float green, float blue, float alfa)
{
	Color* color = new Color();
	color->setColor(red, green, blue);
	color->setAlfa(alfa);
	m_colorSet.push_back(color);
}

void GLPrimitive::setColor(int pos, float red, float green, float blue, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setColor(red, green, blue);
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void GLPrimitive::setAlfa(int pos, float alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->setAlfa(alfa);
	}
}

void GLPrimitive::getColor(int pos, float &red, float &green, float &blue, float &alfa)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
	{
		m_colorSet[pos]->getColor(red, green, blue);
		alfa = m_colorSet[pos]->getAlfa();
	}
}

float GLPrimitive::getAlfa(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_colorSet.size()) )
		return m_colorSet[pos]->getAlfa();
	else
		return 0.0f;
}

void GLPrimitive::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
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
			Vec3d* point = m_coordSet[i];
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

void GLPrimitive::addMaterialIndex(int idx)
{
	m_materialIndexSet.push_back(idx);
}

int GLPrimitive::getMaterialIndex(int pos)
{
	if ( (pos>=0)&&(pos<(int)m_materialIndexSet.size()) )
		return m_materialIndexSet[pos];
	else
		return -1;
}

void GLPrimitive::clearMaterialIndex()
{
	m_materialIndexSet.clear();
}

void GLPrimitive::setMaterialSet(MaterialSet *materialSet)
{
	m_materialSet = materialSet;
}

MaterialSet* GLPrimitive::getMaterialSet()
{
	return m_materialSet;
}

void GLPrimitive::clearMaterial()
{
	m_materialSet->clear();
}


int GLPrimitive::getCoordIndexSetSize()
{
	return static_cast<int>(m_coordIndexSet.size());
}
