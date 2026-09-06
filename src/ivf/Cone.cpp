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

#include <ivf/Cone.h>
#include <ivf/rc.h>

#include <ivf/config.h>

#include <cmath>
#include <vector>

#define PI 3.141592653589793

using namespace ivf;

// ------------------------------------------------------------
Cone::Cone ()
		:Shape()
		//TODO: check and complete member initialisation list!
{
	m_bottomRadius = 1.0;
	m_topRadius = 0.0;
	m_height = 2.0;
	m_slices = 8;
	m_stacks = 1;
	m_selectionBox = new SelectionBox();
	updateSelectBox();
}

// ------------------------------------------------------------
Cone::~Cone ()
{
	if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
	if (m_vbo) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
	if (m_ebo) { glDeleteBuffers(1, &m_ebo); m_ebo = 0; }
	delete m_selectionBox;
}

// ------------------------------------------------------------
void Cone::setTopRadius (const double radius)
{
	m_topRadius = radius;
	m_vaoDirty = true;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getTopRadius ()
{
	return m_topRadius;
	updateSelectBox();
}

// ------------------------------------------------------------
void Cone::setBottomRadius (const double radius)
{
	m_bottomRadius = radius;
	m_vaoDirty = true;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getBottomRadius ()
{
	return m_bottomRadius;
}

// ------------------------------------------------------------
void Cone::setHeight (const double height)
{
	m_height = height;
	m_vaoDirty = true;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getHeight ()
{
	return m_height;
}

void Cone::setSlices(int slices)
{
	m_slices = slices;
	m_vaoDirty = true;
}

int Cone::getSlices()
{
	return m_slices;
}

void Cone::setStacks(int stacks)
{
	m_stacks = stacks;
	m_vaoDirty = true;
}

int Cone::getStacks()
{
	return m_stacks;
}

// ------------------------------------------------------------
void Cone::buildVAO()
{
	struct GpuVertex {
		float position[3];
		float normal[3];
		float texcoord[2];
		float color[4];
	};

	std::vector<GpuVertex> verts;
	std::vector<unsigned int> indices;

	float h  = (float)m_height;
	float rb = (float)m_bottomRadius;
	float rt = (float)m_topRadius;
	float dr = rt - rb; // change in radius over the full height

	// Outward side-surface normal: (h*cosθ, -dr, h*sinθ) / slant_len
	float slant = sqrtf(h * h + dr * dr);
	float nr    = (slant > 1e-6f) ? h  / slant : 1.0f; // radial scale
	float ny    = (slant > 1e-6f) ? -dr / slant : 0.0f; // y component

	// ---- Side surface ----
	for (int stack = 0; stack <= m_stacks; ++stack) {
		float t = (float)stack / m_stacks;
		float r = rb + (rt - rb) * t;
		float y = -h * 0.5f + h * t;
		for (int slice = 0; slice <= m_slices; ++slice) {
			float theta = 2.0f * (float)PI * slice / m_slices;
			float cosT  = cosf(theta);
			float sinT  = sinf(theta);
			GpuVertex v;
			v.position[0] = r * cosT;  v.position[1] = y;  v.position[2] = r * sinT;
			v.normal[0]   = nr * cosT; v.normal[1]   = ny; v.normal[2]   = nr * sinT;
			v.texcoord[0] = (float)slice / m_slices;
			v.texcoord[1] = t;
			v.color[0] = v.color[1] = v.color[2] = v.color[3] = 1.0f;
			verts.push_back(v);
		}
	}
	for (int stack = 0; stack < m_stacks; ++stack) {
		for (int slice = 0; slice < m_slices; ++slice) {
			unsigned int bl = stack       * (m_slices + 1) + slice;
			unsigned int tl = (stack + 1) * (m_slices + 1) + slice;
			unsigned int tr = (stack + 1) * (m_slices + 1) + (slice + 1);
			unsigned int br = stack       * (m_slices + 1) + (slice + 1);
			indices.push_back(bl); indices.push_back(tl); indices.push_back(tr);
			indices.push_back(bl); indices.push_back(tr); indices.push_back(br);
		}
	}

	// ---- Bottom cap ----
	if (rb > 1e-6f) {
		unsigned int cIdx = (unsigned int)verts.size();
		GpuVertex c = {};
		c.position[1] = -h * 0.5f; c.normal[1] = -1.0f;
		c.texcoord[0] = 0.5f; c.texcoord[1] = 0.5f;
		c.color[0] = c.color[1] = c.color[2] = c.color[3] = 1.0f;
		verts.push_back(c);
		unsigned int rimBase = (unsigned int)verts.size();
		for (int slice = 0; slice <= m_slices; ++slice) {
			float theta = 2.0f * (float)PI * slice / m_slices;
			GpuVertex v = {};
			v.position[0] = rb * cosf(theta); v.position[1] = -h * 0.5f; v.position[2] = rb * sinf(theta);
			v.normal[1]   = -1.0f;
			v.texcoord[0] = 0.5f + 0.5f * cosf(theta); v.texcoord[1] = 0.5f + 0.5f * sinf(theta);
			v.color[0] = v.color[1] = v.color[2] = v.color[3] = 1.0f;
			verts.push_back(v);
		}
		for (int slice = 0; slice < m_slices; ++slice) {
			indices.push_back(cIdx);
			indices.push_back(rimBase + slice);
			indices.push_back(rimBase + slice + 1);
		}
	}

	// ---- Top cap ----
	if (rt > 1e-6f) {
		unsigned int cIdx = (unsigned int)verts.size();
		GpuVertex c = {};
		c.position[1] = h * 0.5f; c.normal[1] = 1.0f;
		c.texcoord[0] = 0.5f; c.texcoord[1] = 0.5f;
		c.color[0] = c.color[1] = c.color[2] = c.color[3] = 1.0f;
		verts.push_back(c);
		unsigned int rimBase = (unsigned int)verts.size();
		for (int slice = 0; slice <= m_slices; ++slice) {
			float theta = 2.0f * (float)PI * slice / m_slices;
			GpuVertex v = {};
			v.position[0] = rt * cosf(theta); v.position[1] = h * 0.5f; v.position[2] = rt * sinf(theta);
			v.normal[1]   = 1.0f;
			v.texcoord[0] = 0.5f + 0.5f * cosf(theta); v.texcoord[1] = 0.5f + 0.5f * sinf(theta);
			v.color[0] = v.color[1] = v.color[2] = v.color[3] = 1.0f;
			verts.push_back(v);
		}
		for (int slice = 0; slice < m_slices; ++slice) {
			indices.push_back(cIdx);
			indices.push_back(rimBase + slice + 1);
			indices.push_back(rimBase + slice);
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
void Cone::doCreateGeometry()
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
		glPushMatrix();
			glTranslated(0.0,-getHeight()/2.0,0.0);
			glRotated(-90,1.0,0.0,0.0);

			GLUquadricObj* cylinder = gluNewQuadric();
			gluQuadricNormals(cylinder,GLU_SMOOTH);
			gluQuadricTexture(cylinder,GL_TRUE);
			gluQuadricDrawStyle(cylinder,GLU_FILL);
			gluCylinder(cylinder,
				getBottomRadius(),getTopRadius(),getHeight(),
				getSlices(),getStacks());
			gluDeleteQuadric(cylinder);

			GLUquadricObj* bottom = gluNewQuadric();
			gluQuadricNormals(bottom,GLU_SMOOTH);
			gluQuadricTexture(bottom,GL_TRUE);
			gluQuadricDrawStyle(bottom,GLU_FILL);
			gluQuadricOrientation(bottom,GLU_INSIDE);
			gluDisk(bottom,
				0.0, getBottomRadius(),
				getSlices(),1);
			gluDeleteQuadric(bottom);
		glPopMatrix();

		if (getTopRadius()>0.0)
		{
			glPushMatrix();
				glTranslated(0.0,getHeight()/2.0,0.0);
				glRotated(-90,1.0,0.0,0.0);

				GLUquadricObj* top = gluNewQuadric();
				gluQuadricNormals(top,GLU_SMOOTH);
				gluQuadricTexture(top,GL_TRUE);
				gluQuadricDrawStyle(top,GLU_FILL);
				gluDisk(top,
					0.0, getTopRadius(),
					getSlices(),1);
				gluDeleteQuadric(top);
			glPopMatrix();
		}

	glPopMatrix();
}


void Cone::updateSelectBox()
{
	if (m_bottomRadius>m_topRadius)
		m_selectionBox->setSize(m_bottomRadius*2.0,m_height,m_bottomRadius*2.0);
	else
		m_selectionBox->setSize(m_topRadius*2.0,m_height,m_topRadius*2.0);
}

void Cone::doCreateSelect()
{
	m_selectionBox->render();
}

void Cone::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		double radius;
		if (m_topRadius>=m_bottomRadius)
			radius = sqrt(pow(m_height/2.0,2) + pow(m_topRadius,2));
		else
			radius = sqrt(pow(m_height/2.0,2) + pow(m_bottomRadius,2));
		getBoundingSphere()->setRadius(radius);
	}
}

// ------------------------------------------------------------
bool Cone::hasModernPath()
{
	return true;
}
