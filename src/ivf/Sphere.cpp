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

// Implementation of: public class Sphere

#include <ivf/config.h>
#include <ivf/Sphere.h>
#include <ivf/rc.h>

#include <cmath>
#include <vector>

using namespace ivf;

// ------------------------------------------------------------
Sphere::Sphere ()
		:Shape()
		//TODO: check and complete member initialisation list!
{
	m_radius = 1.0;
	m_slices = 8;
	m_stacks = 8;
	m_selectionBox = new SelectionBox();
	m_selectionBox->setUseName(false);
	m_qobj = gluNewQuadric();
	gluQuadricDrawStyle(m_qobj, GLU_FILL);
	gluQuadricTexture(m_qobj, GL_TRUE);
	gluQuadricOrientation(m_qobj, GLU_OUTSIDE);
	gluQuadricNormals(m_qobj, GLU_SMOOTH);
}

// ------------------------------------------------------------
Sphere::~Sphere ()
{
	if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
	if (m_vbo) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
	if (m_ebo) { glDeleteBuffers(1, &m_ebo); m_ebo = 0; }
	gluDeleteQuadric(m_qobj);
	delete m_selectionBox;
}

// ------------------------------------------------------------
void Sphere::buildVAO()
{
	struct GpuVertex {
		float position[3];
		float normal[3];
		float texcoord[2];
		float color[4];
	};

	std::vector<GpuVertex> verts;
	std::vector<unsigned int> indices;

	float r = (float)m_radius;

	for (int i = 0; i <= m_stacks; ++i) {
		float phi   = (float)M_PI * i / m_stacks;
		float sinPhi = sinf(phi);
		float cosPhi = cosf(phi);
		for (int j = 0; j <= m_slices; ++j) {
			float theta    = 2.0f * (float)M_PI * j / m_slices;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);

			float nx = sinPhi * cosTheta;
			float ny = cosPhi;
			float nz = sinPhi * sinTheta;

			GpuVertex v;
			v.position[0] = r * nx; v.position[1] = r * ny; v.position[2] = r * nz;
			v.normal[0]   = nx;     v.normal[1]   = ny;     v.normal[2]   = nz;
			v.texcoord[0] = (float)j / m_slices;
			v.texcoord[1] = (float)i / m_stacks;
			v.color[0]    = 1.0f; v.color[1] = 1.0f; v.color[2] = 1.0f; v.color[3] = 1.0f;
			verts.push_back(v);
		}
	}

	for (int i = 0; i < m_stacks; ++i) {
		for (int j = 0; j < m_slices; ++j) {
			unsigned int a = i * (m_slices + 1) + j;
			unsigned int b = a + m_slices + 1;
			indices.push_back(a);   indices.push_back(b);   indices.push_back(a + 1);
			indices.push_back(b);   indices.push_back(b + 1); indices.push_back(a + 1);
		}
	}

	m_eboCount = (int)indices.size();

	if (m_vao == 0) glGenVertexArrays(1, &m_vao);
	if (m_vbo == 0) glGenBuffers(1, &m_vbo);
	if (m_ebo == 0) glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(verts.size() * sizeof(GpuVertex)), verts.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(indices.size() * sizeof(unsigned int)), indices.data(), GL_DYNAMIC_DRAW);

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

// ------------------------------------------------------------
void Sphere::setRadius (const double radius)
{
	m_radius = radius;
	m_vaoDirty = true;
	updateSelectBox();
}

// ------------------------------------------------------------
double Sphere::getRadius ()
{
	return m_radius;
}

// ------------------------------------------------------------
void Sphere::doCreateGeometry()
{
	if (rcIsShaderActive()) {
		if (m_vaoDirty)
			buildVAO();
		rcUseShader();
		rcUpdateShader();
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_eboCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		return;
	}

	// Legacy path
	glPushMatrix();
		glRotated(90, 1.0, 0.0, 0.0);
		gluSphere(m_qobj, m_radius, m_slices, m_stacks);
	glPopMatrix();
}

// ------------------------------------------------------------
void Sphere::setSlices(const int slices)
{
	m_slices = slices;
	m_vaoDirty = true;
}

// ------------------------------------------------------------
int Sphere::getSlices()
{
	return m_slices;
}

// ------------------------------------------------------------
void Sphere::setStacks(const int stacks)
{
	m_stacks = stacks;
	m_vaoDirty = true;
}

// ------------------------------------------------------------
int Sphere::getStacks()
{
	return m_stacks;
}

// ------------------------------------------------------------
void Sphere::updateSelectBox()
{
	m_selectionBox->setSize(m_radius*2.0, m_radius*2.0, m_radius*2.0);
}

// ------------------------------------------------------------
void Sphere::doCreateSelect()
{
	m_selectionBox->render();
}

// ------------------------------------------------------------
void Sphere::doUpdateBoundingSphere()
{
	// This is very simple

	if (getBoundingSphere()!=nullptr)
		getBoundingSphere()->setRadius(m_radius);
}
