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

#include <ivf/ExtrArrow.h>

#include <ivf/config.h>
#include <ivf/rc.h>
#include <ivf/MeshData.h>

#include <ivfmath/BoundingSphere.h>
#include <ivfmath/Vec3d.h>

#include <GL/gle.h>

#include <vector>

using namespace ivf;

// ------------------------------------------------------------
ExtrArrow::ExtrArrow()
:Shape()
{
	m_direction[0] = 1.0;
	m_direction[1] = 0.0;
	m_direction[2] = 0.0;
	m_length = 1.0;
	m_head = 0.2 * m_length;
	m_headRadius = 0.03*m_length;
	m_tailRadius = 0.02*m_length;
	m_offset = 0.0;
}

// ------------------------------------------------------------
ExtrArrow::~ExtrArrow()
{

}

// ------------------------------------------------------------
void ExtrArrow::updateGeometry()
{
	m_meshDirty = false;
	m_buffer.clear();

	{
		Vec3d axis(m_direction[0], m_direction[1], m_direction[2]);
		axis.normalize();

		Vec3d up(0.0, 1.0, 0.0);
		double ax, ay, az;
		double ux, uy, uz;
		axis.getComponents(ax, ay, az);
		up.getComponents(ux, uy, uz);
		if (fabs(ax * ux + ay * uy + az * uz) > 0.95)
			up.setComponents(1.0, 0.0, 0.0);

		Vec3d side = axis * up;
		side.normalize();
		Vec3d binormal = side * axis;
		binormal.normalize();

		const int sides = 24;
		std::vector<float> positions;

		auto ringPoint = [&](int ring, int sideIdx) {
			double angle = 2.0 * M_PI * (double)sideIdx / (double)sides;
			double c = cos(angle);
			double s = sin(angle);
			Vec3d p(m_coords[ring][0], m_coords[ring][1], m_coords[ring][2]);
			p = p + side * (m_radius[ring] * c) + binormal * (m_radius[ring] * s);
			return p;
		};

		auto addPoint = [&](Vec3d& p) {
			double x, y, z;
			p.getComponents(x, y, z);
			positions.push_back((float)x);
			positions.push_back((float)y);
			positions.push_back((float)z);
		};

		// glePolyCone() treats the first and last points as phantom endpoints:
		// they only set the direction the end joins are cut at, and no surface is
		// generated between points 0-1 or 4-5. initArrow() places them a unit
		// before the tail and a unit past the tip precisely for that. Sweeping
		// all five gaps drew those two phantom segments as real geometry, which
		// is why the arrow came out longer than the fixed-function one.

		MeshData mesh;

		for (int ring = 1; ring < 4; ++ring)
		{
			// Analytic normal for a truncated cone. Parametrise the surface as
			// P(t) + R(t) u; the cross product of the two partials comes out as
			// L u - dR a, which is radial for a cylinder, tilted for a cone, and
			// axial for the flat annulus at the head where L is zero.

			Vec3d p0(m_coords[ring][0], m_coords[ring][1], m_coords[ring][2]);
			Vec3d p1(m_coords[ring + 1][0], m_coords[ring + 1][1], m_coords[ring + 1][2]);

			Vec3d segment = p1 - p0;
			double length = segment.length();

			Vec3d segAxis = axis;

			if (length > 1e-12)
			{
				segAxis = segment;
				segAxis.normalize();
			}

			const double dR = m_radius[ring + 1] - m_radius[ring];

			const unsigned int base = (unsigned int)mesh.positions.size();

			for (int sideIdx = 0; sideIdx < sides; ++sideIdx)
			{
				double angle = 2.0 * M_PI * (double)sideIdx / (double)sides;
				double c = cos(angle);
				double s = sin(angle);

				Vec3d radial = side * c + binormal * s;

				Vec3d n = radial * length - segAxis * dR;

				if (n.length() > 1e-12)
					n.normalize();
				else
					n = radial;

				double nx, ny, nz;
				n.getComponents(nx, ny, nz);

				Vec3d a0 = ringPoint(ring, sideIdx);
				Vec3d a1 = ringPoint(ring + 1, sideIdx);

				double x, y, z;

				a0.getComponents(x, y, z);
				mesh.positions.push_back(glm::vec3((float)x, (float)y, (float)z));
				mesh.normals.push_back(glm::vec3((float)nx, (float)ny, (float)nz));
				mesh.texCoords.push_back(glm::vec2((float)sideIdx / (float)sides, 0.0f));
				mesh.colors.push_back(glm::vec4(1.0f));

				a1.getComponents(x, y, z);
				mesh.positions.push_back(glm::vec3((float)x, (float)y, (float)z));
				mesh.normals.push_back(glm::vec3((float)nx, (float)ny, (float)nz));
				mesh.texCoords.push_back(glm::vec2((float)sideIdx / (float)sides, 1.0f));
				mesh.colors.push_back(glm::vec4(1.0f));
			}

			// Same winding the immediate mode version used.

			for (int sideIdx = 0; sideIdx < sides; ++sideIdx)
			{
				unsigned int cur = base + 2u * (unsigned int)sideIdx;
				unsigned int nxt = base + 2u * (unsigned int)((sideIdx + 1) % sides);

				mesh.indices.push_back(glm::uvec3(cur, cur + 1u, nxt + 1u));
				mesh.indices.push_back(glm::uvec3(cur, nxt + 1u, nxt));
			}
		}

		m_buffer.setMesh(mesh);
	}
}

// ------------------------------------------------------------
void ExtrArrow::doCreateGeometry()
{
	if (rcIsShaderActive())
	{
		if (m_meshDirty)
			this->updateGeometry();

		if (!m_buffer.isEmpty())
		{
			rcUseShader();
			rcUpdateShader();
			m_buffer.drawShader(GL_TRIANGLES);
		}

		return;
	}

	glePolyCone(6,m_coords,nullptr,m_radius);
}

// ------------------------------------------------------------
void ExtrArrow::setDirection(double ex, double ey, double ez)
{
	double l;
	l = sqrt(pow(ex,2) + pow(ey,2) + pow(ez,2));
	m_direction[0] = ex/l;
	m_direction[1] = ey/l;
	m_direction[2] = ez/l;
	this->initExtrusion();
}

// ------------------------------------------------------------
void ExtrArrow::getDirection(double &ex, double &ey, double &ez)
{
	ex = m_direction[0];
	ey = m_direction[1];
	ez = m_direction[2];
}

// ------------------------------------------------------------
void ExtrArrow::initExtrusion()
{
	m_meshDirty = true;

	m_coords[0][0] = (m_offset-1.0)*m_direction[0];
	m_coords[0][1] = (m_offset-1.0)*m_direction[1];
	m_coords[0][2] = (m_offset-1.0)*m_direction[2];
	m_radius[0] = 0.0;

	m_coords[1][0] = m_offset*m_direction[0];
	m_coords[1][1] = m_offset*m_direction[1];
	m_coords[1][2] = m_offset*m_direction[2];
	m_radius[1] = m_tailRadius;

	m_coords[2][0] = (m_length-m_head+m_offset)*m_direction[0];
	m_coords[2][1] = (m_length-m_head+m_offset)*m_direction[1];
	m_coords[2][2] = (m_length-m_head+m_offset)*m_direction[2];
	m_radius[2] = m_tailRadius;

	m_coords[3][0] = (m_length-m_head+m_offset)*m_direction[0];
	m_coords[3][1] = (m_length-m_head+m_offset)*m_direction[1];
	m_coords[3][2] = (m_length-m_head+m_offset)*m_direction[2];
	m_radius[3] = m_headRadius;

	m_coords[4][0] = (m_length+m_offset)*m_direction[0];
	m_coords[4][1] = (m_length+m_offset)*m_direction[1];
	m_coords[4][2] = (m_length+m_offset)*m_direction[2];
	m_radius[4] = 0.0;

	m_coords[5][0] = (m_length+m_offset+1)*m_direction[0];
	m_coords[5][1] = (m_length+m_offset+1)*m_direction[1];
	m_coords[5][2] = - (m_length+m_offset+1)*m_direction[2];
	m_radius[5] = 0.0;
}

// ------------------------------------------------------------
void ExtrArrow::setSize(double length, double head)
{
	m_length = length;
	m_head = head;
	this->initExtrusion();
}

// ------------------------------------------------------------
void ExtrArrow::setRadius(double head, double tail)
{
	m_headRadius = head;
	m_tailRadius = tail;
	this->initExtrusion();
}

// ------------------------------------------------------------
void ExtrArrow::getSize(double &length, double &head)
{
	length = m_length;
	head = m_head;
}

// ------------------------------------------------------------
void ExtrArrow::getRadius(double &head, double &tail)
{
	head = m_headRadius;
	tail = m_tailRadius;
}

// ------------------------------------------------------------
void ExtrArrow::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		BoundingSphere* bSphere = getBoundingSphere();
		bSphere->setRadius(m_length/2.0);
	}
}

void ExtrArrow::setOffset(double offset)
{
	m_offset = offset;
	initExtrusion();
}

double ExtrArrow::getOffset()
{
	return m_offset;
}

void ExtrArrow::setDirection(Vec3d &vec)
{
	double vx, vy, vz;
	vec.getComponents(vx, vy, vz);
	this->setDirection(vx, vy, vz);
}

// ------------------------------------------------------------
bool ExtrArrow::hasModernPath()
{
	return true;
}
