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

// Implementation of: public class CShape

#include <ivf/ivfconfig.h>
#include <ivf/IvfShape.h>

#include <ivf/IvfGlobalState.h>

// ------------------------------------------------------------
CShape::CShape ()
		:CGLBase()
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
	m_highlightMaterial = new CMaterial();
	m_highlightMaterial->setDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_highlightMaterial->setAmbientColor(0.5, 0.5, 0.5, 1.0);

	m_selectMaterial = new CMaterial();
	m_selectMaterial->setDiffuseColor(1.0, 0.8, 0.0, 1.0);
	m_selectMaterial->setAmbientColor(0.6, 0.6, 0.0, 1.0);

	m_objectName = IVF_NONAME;
	m_renderName = true;
	m_highlight = HS_OFF;
	m_highlightType = HT_MATERIAL;
	m_normalize = false;
}

// ------------------------------------------------------------
CShape::~CShape ()
{
}

// ------------------------------------------------------------
void CShape::setPosition (const double x, const double y, const double z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CShape::setPosition (CShape* shape)
{
	double x, y, z;
	shape->getPosition(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CShape::setPosition (CPoint3d* point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

// ------------------------------------------------------------
void CShape::setScale (const double xScale, const double yScale, const double zScale)
{
	m_scale[0] = xScale;
	m_scale[1] = yScale;
	m_scale[2] = zScale;
}

// ------------------------------------------------------------
void CShape::setRotation (const double xRot, const double yRot, const double zRot)
{
	m_rotation[0] = xRot;
	m_rotation[1] = yRot;
	m_rotation[2] = zRot;
}

// ------------------------------------------------------------
void CShape::setMaterial (CMaterial* material)
{
	m_material = material;
}

// ------------------------------------------------------------
void CShape::getPosition (double &x, double &y, double &z)
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
}

// ------------------------------------------------------------
void CShape::getPosition (CShape* shape)
{
	shape->setPosition(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void CShape::getPosition (CPoint3d* point)
{
	point->setComponents(m_position[0],m_position[1],m_position[2]);
}

// ------------------------------------------------------------
void CShape::getScale (double &xScale, double &yScale, double &zScale)
{
	xScale = m_scale[0];
	yScale = m_scale[1];
	zScale = m_scale[2];
}

// ------------------------------------------------------------
void CShape::getRotation (double &xRot, double &yRot, double &zRot)
{
	xRot = m_rotation[0];
	yRot = m_rotation[1];
	zRot = m_rotation[2];
}

// ------------------------------------------------------------
CMaterial* CShape::getMaterial ()
{
	return m_material;
}


void CShape::doCreateMaterial()
{
	if (CGlobalState::getInstance()->isMaterialRenderingEnabled())
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

void CShape::doBeginTransform()
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
		if (CGlobalState::getInstance()->isTextureRenderingEnabled())
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

void CShape::doEndTransform()
{
	if (m_texture!=NULL)
	{
		if (CGlobalState::getInstance()->isTextureRenderingEnabled())
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

void CShape::setRotationQuat(double vx, double vy, double vz, double theta)
{
	m_rotQuat[0] = vx;
	m_rotQuat[1] = vy;
 	m_rotQuat[2] = vz;
	m_rotQuat[3] = theta;
}

// Modified JL
void CShape::getRotationQuat(double &vx, double &vy, double &vz, double &theta)
{
	vx = m_rotQuat[0];
	vy = m_rotQuat[1];
	vz = m_rotQuat[2];
	theta = m_rotQuat[3];
}
// End Modify JL

void CShape::assignPositionTo(CShape * shape)
{
	shape->getPosition(m_position[0], m_position[1], m_position[2]);
}

void CShape::setObjectName(GLuint name)
{
	m_objectName = name;
}

GLuint CShape::getObjectName()
{
	return m_objectName;
}

void CShape::setHighlightMaterial(CMaterial * material)
{
	m_highlightMaterial = material;
}

CMaterial* CShape::getHightlightMaterial()
{
	return m_highlightMaterial;
}

void CShape::setHighlight(THighlightState state)
{
	m_highlight = state;
}

void CShape::assignPointTo(CPoint3d* point)
{
	point->setComponents(m_position[0], m_position[1], m_position[2]);
}

void CShape::doCreateGeometry()
{
}

void CShape::doCreateSelect()
{
}

void CShape::setUseName(bool flag)
{
	m_renderName = flag;
}

bool CShape::getUseName()
{
	return m_renderName;
}

CShape::THighlightState CShape::getHighlight()
{
	return m_highlight;
}

void CShape::refresh()
{

}

void CShape::setTexture(CTexture *texture)
{
	m_texture = texture;
}

CTexture* CShape::getTexture()
{
	return m_texture;
}

void CShape::setNormalize(bool flag)
{
	m_normalize = flag;
}

void CShape::setSelectMaterial(CMaterial *material)
{
	m_selectMaterial = material;
}

void CShape::setPosition(CVec3d &pos)
{
	pos.getComponents(m_position);
}

void CShape::setPosition(CPoint3d &pos)
{
	pos.getComponents(m_position);
}

CVec3d& CShape::getPosition()
{
	CVec3d& pos = ivfGetTempVec3d();
	pos.setComponents(m_position);
	return pos;
}

void CShape::setRotation(CQuat &q)
{
	double vx, vy, vz, angle;
	q.getAxisAngle(vx, vy, vz, angle);
	this->setRotationQuat(vx, vy, vz, angle*360.0/2.0/M_PI);
}

void CShape::setHighlightType(THighlightType type)
{
	m_highlightType = type;
}

void CShape::enableHighlight()
{
	m_highlight = HS_ON;
}

void CShape::disableHighlight()
{
	m_highlight = HS_OFF;
}

bool CShape::isEnabledHighlight()
{
	return (m_highlight==HS_ON);
}
