//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//

// Implementation of: public class CIvfShape

#include <ivf/ivfconfig.h>
#include <ivf/IvfShape.h>

#include <ivf/IvfGlobalState.h>

// ------------------------------------------------------------
CIvfShape::CIvfShape ()
		:CIvfGLBase()
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
	m_material = NULL;
	m_texture = NULL;
	m_highlightMaterial = new CIvfMaterial();
	m_highlightMaterial->setDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_highlightMaterial->setAmbientColor(0.5, 0.5, 0.5, 1.0);

	m_selectMaterial = new CIvfMaterial();
	m_selectMaterial->setDiffuseColor(1.0, 0.8, 0.0, 1.0);
	m_selectMaterial->setAmbientColor(0.6, 0.6, 0.0, 1.0);

	m_objectName = IVF_NONAME;
	m_renderName = true;
	m_highlight = HS_OFF;
	m_highlightType = HT_MATERIAL;
	m_normalize = false;
}

// ------------------------------------------------------------
CIvfShape::~CIvfShape ()
{
}

// ------------------------------------------------------------
void CIvfShape::setPosition (const double x, const double y, const double z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CIvfShape::setPosition (CIvfShape* shape)
{
	double x, y, z;
	shape->getPosition(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CIvfShape::setPosition (CIvfPoint3d* point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CIvfShape::setScale (const double xScale, const double yScale, const double zScale)
{
	m_scale[0] = xScale;
	m_scale[1] = yScale;
	m_scale[2] = zScale;
}

// ------------------------------------------------------------
void CIvfShape::setRotation (const double xRot, const double yRot, const double zRot)
{
	m_rotation[0] = xRot;
	m_rotation[1] = yRot;
	m_rotation[2] = zRot;
}

// ------------------------------------------------------------
void CIvfShape::setMaterial (CIvfMaterial* material)
{
	m_material = material;
}

// ------------------------------------------------------------
void CIvfShape::getPosition (double &x, double &y, double &z)
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
}

// ------------------------------------------------------------
void CIvfShape::getPosition (CIvfShape* shape)
{
	shape->setPosition(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void CIvfShape::getPosition (CIvfPoint3d* point)
{
	point->setComponents(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void CIvfShape::getScale (double &xScale, double &yScale, double &zScale)
{
	xScale = m_scale[0];
	yScale = m_scale[1];
	zScale = m_scale[2];
}

// ------------------------------------------------------------
void CIvfShape::getRotation (double &xRot, double &yRot, double &zRot)
{
	xRot = m_rotation[0];
	yRot = m_rotation[1];
	zRot = m_rotation[2];
}

// ------------------------------------------------------------
CIvfMaterial* CIvfShape::getMaterial ()
{
	return m_material;
}


void CIvfShape::doCreateMaterial()
{
	if (CIvfGlobalState::getInstance()->isMaterialRenderingEnabled())
	{
		if (this->getSelect()==SS_ON)
		{
			if (m_selectMaterial!=NULL)
				m_selectMaterial->render();
			else
				if (m_material!=NULL)
					m_material->render();
		}
		else
		{
			if (m_highlight == HS_ON)
			{
				if (m_highlightType==HT_MATERIAL)
				{
					if (m_highlightMaterial!=NULL)
						m_highlightMaterial->render();
					else
						if (m_material!=NULL)
							m_material->render();
				}
			}
			else
				if (m_material!=NULL)
					m_material->render();
		}
	}
}

void CIvfShape::doBeginTransform()
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

	if (m_texture!=NULL)
	{
		if (CIvfGlobalState::getInstance()->isTextureRenderingEnabled())
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

void CIvfShape::doEndTransform()
{
	if (m_texture!=NULL)
	{
		if (CIvfGlobalState::getInstance()->isTextureRenderingEnabled())
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

void CIvfShape::setRotationQuat(double vx, double vy, double vz, double theta)
{
	m_rotQuat[0] = vx;
	m_rotQuat[1] = vy;
	m_rotQuat[2] = vz;
	m_rotQuat[3] = theta;
}

// Modified JL
void CIvfShape::getRotationQuat(double &vx, double &vy, double &vz, double &theta)
{
	vx = m_rotQuat[0];
	vy = m_rotQuat[1];
	vz = m_rotQuat[2];
	theta = m_rotQuat[3];
}
// End Modify JL

void CIvfShape::assignPositionTo(CIvfShape * shape)
{
	shape->getPosition(m_position[0], m_position[1], m_position[2]);
}

void CIvfShape::setObjectName(GLuint name)
{
	m_objectName = name;
}

GLuint CIvfShape::getObjectName()
{
	return m_objectName;
}

void CIvfShape::setHighlightMaterial(CIvfMaterial * material)
{
	m_highlightMaterial = material;
}

CIvfMaterial* CIvfShape::getHightlightMaterial()
{
	return m_highlightMaterial;
}

void CIvfShape::setHighlight(THighlightState state)
{
	m_highlight = state;
}

void CIvfShape::assignPointTo(CIvfPoint3d* point)
{
	point->setComponents(m_position[0], m_position[1], m_position[2]);
}

void CIvfShape::doCreateGeometry()
{
}

void CIvfShape::doCreateSelect()
{
}

void CIvfShape::setUseName(bool flag)
{
	m_renderName = flag;
}

bool CIvfShape::getUseName()
{
	return m_renderName;
}

CIvfShape::THighlightState CIvfShape::getHighlight()
{
	return m_highlight;
}

void CIvfShape::refresh()
{

}

void CIvfShape::setTexture(CIvfTexture *texture)
{
	m_texture = texture;
}

CIvfTexture* CIvfShape::getTexture()
{
	return m_texture;
}

void CIvfShape::setNormalize(bool flag)
{
	m_normalize = flag;
}

void CIvfShape::setSelectMaterial(CIvfMaterial *material)
{
	m_selectMaterial = material;
}

void CIvfShape::setPosition(CIvfVec3d &pos)
{
	pos.getComponents(m_position);
}

void CIvfShape::setPosition(CIvfPoint3d &pos)
{
	pos.getComponents(m_position);
}

CIvfVec3d& CIvfShape::getPosition()
{
	CIvfVec3d& pos = ivfGetTempVec3d();
	pos.setComponents(m_position);
	return pos;
}

void CIvfShape::setRotation(CIvfQuat &q)
{
	double vx, vy, vz, angle;
	q.getAxisAngle(vx, vy, vz, angle);
	this->setRotationQuat(vx, vy, vz, angle*360.0/2.0/M_PI);
}

void CIvfShape::setHighlightType(THighlightType type)
{
	m_highlightType = type;
}

void CIvfShape::enableHighlight()
{
	m_highlight = HS_ON;
}

void CIvfShape::disableHighlight()
{
	m_highlight = HS_OFF;
}

bool CIvfShape::isEnabledHighlight()
{
	return (m_highlight==HS_ON);
}
