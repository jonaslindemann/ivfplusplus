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

#include <ivf/config.h>
#include <ivf/Billboard.h>

using namespace ivf;

CBillBoard::CBillBoard()
{
	m_camera = nullptr;
	m_alignObject = IVF_ALIGN_CAMERA;
	this->setBillboardType(IVF_BILLBOARD_Y);
}

CBillBoard::~CBillBoard()
{

}

void CBillBoard::updateRotation()
{
	if (m_camera!=nullptr)
	{
		double ex, ey, ez, e;

		switch (m_alignObject) {
		case IVF_ALIGN_CAMERA:
			m_camera->getForwardVector(ex, ey, ez);
			break;
		case IVF_ALIGN_VECTOR:
			ex = m_forward[0];
			ey = m_forward[1];
			ez = m_forward[2];
			break;
		default:
			m_camera->getForwardVector(ex, ey, ez);
			break;
		}

		ex = -ex;
		ey = -ey;
		ez = -ez;

		e = sqrt(pow(ex,2) + pow(ez,2));

		m_angle1 = atan2(ez,ex);
		m_angle2 = atan2(e,ey);

		switch (m_billboardType) {
		case IVF_BILLBOARD_X:
			m_angle1 = M_PI/2;
			break;
		case IVF_BILLBOARD_Y:
			m_angle2 = M_PI/2;
			break;
		default:
			break;
		}

		//this->setRotationQuat(1.0, 0.0, 0.0, m_angle2*180.0/M_PI-90);
	}
}

void CBillBoard::setCamera(CCamera *camera)
{
	m_camera = camera;
}

void CBillBoard::setBillboardType(int type)
{
	m_billboardType = type;
}

int CBillBoard::getBillboardType()
{
	return m_billboardType;
}

void CBillBoard::doCreateGeometry()
{
	this->updateRotation();
	glPushMatrix();
	glRotated(-m_angle1*180.0/M_PI+90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glRotated(m_angle2*180.0/M_PI-90, 1.0, 0.0, 0.0);
	CComposite::doCreateGeometry();
	glPopMatrix();
	glPopMatrix();
}


void CBillBoard::setVector(double vx, double vy, double vz)
{
	m_forward[0] = vx;
	m_forward[1] = vy;
	m_forward[2] = vz;
}

void CBillBoard::setVector(double *v)
{
	m_forward[0] = v[0];
	m_forward[1] = v[1];
	m_forward[2] = v[2];
}

void CBillBoard::setAlignObject(int objectType)
{
	m_alignObject = objectType;
}
