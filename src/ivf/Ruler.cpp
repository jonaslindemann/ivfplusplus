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

#include <ivf/Ruler.h>

#include <ivf/Lighting.h>
#include <ivf/rc.h>

#include <vector>

using namespace ivf;

Ruler::Ruler()
{
	m_startPoint.setComponents(0.0, 0.0, 0.0);
	m_endPoint.setComponents(0.0, 1.0, 0.0);
	m_tickStep = 0.2;
	initRuler();
}

Ruler::~Ruler()
{

}

void Ruler::setStartPoint(Vec3d &startPoint)
{
	m_startPoint = startPoint;
	initRuler();
}

void Ruler::setEndPoint(Vec3d &endPoint)
{
	m_endPoint = endPoint;
	initRuler();
}

void Ruler::initRuler()
{
	m_direction = m_endPoint - m_startPoint;
	m_length = m_direction.length();
	m_direction.normalize();
}

void Ruler::doCreateGeometry()
{
	int i, nSteps;
	double x, y, z;
	Vec3d p;
	bool oldLightingState = Lighting::getInstance()->isEnabled();

	nSteps = (int)(m_length/m_tickStep);

	glLineWidth(1.0);

	if (rcIsShaderActive())
	{
		std::vector<float> positions;
		std::vector<float> colors;

		auto addVertex = [&](double vx, double vy, double vz, float r, float g, float b) {
			positions.push_back((float)vx);
			positions.push_back((float)vy);
			positions.push_back((float)vz);
			colors.push_back(r);
			colors.push_back(g);
			colors.push_back(b);
			colors.push_back(1.0f);
		};

		const double* s = m_startPoint.getComponents();
		const double* e = m_endPoint.getComponents();
		addVertex(s[0], s[1], s[2], 0.3f, 0.3f, 0.3f);
		addVertex(e[0], e[1], e[2], 0.3f, 0.3f, 0.3f);

		for (i=0; i<nSteps; i++)
		{
			p = m_startPoint + i*m_tickStep*m_direction;
			p.getComponents(x, y, z);

			addVertex(x - m_tickStep*0.1, y, z, 0.6f, 0.6f, 0.6f);
			addVertex(x + m_tickStep*0.1, y, z, 0.6f, 0.6f, 0.6f);
			addVertex(x, y, z - m_tickStep*0.1, 0.6f, 0.6f, 0.6f);
			addVertex(x, y, z + m_tickStep*0.1, 0.6f, 0.6f, 0.6f);
		}

		if (rcDrawUnlit(GL_LINES, positions.data(), colors.data(), (int)(positions.size() / 3)))
			return;
	}

	Lighting::getInstance()->disable();
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBegin(GL_LINES);
	glVertex3dv(m_startPoint.getComponents());
	glVertex3dv(m_endPoint.getComponents());
	glEnd();

	glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
	glLineWidth(1.0);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
	for (i=0; i<nSteps; i++)
	{
		p = m_startPoint + i*m_tickStep*m_direction;
		p.getComponents(x, y, z);

		glVertex3d(x - m_tickStep*0.1, y, z);
		glVertex3d(x + m_tickStep*0.1 , y, z);
		glVertex3d(x, y, z - m_tickStep*0.1);
		glVertex3d(x, y, z + m_tickStep*0.1);
		//glVertex3d(x, y - m_tickStep*0.1, z);
		//glVertex3d(x, y + m_tickStep*0.1, z);
	}
	glEnd();
	glEnable(GL_DEPTH_TEST);

	// Restore the default width. Zero is not a legal line width -- it raises
	// GL_INVALID_VALUE -- so the intended "put it back" value is 1.0.

	glLineWidth(1.0);
	if (oldLightingState)
		Lighting::getInstance()->enable();
	else
		Lighting::getInstance()->disable();
}

void Ruler::setStartPoint(double x, double y, double z)
{
	m_startPoint.setComponents(x, y, z);
	initRuler();
}

void Ruler::setEndPoint(double x, double y, double z)
{
	m_endPoint.setComponents(x, y, z);
	initRuler();
}

void Ruler::setTickStep(double tickStep)
{
	m_tickStep = tickStep;
}
