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
#include <ivf/rc.h>
#include <ivf/ShaderProgram.h>

#include <ivf/config.h>

#include <vector>

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
	if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
	if (m_vbo) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
	this->clear();
}

// ------------------------------------------------------------
bool GLPrimitive::buildAndDrawVAO(GLenum legacyPrimitive)
{
	ShaderProgram* prog = rcShader();
	if (!rcIsShaderActive())
		return false;

	if (m_vaoDirty) {
		struct GpuVertex {
			float position[3];
			float normal[3];
			float texcoord[2];
			float color[4];
		};

		std::vector<GpuVertex> packed;

		for (int i = 0; i < (int)m_coordIndexSet.size(); ++i) {
			Index* coordIdx = m_coordIndexSet[i];
			Index* normalIdx  = (i < (int)m_normalIndexSet.size())  ? (Index*)m_normalIndexSet[i]  : nullptr;
			Index* texIdx     = (i < (int)m_textureIndexSet.size())  ? (Index*)m_textureIndexSet[i]  : nullptr;
			Index* colorIdx   = (i < (int)m_colorIndexSet.size())   ? (Index*)m_colorIndexSet[i]   : nullptr;
			int n = coordIdx->getSize();

			// Determine effective primitive for this coordIndex.
			// GL_QUADS is used as the "mixed-topology" sentinel: the actual topology
			// is read from coordIdx itself (IVF_IDX_QUADS=1, else triangles).
			GLenum effPrim = legacyPrimitive;
			if (legacyPrimitive == GL_QUADS)
				effPrim = (coordIdx->getTopology() == 1) ? GL_QUADS : GL_TRIANGLES;

			// How many vertices form one primitive unit (for face-normal indexing).
			int vertsPerFace = 3;
			if (effPrim == GL_QUADS)      vertsPerFace = 4;
			else if (effPrim == GL_LINES)  vertsPerFace = 2;
			else if (effPrim == GL_POINTS) vertsPerFace = 1;

			// Pack one vertex at flat position j within this coordIndex.
			auto getVertex = [&](int j, int faceIdx) -> GpuVertex {
				GpuVertex v = {};
				int ci = coordIdx->getIndex(j);

				// Position
				if (ci < (int)m_coordSet.size()) {
					const double* p = m_coordSet[ci]->getComponents();
					v.position[0] = (float)p[0];
					v.position[1] = (float)p[1];
					v.position[2] = (float)p[2];
				}

				// Normal: vertex normal takes priority over face normal
				if (getUseVertexNormals() && ci < (int)m_vertexNormalSet.size()) {
					const double* n3 = m_vertexNormalSet[ci]->getComponents();
					v.normal[0] = (float)n3[0]; v.normal[1] = (float)n3[1]; v.normal[2] = (float)n3[2];
				} else if (normalIdx && faceIdx < normalIdx->getSize()) {
					int ni = normalIdx->getIndex(faceIdx);
					if (ni < (int)m_normalSet.size()) {
						const double* n3 = m_normalSet[ni]->getComponents();
						v.normal[0] = (float)n3[0]; v.normal[1] = (float)n3[1]; v.normal[2] = (float)n3[2];
					}
				}

				// Texture coordinate
				if (texIdx && j < texIdx->getSize()) {
					int ti = texIdx->getIndex(j);
					if (ti < (int)m_textureCoordSet.size()) {
						const double* tc = m_textureCoordSet[ti]->getComponents();
						v.texcoord[0] = (float)tc[0]; v.texcoord[1] = (float)tc[1];
					}
				}

				// Color
				if (colorIdx && j < colorIdx->getSize()) {
					int ki = colorIdx->getIndex(j);
					if (ki < (int)m_colorSet.size()) {
						const float* c = m_colorSet[ki]->getColor();
						v.color[0] = c[0]; v.color[1] = c[1]; v.color[2] = c[2];
						v.color[3] = m_colorSet[ki]->getAlfa();
					}
				} else {
					v.color[0] = v.color[1] = v.color[2] = v.color[3] = 1.0f;
				}

				return v;
			};

			if (effPrim == GL_QUADS) {
				// Split each quad (j, j+1, j+2, j+3) into two triangles.
				for (int j = 0; j + 3 < n; j += 4) {
					int fi = j / 4;
					packed.push_back(getVertex(j,   fi));
					packed.push_back(getVertex(j+1, fi));
					packed.push_back(getVertex(j+2, fi));
					packed.push_back(getVertex(j,   fi));
					packed.push_back(getVertex(j+2, fi));
					packed.push_back(getVertex(j+3, fi));
				}
			} else if (effPrim == GL_QUAD_STRIP) {
				// GL_QUAD_STRIP pairs: (j,j+1,j+3,j+2) → 2 triangles each step.
				for (int j = 0; j + 3 < n; j += 2) {
					int fi = j / 2;
					packed.push_back(getVertex(j,   fi));
					packed.push_back(getVertex(j+1, fi));
					packed.push_back(getVertex(j+3, fi));
					packed.push_back(getVertex(j,   fi));
					packed.push_back(getVertex(j+3, fi));
					packed.push_back(getVertex(j+2, fi));
				}
			} else {
				// GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP, GL_POINTS
				for (int j = 0; j < n; ++j) {
					int fi = (vertsPerFace > 1) ? j / vertsPerFace : j;
					packed.push_back(getVertex(j, fi));
				}
			}
		}

		m_vaoVertexCount = (GLsizei)packed.size();

		if (m_vao == 0) glGenVertexArrays(1, &m_vao);
		if (m_vbo == 0) glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(packed.size() * sizeof(GpuVertex)),
		             packed.data(), GL_DYNAMIC_DRAW);

		GLsizei stride = sizeof(GpuVertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, texcoord));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, color));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
		m_vaoDirty = false;
	}

	if (m_vaoVertexCount == 0)
		return true; // nothing to draw, but still handled

	// Determine actual draw mode (quads → triangles in core profile).
	GLenum drawMode;
	switch (legacyPrimitive) {
		case GL_QUADS:      drawMode = GL_TRIANGLES; break;
		case GL_QUAD_STRIP: drawMode = GL_TRIANGLES; break;
		default:            drawMode = legacyPrimitive; break;
	}

	rcUseShader();
	rcUpdateShader(prog);

	glBindVertexArray(m_vao);
	glDrawArrays(drawMode, 0, m_vaoVertexCount);
	glBindVertexArray(0);

	return true;
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
	for (i=0; i<(int)m_textureCoordSet.size(); i++)
		delete m_textureCoordSet[i];
	m_textureCoordSet.clear();
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

	m_vaoDirty = true; // geometry changed — rebuild VAO next draw
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
