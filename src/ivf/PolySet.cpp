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

#include <ivf/PolySet.h>
#include <ivf/GlobalState.h>

using namespace ivf;

PolySet::PolySet()
{
	m_useColor = false;
}

PolySet::~PolySet()
{

}

void PolySet::doCreateGeometry()
{
	Index* coordIdx;
	Index* colorIdx;
	Index* normalIdx;
	Index* textureIdx;
	int topoHop = 4;

	long i, j;

	glPushAttrib(GL_COLOR_MATERIAL);

	if (m_useColor)
		glEnable(GL_COLOR_MATERIAL);


	for (i=0; i<(int)m_coordIndexSet.size(); i++)
	{
		if (!m_useColor)
		{
			if (GlobalState::getInstance()->isMaterialRenderingEnabled())
			{
				int idx = this->getMaterialIndex(i);
				Material* material;

				if (idx>=0)
				{
					material = this->getMaterialAt(idx);

					if (material!=nullptr)
						material->render();
				}
			}
		}

		coordIdx = m_coordIndexSet[i];
		if (coordIdx->getTopology()==IVF_IDX_QUADS)
		{
			glBegin(GL_QUADS);
			topoHop = 4;
		}
		else
		{
			glBegin(GL_TRIANGLES);
			topoHop = 3;
		}

		if (m_useColor)
		{
			if (i<(int)m_colorIndexSet.size())
				colorIdx = m_colorIndexSet[i];
		}
		normalIdx = m_normalIndexSet[i];

		if (i<(int)m_textureIndexSet.size())
			textureIdx = m_textureIndexSet[i];
		else
			textureIdx = nullptr;

		for (j=0; j<coordIdx->getSize(); j+=topoHop)
		{
			if (getUseVertexNormals())
				glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j)]->getComponents());
			else
				glNormal3dv(m_normalSet[normalIdx->getIndex(j/topoHop)]->getComponents());

			if (m_useColor)
				glColor3fv(m_colorSet[colorIdx->getIndex(j)]->getColor());

			if (textureIdx!=nullptr)
				glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j)]->getComponents());

			glVertex3dv(m_coordSet[coordIdx->getIndex(j)]->getComponents());

			if (getUseVertexNormals())
				glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j+1)]->getComponents());
			else
				glNormal3dv(m_normalSet[normalIdx->getIndex(j/topoHop)]->getComponents());

			if (m_useColor)
				glColor3fv(m_colorSet[colorIdx->getIndex(j+1)]->getColor());

			if (textureIdx!=nullptr)
				glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j+1)]->getComponents());

			glVertex3dv(m_coordSet[coordIdx->getIndex(j+1)]->getComponents());

			if (getUseVertexNormals())
				glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j+2)]->getComponents());
			else
				glNormal3dv(m_normalSet[normalIdx->getIndex(j/topoHop)]->getComponents());

			if (m_useColor)
				glColor3fv(m_colorSet[colorIdx->getIndex(j+2)]->getColor());

			if (textureIdx!=nullptr)
				glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j+2)]->getComponents());

			glVertex3dv(m_coordSet[coordIdx->getIndex(j+2)]->getComponents());

			if (topoHop==4)
			{
				if (getUseVertexNormals())
					glNormal3dv(m_vertexNormalSet[coordIdx->getIndex(j+3)]->getComponents());
				else
					glNormal3dv(m_normalSet[normalIdx->getIndex(j/topoHop)]->getComponents());

				if (m_useColor)
					glColor3fv(m_colorSet[colorIdx->getIndex(j+3)]->getColor());

				if (textureIdx!=nullptr)
					glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j+3)]->getComponents());

				glVertex3dv(m_coordSet[coordIdx->getIndex(j+3)]->getComponents());
			}
		}
		glEnd();
	}

	glPopAttrib();
}

void PolySet::setUseColor(bool flag)
{
	m_useColor = flag;
}

bool PolySet::getUseColor()
{
	return m_useColor;
}

void PolySet::calcNormal(Index *idx)
{
	long i;
	Vec3d* p1;
	Vec3d* p2;
	Vec3d* p3;
	Vec3d u;
	Vec3d v;
	Index* normalIdx;
	Vec3d n;
	//double n[3];

	normalIdx = new Index();

	if (idx->getTopology()==IVF_IDX_QUADS)
	{
		for (i=0; i<idx->getSize(); i+=4)
		{
			Vec3d* normal = new Vec3d();

			p1 = m_coordSet[idx->getIndex(i)];
			p2 = m_coordSet[idx->getIndex(i+1)];
			p3 = m_coordSet[idx->getIndex(i+3)];

			u.setFromPoints(*p1,*p2);
			v.setFromPoints(*p1,*p3);

			*normal = u*v; // cross product
			(*normal).normalize();

			/*
			normalizedcross(u.getComponents(), v.getComponents(), n);

			normal->setComponents(n[0], n[1], n[2]);
			*/

			m_normalSet.push_back(normal);

			normalIdx->add(static_cast<int>(m_normalSet.size())-1);

			// Add normal idx to each vertex

			m_vertexNormalIndexSet[idx->getIndex(i)]->add(static_cast<int>(m_normalSet.size())-1);
			m_vertexNormalIndexSet[idx->getIndex(i+1)]->add(static_cast<int>(m_normalSet.size())-1);
			m_vertexNormalIndexSet[idx->getIndex(i+2)]->add(static_cast<int>(m_normalSet.size())-1);
			m_vertexNormalIndexSet[idx->getIndex(i+3)]->add(static_cast<int>(m_normalSet.size())-1);
		}
	}
	else
	{
		for (i=0; i<idx->getSize(); i+=3)
		{
			Vec3d* normal = new Vec3d();

			p1 = m_coordSet[idx->getIndex(i)];
			p2 = m_coordSet[idx->getIndex(i+1)];
			p3 = m_coordSet[idx->getIndex(i+2)];

			u.setFromPoints(*p1,*p2);
			v.setFromPoints(*p1,*p3);

			*normal = u*v; // cross product
			(*normal).normalize();

			/*
			normalizedcross(u.getComponents(), v.getComponents(), n);
			normal->setComponents(n[0], n[1], n[2]);
			*/

			m_normalSet.push_back(normal);

			normalIdx->add(static_cast<int>(m_normalSet.size())-1);

			// Add normal idx to each vertex

			m_vertexNormalIndexSet[idx->getIndex(i)]->add(static_cast<int>(m_normalSet.size())-1);
			m_vertexNormalIndexSet[idx->getIndex(i+1)]->add(static_cast<int>(m_normalSet.size())-1);
			m_vertexNormalIndexSet[idx->getIndex(i+2)]->add(static_cast<int>(m_normalSet.size())-1);
		}
	}

	m_normalIndexSet.push_back(normalIdx);
}


void PolySet::updateVertexNormals()
{
	long i, j;
	Vec3d* vertexNormal;
	Vec3d* normal;
	Vec3d averageNormal;
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
