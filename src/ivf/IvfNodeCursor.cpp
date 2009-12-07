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
//
// Written by Jonas Lindemann
//

#include <ivf/IvfNodeCursor.h>

CIvfNodeCursor::CIvfNodeCursor()
{
	m_size = 0.5;
	m_shapeSize = 0.0;
	m_cursorType = CT_XZ;
	m_cursorDirection = CD_OUTWARDS;
	m_cursorArrowType = AT_NORMAL;
	m_usePlane = false;

	this->setUseName(false);

	m_cursorShape = new CIvfComposite();

	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.5f, 0.1f, 1.0f);

	this->setMaterial(material);

	m_arrow = new CIvfArrow();
	//m_arrow->setMaterial(material);
	m_arrow->setUseName(false);

	m_arrowXZ1 = new CIvfTransform();
	m_arrowXZ1->addChild(m_arrow);
	m_arrowXZ1->setRotationQuat(1.0, 0.0, 0.0, 90.0);
	m_arrowXZ1->setUseName(false);

	m_arrowXZ2 = new CIvfTransform();
	m_arrowXZ2->addChild(m_arrow);
	m_arrowXZ2->setRotationQuat(1.0, 0.0, 0.0, -90.0);
	m_arrowXZ2->setUseName(false);

	m_arrowXZ3 = new CIvfTransform();
	m_arrowXZ3->addChild(m_arrow);
	m_arrowXZ3->setRotationQuat(0.0, 0.0, 1.0, 90.0);
	m_arrowXZ3->setUseName(false);

	m_arrowXZ4 = new CIvfTransform();
	m_arrowXZ4->addChild(m_arrow);
	m_arrowXZ4->setRotationQuat(0.0, 0.0, 1.0, -90.0);
	m_arrowXZ4->setUseName(false);

	m_arrowY1 = new CIvfTransform();
	m_arrowY1->addChild(m_arrow);
	m_arrowY1->setState(CIvfGLBase::OS_OFF);
	m_arrowY1->setUseName(false);

	m_arrowY2 = new CIvfTransform();
	m_arrowY2->addChild(m_arrow);
	m_arrowY2->setRotationQuat(0.0, 0.0, 1.0, -180.0);
	m_arrowY2->setState(CIvfGLBase::OS_OFF);
	m_arrowY2->setUseName(false);

	CIvfMaterialPtr planeMaterial = new CIvfMaterial();
	planeMaterial->setDiffuseColor(0.0f, 0.0f, 0.0f, 0.5f);
	planeMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.5f);
	planeMaterial->setAmbientColor(0.0f, 0.0f, 0.0f, 0.5f);
	m_plane = new CIvfQuadPlane();
	m_plane->setMaterial(planeMaterial);
	m_plane->setState(CIvfGLBase::OS_OFF);
	m_plane->setOrientation(CIvfQuadPlane::PO_XZ);
	m_plane->setSize(100.0, 100.0);
	m_plane->setUseName(false);

	m_cursorShape->addChild(m_arrowXZ1);
	m_cursorShape->addChild(m_arrowXZ2);
	m_cursorShape->addChild(m_arrowXZ3);
	m_cursorShape->addChild(m_arrowXZ4);
	m_cursorShape->addChild(m_arrowY1);
	m_cursorShape->addChild(m_arrowY2);

	initCursor();
}

CIvfNodeCursor::~CIvfNodeCursor()
{

}

void CIvfNodeCursor::setSize(double size)
{
	m_size = size;
	initCursor();
}

double CIvfNodeCursor::getSize()
{
	return m_size;
}

void CIvfNodeCursor::initCursor()
{
	if (m_cursorArrowType==AT_NORMAL)
		m_arrow->setSize(m_size*1.0, m_size*0.4);
	else
		m_arrow->setSize(m_size*0.5, m_size*0.5);
	m_arrow->setRadius(m_size*0.1, m_size*0.05);
	setDirection(m_cursorDirection);
}

void CIvfNodeCursor::setType(TCursorType type)
{
	m_cursorType = type;

	switch (m_cursorType) {
	case CT_XZ:
		m_arrowY1->setState(CIvfGLBase::OS_OFF);
		m_arrowY2->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ1->setState(CIvfGLBase::OS_ON);
		m_arrowXZ2->setState(CIvfGLBase::OS_ON);
		m_arrowXZ3->setState(CIvfGLBase::OS_ON);
		m_arrowXZ4->setState(CIvfGLBase::OS_ON);
		if (m_usePlane)
			m_plane->setState(CIvfGLBase::OS_OFF);
		break;
	case CT_Y:
		m_arrowY1->setState(CIvfGLBase::OS_ON);
		m_arrowY2->setState(CIvfGLBase::OS_ON);
		m_arrowXZ1->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ2->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ3->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ4->setState(CIvfGLBase::OS_OFF);
		if (m_usePlane)
			m_plane->setState(CIvfGLBase::OS_ON);
		break;
	case CT_XYZ:
		m_arrowY1->setState(CIvfGLBase::OS_ON);
		m_arrowY2->setState(CIvfGLBase::OS_ON);
		m_arrowXZ1->setState(CIvfGLBase::OS_ON);
		m_arrowXZ2->setState(CIvfGLBase::OS_ON);
		m_arrowXZ3->setState(CIvfGLBase::OS_ON);
		m_arrowXZ4->setState(CIvfGLBase::OS_ON);
		break;
	case CT_Y_TOP:
		m_arrowY1->setState(CIvfGLBase::OS_OFF);
		m_arrowY2->setState(CIvfGLBase::OS_ON);
		m_arrowXZ1->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ2->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ3->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ4->setState(CIvfGLBase::OS_OFF);
		break;
	case CT_NONE:
		m_arrowY1->setState(CIvfGLBase::OS_OFF);
		m_arrowY2->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ1->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ2->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ3->setState(CIvfGLBase::OS_OFF);
		m_arrowXZ4->setState(CIvfGLBase::OS_OFF);
		if (m_usePlane)
			m_plane->setState(CIvfGLBase::OS_OFF);
		break;
	default:
		break;
	}
}

void CIvfNodeCursor::setShape(CIvfShape *shape)
{
	m_nodeShape = shape;
}

void CIvfNodeCursor::doCreateGeometry()
{
	m_cursorShape->render();

	if (m_nodeShape!=NULL)
		m_nodeShape->render();

	if (m_usePlane)
	{
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_plane->render();
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}

CIvfShape* CIvfNodeCursor::getShape()
{
	return m_nodeShape;
}

void CIvfNodeCursor::setUsePlaneHelper(bool flag)
{
	m_usePlane = flag;
}

bool CIvfNodeCursor::getUsePlaneHelper()
{
	return m_usePlane;
}

void CIvfNodeCursor::setShapeSize(double size)
{
	m_shapeSize = size;
	initCursor();
}

void CIvfNodeCursor::setDirection(TCursorDirection direction)
{
	m_cursorDirection = direction;

	double sFactor;

	if (m_cursorArrowType==AT_NORMAL)
		sFactor=1.0;
	else
		sFactor=0.5;

	switch (m_cursorDirection) {
	case CD_OUTWARDS:
		m_arrow->setOffset(m_size*sFactor*0.5+m_shapeSize*0.5);
		break;
	case CD_INWARDS:
		m_arrow->setOffset(-m_size*sFactor*0.5-m_shapeSize*0.5);
		break;
	default:
		m_arrow->setOffset(m_size*sFactor*0.5+m_shapeSize*0.5);
		break;
	}
}

CIvfNodeCursor::TCursorDirection CIvfNodeCursor::getDirection()
{
	return m_cursorDirection;
}

void CIvfNodeCursor::setArrowType(TCursorArrowType type)
{
	m_cursorArrowType = type;
	initCursor();
}

CIvfNodeCursor::TCursorArrowType CIvfNodeCursor::getArrowType()
{
	return m_cursorArrowType;
}
