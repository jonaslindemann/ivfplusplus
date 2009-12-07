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

#include <ivf/IvfCulledScene.h>

#include <ivf/IvfCamera.h>

// ------------------------------------------------------------
CIvfCulledScene::CIvfCulledScene()
{
	m_matrixStack = new CIvfMatrixStack();

	m_frustum = new CIvfViewFrustum();

	m_useCulling = true;
	m_cullCount = 0;
	m_cullView = NULL;

	this->initBoundingSphere();
	this->getBoundingSphere()->setRadius(0.0);
}

// ------------------------------------------------------------
CIvfCulledScene::~CIvfCulledScene()
{
	delete m_matrixStack;
	delete m_frustum;
}

// ------------------------------------------------------------
void CIvfCulledScene::addChild(CIvfShape *shape)
{
	CIvfScene::addChild(shape);

	// Initialize bounding sphere

	shape->initBoundingSphere();
}

// ------------------------------------------------------------
void CIvfCulledScene::cull()
{
	CIvfView* view;
	if (m_cullView==NULL)
		view = this->getCamera();
	else
		view = m_cullView;

	view->getViewFrustum(m_frustum);

	m_cullCount = 0;

	cullChildren(this);
}

// ------------------------------------------------------------
void CIvfCulledScene::cullChildren(CIvfShape *shape)
{
	// Recursively cull children

	int i;
	double x, y, z;
	double wx, wy, wz;
	double vx, vy, vz, theta;

	if (shape->isClass("CIvfComposite"))
	{
		CIvfComposite* composite = (CIvfComposite*) shape;

		composite->getPosition(x, y, z);
		m_matrixStack->getWorldCoordinate(x, y, z, wx, wy, wz);
		composite->getBoundingSphere()->setGlobalPosition(wx, wy, wz);

		//cout << composite->getBoundingSphere()->getRadius() << endl;

		// if (composite bounding sphere intersects view frustum)
		//    turn composite shape on and
		//    traverse and cull children
		// else
		//    turn composite shape off

		if ((composite==(CIvfComposite*)this)||intersectFrustum(composite->getBoundingSphere()))
		{
			m_matrixStack->pushMatrix();

			composite->getRotationQuat(vx, vy, vz, theta);

			//if ((x!=0.0)||(y!=0.0)||(z!=0.0))
				m_matrixStack->translate(x, y, z);

			if (theta!=0)
				m_matrixStack->rotate(vx, vy, vz, theta);

			for (i = 0; i<composite->getSize(); i++)
			{
				CIvfShape* child = composite->getChild(i);
				cullChildren(child);
			}
			m_matrixStack->popMatrix();
		}
		else
		{
			//composite->setState(IVF_OBJECT_OFF);
			composite->setCulled(true);
			m_cullCount++;
		}

	}
	else
	{
		// if (shape intersects view frustum)
		//    turn shape on
		// else
		//    turn shape off

		shape->getPosition(x, y, z);
		m_matrixStack->getWorldCoordinate(x, y, z, x, y, z);
		shape->getBoundingSphere()->setGlobalPosition(x, y, z);

		if (!intersectFrustum(shape->getBoundingSphere()))
		{
			shape->setCulled(true);
			m_cullCount++;
		}
	}
}

bool CIvfCulledScene::intersectFrustum(CIvfBoundingSphere *bSphere)
{
	return m_frustum->isInside(bSphere);
}

int CIvfCulledScene::getCullCount()
{
	return m_cullCount;
}

void CIvfCulledScene::doCreateGeometry()
{
	if (m_useCulling)
		this->cull();

	CIvfScene::doCreateGeometry();
}

void CIvfCulledScene::setUseCulling(bool flag)
{
	m_useCulling = flag;
}

bool CIvfCulledScene::getUseCulling()
{
	return m_useCulling;
}

void CIvfCulledScene::setCullView(CIvfView *view)
{
	m_cullView = view;
}

CIvfView* CIvfCulledScene::getCullView()
{
	return m_cullView;
}
