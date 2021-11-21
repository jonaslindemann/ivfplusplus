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

#include <ivf/NodeCursor.h>

using namespace ivf;

CNodeCursor::CNodeCursor()
{
	m_size = 0.5;
	m_shapeSize = 0.0;
	m_cursorType = CT_XZ;
	m_cursorDirection = CD_OUTWARDS;
	m_cursorArrowType = AT_NORMAL;
	m_usePlane = false;

	this->setUseName(false);

	m_cursorShape = new CComposite();

	CMaterialPtr material = new CMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.5f, 0.1f, 1.0f);

	this->setMaterial(material);

	m_arrow = new CArrow();
	//m_arrow->setMaterial(material);
	m_arrow->setUseName(false);

	m_arrowXZ1 = new CTransform();
	m_arrowXZ1->addChild(m_arrow);
	m_arrowXZ1->setRotationQuat(1.0, 0.0, 0.0, 90.0);
	m_arrowXZ1->setUseName(false);

	m_arrowXZ2 = new CTransform();
	m_arrowXZ2->addChild(m_arrow);
	m_arrowXZ2->setRotationQuat(1.0, 0.0, 0.0, -90.0);
	m_arrowXZ2->setUseName(false);

	m_arrowXZ3 = new CTransform();
	m_arrowXZ3->addChild(m_arrow);
	m_arrowXZ3->setRotationQuat(0.0, 0.0, 1.0, 90.0);
	m_arrowXZ3->setUseName(false);

	m_arrowXZ4 = new CTransform();
	m_arrowXZ4->addChild(m_arrow);
	m_arrowXZ4->setRotationQuat(0.0, 0.0, 1.0, -90.0);
	m_arrowXZ4->setUseName(false);

	m_arrowY1 = new CTransform();
	m_arrowY1->addChild(m_arrow);
	m_arrowY1->setState(CGLBase::OS_OFF);
	m_arrowY1->setUseName(false);

	m_arrowY2 = new CTransform();
	m_arrowY2->addChild(m_arrow);
	m_arrowY2->setRotationQuat(0.0, 0.0, 1.0, -180.0);
	m_arrowY2->setState(CGLBase::OS_OFF);
	m_arrowY2->setUseName(false);

	CMaterialPtr planeMaterial = new CMaterial();
	planeMaterial->setDiffuseColor(0.0f, 0.0f, 0.0f, 0.5f);
	planeMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.5f);
	planeMaterial->setAmbientColor(0.0f, 0.0f, 0.0f, 0.5f);
	m_plane = new CQuadPlane();
	m_plane->setMaterial(planeMaterial);
	m_plane->setState(CGLBase::OS_OFF);
	m_plane->setOrientation(CQuadPlane::PO_XZ);
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

CNodeCursor::~CNodeCursor()
{

}

void CNodeCursor::setSize(double size)
{
	m_size = size;
	initCursor();
}

double CNodeCursor::getSize()
{
	return m_size;
}

void CNodeCursor::initCursor()
{
	if (m_cursorArrowType==AT_NORMAL)
		m_arrow->setSize(m_size*1.0, m_size*0.4);
	else
		m_arrow->setSize(m_size*0.5, m_size*0.5);
	m_arrow->setRadius(m_size*0.1, m_size*0.05);
	setDirection(m_cursorDirection);
}

void CNodeCursor::setType(TCursorType type)
{
	m_cursorType = type;

	switch (m_cursorType) {
	case CT_XZ:
		m_arrowY1->setState(CGLBase::OS_OFF);
		m_arrowY2->setState(CGLBase::OS_OFF);
		m_arrowXZ1->setState(CGLBase::OS_ON);
		m_arrowXZ2->setState(CGLBase::OS_ON);
		m_arrowXZ3->setState(CGLBase::OS_ON);
		m_arrowXZ4->setState(CGLBase::OS_ON);
		if (m_usePlane)
			m_plane->setState(CGLBase::OS_OFF);
		break;
	case CT_Y:
		m_arrowY1->setState(CGLBase::OS_ON);
		m_arrowY2->setState(CGLBase::OS_ON);
		m_arrowXZ1->setState(CGLBase::OS_OFF);
		m_arrowXZ2->setState(CGLBase::OS_OFF);
		m_arrowXZ3->setState(CGLBase::OS_OFF);
		m_arrowXZ4->setState(CGLBase::OS_OFF);
		if (m_usePlane)
			m_plane->setState(CGLBase::OS_ON);
		break;
	case CT_XYZ:
		m_arrowY1->setState(CGLBase::OS_ON);
		m_arrowY2->setState(CGLBase::OS_ON);
		m_arrowXZ1->setState(CGLBase::OS_ON);
		m_arrowXZ2->setState(CGLBase::OS_ON);
		m_arrowXZ3->setState(CGLBase::OS_ON);
		m_arrowXZ4->setState(CGLBase::OS_ON);
		break;
	case CT_Y_TOP:
		m_arrowY1->setState(CGLBase::OS_OFF);
		m_arrowY2->setState(CGLBase::OS_ON);
		m_arrowXZ1->setState(CGLBase::OS_OFF);
		m_arrowXZ2->setState(CGLBase::OS_OFF);
		m_arrowXZ3->setState(CGLBase::OS_OFF);
		m_arrowXZ4->setState(CGLBase::OS_OFF);
		break;
	case CT_NONE:
		m_arrowY1->setState(CGLBase::OS_OFF);
		m_arrowY2->setState(CGLBase::OS_OFF);
		m_arrowXZ1->setState(CGLBase::OS_OFF);
		m_arrowXZ2->setState(CGLBase::OS_OFF);
		m_arrowXZ3->setState(CGLBase::OS_OFF);
		m_arrowXZ4->setState(CGLBase::OS_OFF);
		if (m_usePlane)
			m_plane->setState(CGLBase::OS_OFF);
		break;
	default:
		break;
	}
}

void CNodeCursor::setShape(CShape *shape)
{
	m_nodeShape = shape;
}

void CNodeCursor::doCreateGeometry()
{
	m_cursorShape->render();

	if (m_nodeShape!=nullptr)
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

CShape* CNodeCursor::getShape()
{
	return m_nodeShape;
}

void CNodeCursor::setUsePlaneHelper(bool flag)
{
	m_usePlane = flag;
}

bool CNodeCursor::getUsePlaneHelper()
{
	return m_usePlane;
}

void CNodeCursor::setShapeSize(double size)
{
	m_shapeSize = size;
	initCursor();
}

void CNodeCursor::setDirection(TCursorDirection direction)
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

CNodeCursor::TCursorDirection CNodeCursor::getDirection()
{
	return m_cursorDirection;
}

void CNodeCursor::setArrowType(TCursorArrowType type)
{
	m_cursorArrowType = type;
	initCursor();
}

CNodeCursor::TCursorArrowType CNodeCursor::getArrowType()
{
	return m_cursorArrowType;
}
