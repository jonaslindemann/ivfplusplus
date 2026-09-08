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

#include <ivf/Mesh.h>
#include <ivf/LegacyGL.h>
#include <ivf/MeshData.h>

#include <cmath>

using namespace ivf;

namespace {

// Bernstein basis of degree order-1, and its derivative, at t.
//
// This is what glMap2d evaluates internally. Writing it out is what lets the
// patch be drawn without the evaluators, which a core profile does not have.

void bernstein(int order, double t, double *b, double *db)
{
	const int degree = order - 1;

	// Binomial coefficients for the degrees in use (linear and cubic).

	double binomial[4] = {1.0, 1.0, 1.0, 1.0};

	for (int i = 0; i <= degree; i++)
	{
		double c = 1.0;

		for (int k = 0; k < i; k++)
			c = c * (double)(degree - k) / (double)(k + 1);

		binomial[i] = c;
	}

	auto basis = [&](int d, int i, double x) -> double {
		if ((i < 0) || (i > d))
			return 0.0;

		double c = 1.0;

		for (int k = 0; k < i; k++)
			c = c * (double)(d - k) / (double)(k + 1);

		return c * std::pow(x, (double)i) * std::pow(1.0 - x, (double)(d - i));
	};

	for (int i = 0; i <= degree; i++)
	{
		b[i] = binomial[i] * std::pow(t, (double)i) * std::pow(1.0 - t, (double)(degree - i));

		// dB_i^d = d * (B_{i-1}^{d-1} - B_i^{d-1})

		db[i] = (double)degree * (basis(degree - 1, i - 1, t) - basis(degree - 1, i, t));
	}
}

} // namespace

Mesh::Mesh()
{
	m_controlPoints = nullptr;
	m_initDone = false;
	m_meshDirty = true;
	m_meshRows = 20;
	m_meshCols = 20;
	m_meshType = MT_ORDER_4;
	m_meshOrientation = MO_CW;
	this->setSize(4,4);
}

Mesh::~Mesh()
{
	if (m_controlPoints!=nullptr)
		delete [] m_controlPoints;
}

void Mesh::setSize(int xSize, int ySize)
{
	m_xSize = xSize;
	m_ySize = ySize;
	this->initialize();
}

void Mesh::getSize(int &xSize, int &ySize)
{
	xSize = m_xSize;
	ySize = m_ySize;
}

void Mesh::initialize()
{
	if (m_controlPoints!=nullptr)
		delete [] m_controlPoints;

	m_controlPoints = new GLdouble[m_ySize*m_xSize*3];

	m_initDone = false;
	m_meshDirty = true;
}

void Mesh::evalPatch(double u, double v, glm::vec3 &position, glm::vec3 &normal) const
{
	const int order = (m_meshType == MT_ORDER_2) ? 2 : 4;

	double bu[4], dbu[4];
	double bv[4], dbv[4];

	bernstein(order, u, bu, dbu);
	bernstein(order, v, bv, dbv);

	glm::dvec3 p(0.0);
	glm::dvec3 du(0.0);
	glm::dvec3 dv(0.0);

	// glMap2d was given ustride 3 and vstride m_ySize*3, so the control point
	// for (i, j) lives at 3*(i + m_ySize*j). The same indexing is used here so
	// that both paths read the same patch.

	for (int i = 0; i < order; i++)
	{
		for (int j = 0; j < order; j++)
		{
			const GLdouble *c = &m_controlPoints[3 * (i + m_ySize * j)];
			const glm::dvec3 cp(c[0], c[1], c[2]);

			p  += bu[i]  * bv[j]  * cp;
			du += dbu[i] * bv[j]  * cp;
			dv += bu[i]  * dbv[j] * cp;
		}
	}

	position = glm::vec3(p);

	// GL_AUTO_NORMAL generates the analytic normal, the cross product of the
	// two partial derivatives. A degenerate patch has none, so it falls back to
	// something usable rather than to a zero vector the shader would divide by.

	glm::dvec3 n = glm::cross(du, dv);
	const double len = glm::length(n);

	if (len > 1e-12)
		normal = glm::vec3(n / len);
	else
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Mesh::updateGeometry()
{
	m_meshDirty = false;
	m_buffer.clear();

	if (m_controlPoints == nullptr)
		return;

	const int cols = (m_meshCols > 0) ? m_meshCols : 1;
	const int rows = (m_meshRows > 0) ? m_meshRows : 1;

	MeshData mesh;

	const int nu = cols + 1;
	const int nv = rows + 1;

	mesh.positions.reserve((std::size_t)nu * nv);
	mesh.normals.reserve((std::size_t)nu * nv);
	mesh.texCoords.reserve((std::size_t)nu * nv);
	mesh.colors.reserve((std::size_t)nu * nv);

	// glMapGrid2d(cols, 0, 1, rows, 0, 1) followed by glEvalMesh2 samples the
	// patch at exactly these parameter values.

	for (int jv = 0; jv < nv; jv++)
	{
		const double v = (double)jv / (double)rows;

		for (int iu = 0; iu < nu; iu++)
		{
			const double u = (double)iu / (double)cols;

			glm::vec3 position;
			glm::vec3 normal;

			this->evalPatch(u, v, position, normal);

			mesh.positions.push_back(position);
			mesh.normals.push_back(normal);
			mesh.texCoords.push_back(glm::vec2((float)u, (float)v));
			mesh.colors.push_back(glm::vec4(1.0f));
		}
	}

	mesh.indices.reserve((std::size_t)cols * rows * 2);

	// glEvalMesh2(GL_FILL, ...) emits one quad strip per row of v, stepping
	// along u and alternating between v and v+1. Splitting each of those quads
	// the same way keeps the winding the evaluator produced, which is what
	// makes the MO_CW / MO_CCW setting mean the same thing on both paths.

	for (int jv = 0; jv < rows; jv++)
	{
		for (int iu = 0; iu < cols; iu++)
		{
			const unsigned int a = (unsigned int)(jv * nu + iu);
			const unsigned int b = (unsigned int)((jv + 1) * nu + iu);
			const unsigned int c = a + 1;
			const unsigned int d = b + 1;

			mesh.indices.push_back(glm::uvec3(a, b, d));
			mesh.indices.push_back(glm::uvec3(a, d, c));
		}
	}

	m_buffer.setMesh(mesh);
}

void Mesh::doCreateGeometry()
{
	// Winding first, and for both paths -- glFrontFace is core legal.

	switch (m_meshOrientation) {
	case MO_CW:
		glFrontFace(GL_CW);
		break;
	case MO_CCW:
		glFrontFace(GL_CCW);
		break;
	}

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

		glFrontFace(GL_CCW);
		return;
	}

	lgPushAttrib(GL_LIGHTING|GL_AUTO_NORMAL|GL_NORMALIZE);
	lgEnableLegacy(GL_AUTO_NORMAL);
	lgEnableLegacy(GL_NORMALIZE);
	glEnable(GL_MAP2_VERTEX_3);
	switch (m_meshType) {
	case MT_ORDER_2:
		glMap2d(GL_MAP2_VERTEX_3,
			0, 1,  // U ranges 0..1
			3,         // U stride, 3 floats per coord
			2,         // U is 2nd order, ie. linear
			0, 1,  // V ranges 0..1
			m_ySize * 3,     // V stride, row is 2 coords, 3 floats per coord
			2,         // V is 2nd order, ie linear
			m_controlPoints);  // control points
		break;
	case MT_ORDER_4:
		glMap2d(GL_MAP2_VERTEX_3,
			0, 1,  // U ranges 0..1
			3,         // U stride, 3 floats per coord
			4,         // U is 2nd order, ie. linear
			0, 1,  // V ranges 0..1
			m_ySize * 3,     // V stride, row is 2 coords, 3 floats per coord
			4,         // V is 2nd order, ie linear
			m_controlPoints);  // control points
		break;
	default:
		break;
	}

	glMapGrid2d(
		m_meshCols, 0.0, 1.0,
		m_meshRows, 0.0, 1.0
		);

	glEvalMesh2(GL_FILL, 0, m_meshCols, 0, m_meshRows);
	glFrontFace(GL_CCW);
	lgPopAttrib();
}

bool Mesh::hasModernPath()
{
	return true;
}

void Mesh::setControlPoint(int i, int j, double x, double y, double z)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		m_controlPoints[3*(j+m_ySize*i)+0] = x;
		m_controlPoints[3*(j+m_ySize*i)+1] = y;
		m_controlPoints[3*(j+m_ySize*i)+2] = z;
		m_initDone = false;
		m_meshDirty = true;
	}
}

void Mesh::getControlPoint(int i, int j, double &x, double &y, double &z)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		x = m_controlPoints[3*(i+m_ySize*j)+0];
		y = m_controlPoints[3*(i+m_ySize*j)+1];
		z = m_controlPoints[3*(i+m_ySize*j)+2];
		m_initDone = false;
	}
}

void Mesh::createMesh(double width, double height)
{
	int i, j;

	for (i=0; i<m_ySize; i++)
		for (j=0; j<m_xSize; j++)
		{
			m_controlPoints[3*(i+m_ySize*j)+0] = width/2.0 - (double)j*width/(double)(m_xSize-1);
			m_controlPoints[3*(i+m_ySize*j)+1] = 0.0;
			m_controlPoints[3*(i+m_ySize*j)+2] = height/2.0 - (double)i*height/(double)(m_ySize-1);
			m_initDone = false;
			m_meshDirty = true;
		}
}

void Mesh::setControlPoint(int i, int j, double y)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		m_controlPoints[3*(i+m_ySize*j)+1] = y;
		m_initDone = false;
		m_meshDirty = true;
	}
}

void Mesh::setMeshType(TMeshType meshType)
{
	m_meshType = meshType;

	switch (m_meshType) {
	case MT_ORDER_2:
		m_xSize = 2;
		m_ySize = 2;
		initialize();
		break;
	case MT_ORDER_4:
		m_xSize = 4;
		m_ySize = 4;
		initialize();
		break;
	default:

		break;
	}
}

void Mesh::setMeshResolution(int rows, int cols)
{
	m_meshRows = rows;
	m_meshCols = cols;
	m_meshDirty = true;
}

void Mesh::setMeshOrientation(TMeshOrientation meshOrientation)
{
	m_meshOrientation = meshOrientation;
}
