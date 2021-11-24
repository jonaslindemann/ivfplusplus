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

// Implementation of: public class Shape

#include <ivf/config.h>
#include <ivf/Shape.h>
#include <ivf/GlobalState.h>

using namespace ivf;

// ------------------------------------------------------------
Shape::Shape ()
		:GLBase()
		//TODO: check and complete member initialisation list!
{
	m_position[0] = 0.0;
	m_position[1] = 0.0;
	m_position[2] = 0.0;
	m_scale[0] = 1.0;
	m_scale[1] = 1.0;
	m_scale[2] = 1.0;
	m_rotation[0] = 0.0;
	m_rotation[1] = 0.0;
	m_rotation[2] = 0.0;
	m_rotQuat[0] = 0.0;
	m_rotQuat[1] = 1.0;
	m_rotQuat[2] = 0.0;
	m_rotQuat[3] = 0.0;
	m_material = nullptr;
	m_texture = nullptr;
	m_highlightMaterial = new Material();
	m_highlightMaterial->setDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_highlightMaterial->setAmbientColor(0.5, 0.5, 0.5, 1.0);

	m_selectMaterial = new Material();
	m_selectMaterial->setDiffuseColor(1.0, 0.8, 0.0, 1.0);
	m_selectMaterial->setAmbientColor(0.6, 0.6, 0.0, 1.0);

	m_objectName = IVF_NONAME;
	m_renderName = true;
	m_highlight = HS_OFF;
	m_highlightType = HT_MATERIAL;
	m_normalize = false;
}

// ------------------------------------------------------------
Shape::~Shape ()
{
}

// ------------------------------------------------------------
void Shape::setPosition (const double x, const double y, const double z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void Shape::setPosition (Shape* shape)
{
	double x, y, z;
	shape->getPosition(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void Shape::setPosition (Point3d* point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void Shape::setScale (const double xScale, const double yScale, const double zScale)
{
	m_scale[0] = xScale;
	m_scale[1] = yScale;
	m_scale[2] = zScale;
}

// ------------------------------------------------------------
void Shape::setRotation (const double xRot, const double yRot, const double zRot)
{
	m_rotation[0] = xRot;
	m_rotation[1] = yRot;
	m_rotation[2] = zRot;
}

// ------------------------------------------------------------
void Shape::setMaterial (Material* material)
{
	m_material = material;
}

// ------------------------------------------------------------
void Shape::getPosition (double &x, double &y, double &z)
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
}

// ------------------------------------------------------------
void Shape::getPosition (Shape* shape)
{
	shape->setPosition(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void Shape::getPosition (Point3d* point)
{
	point->setComponents(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void Shape::getScale (double &xScale, double &yScale, double &zScale)
{
	xScale = m_scale[0];
	yScale = m_scale[1];
	zScale = m_scale[2];
}

// ------------------------------------------------------------
void Shape::getRotation (double &xRot, double &yRot, double &zRot)
{
	xRot = m_rotation[0];
	yRot = m_rotation[1];
	zRot = m_rotation[2];
}

// ------------------------------------------------------------
Material* Shape::getMaterial ()
{
	return m_material;
}


void Shape::doCreateMaterial()
{
	if (GlobalState::getInstance()->isMaterialRenderingEnabled())
	{
		if (this->getSelect()==SS_ON)
		{
			if (m_selectMaterial!=nullptr)
				m_selectMaterial->render();
			else
				if (m_material!=nullptr)
					m_material->render();
		}
		else
		{
			if (m_highlight == HS_ON)
			{
				if (m_highlightType==HT_MATERIAL)
				{
					if (m_highlightMaterial!=nullptr)
						m_highlightMaterial->render();
					else
						if (m_material!=nullptr)
							m_material->render();
				}
			}
			else
				if (m_material!=nullptr)
					m_material->render();
		}
	}
}

void Shape::doBeginTransform()
{
	if (m_renderName)
		glLoadName(m_objectName);

	glPushMatrix();

	if (!((m_position[0]==0.0)&&(m_position[1]==0.0)&&(m_position[2]==0.0)))
		glTranslated(m_position[0],m_position[1],m_position[2]);

	if (m_rotation[0]!=0.0)
		glRotated(m_rotation[0], 1.0, 0.0, 0.0);

	if (m_rotation[1]!=0.0)
		glRotated(m_rotation[1], 0.0, 1.0, 0.0);

	if (m_rotation[2]!=0.0)
		glRotated(m_rotation[2], 0.0, 0.0, 1.0);

	if (m_rotQuat[3]!=0.0)
		glRotated(m_rotQuat[3], m_rotQuat[0], m_rotQuat[1], m_rotQuat[2]);

	if (m_normalize)
		glEnable(GL_NORMALIZE);

	if (!((m_scale[0]==0.0)&&(m_scale[1]==0.0)&&(m_scale[2]==0.0)))
		glScaled(m_scale[0],m_scale[1],m_scale[2]);

	if (m_texture!=nullptr)
	{
		if (GlobalState::getInstance()->isTextureRenderingEnabled())
		{
			if (m_texture->isActive())
			{
				glPushAttrib(GL_TEXTURE_2D|GL_TEXTURE_1D);
				glEnable(GL_TEXTURE_1D);
				glEnable(GL_TEXTURE_2D);
				if (!m_texture->isBound())
					m_texture->bind();
				else
					m_texture->apply();
			}
		}
	}
}

void Shape::doEndTransform()
{
	if (m_texture!=nullptr)
	{
		if (GlobalState::getInstance()->isTextureRenderingEnabled())
		{
			if (m_texture->isActive())
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_TEXTURE_1D);
				glPopAttrib();
			}
		}
	}
	glPopMatrix();

	if (m_normalize)
		glDisable(GL_NORMALIZE);
}

void Shape::setRotationQuat(double vx, double vy, double vz, double theta)
{
	m_rotQuat[0] = vx;
	m_rotQuat[1] = vy;
 	m_rotQuat[2] = vz;
	m_rotQuat[3] = theta;
}

// Modified JL
void Shape::getRotationQuat(double &vx, double &vy, double &vz, double &theta)
{
	vx = m_rotQuat[0];
	vy = m_rotQuat[1];
	vz = m_rotQuat[2];
	theta = m_rotQuat[3];
}
// End Modify JL

void Shape::assignPositionTo(Shape * shape)
{
	shape->getPosition(m_position[0], m_position[1], m_position[2]);
}

void Shape::setObjectName(GLuint name)
{
	m_objectName = name;
}

GLuint Shape::getObjectName()
{
	return m_objectName;
}

void Shape::setHighlightMaterial(Material * material)
{
	m_highlightMaterial = material;
}

Material* Shape::getHightlightMaterial()
{
	return m_highlightMaterial;
}

void Shape::setHighlight(THighlightState state)
{
	m_highlight = state;
}

void Shape::assignPointTo(Point3d* point)
{
	point->setComponents(m_position[0], m_position[1], m_position[2]);
}

void Shape::doCreateGeometry()
{
}

void Shape::doCreateSelect()
{
}

void Shape::setUseName(bool flag)
{
	m_renderName = flag;
}

bool Shape::getUseName()
{
	return m_renderName;
}

Shape::THighlightState Shape::getHighlight()
{
	return m_highlight;
}

void Shape::refresh()
{

}

void Shape::setTexture(Texture *texture)
{
	m_texture = texture;
}

Texture* Shape::getTexture()
{
	return m_texture;
}

void Shape::setNormalize(bool flag)
{
	m_normalize = flag;
}

void Shape::setSelectMaterial(Material *material)
{
	m_selectMaterial = material;
}

void Shape::setPosition(Vec3d &pos)
{
	pos.getComponents(m_position);
}

void Shape::setPosition(Point3d &pos)
{
	pos.getComponents(m_position);
}

Vec3d& Shape::getPosition()
{
	Vec3d& pos = ivfGetTempVec3d();
	pos.setComponents(m_position);
	return pos;
}

void Shape::setRotation(Quat &q)
{
	double vx, vy, vz, angle;
	q.getAxisAngle(vx, vy, vz, angle);
	this->setRotationQuat(vx, vy, vz, angle*360.0/2.0/M_PI);
}

void Shape::setHighlightType(THighlightType type)
{
	m_highlightType = type;
}

void Shape::enableHighlight()
{
	m_highlight = HS_ON;
}

void Shape::disableHighlight()
{
	m_highlight = HS_OFF;
}

bool Shape::isEnabledHighlight()
{
	return (m_highlight==HS_ON);
}
