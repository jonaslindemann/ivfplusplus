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

// Implementation of: public class COldScene

#include <ivf/OldScene.h>

using namespace ivf;

// ------------------------------------------------------------
COldScene::COldScene ()
		:CSelectComposite()
{
	CCamera* camera = new CCamera();
	this->setCamera(camera);

	m_world = new CCoordinateSystem();
	m_world->addReference();
	m_cursor = new CCursor();
	m_cursor->addReference();
	m_tempShape = nullptr;
	m_transparentShape = nullptr;
	m_objectSize = 1;
	m_cursorLocked = false;

	m_lighting = CLighting::getInstance();

	m_showGrid = true;
	m_showCursor = true;
	m_snapToGrid = true;

	m_lightMode = IVF_LIGHT_LOCAL;
}

// ------------------------------------------------------------
COldScene::~COldScene ()
{
	if (m_cursor!=nullptr)
		m_cursor->deleteReference();
	if (m_world!=nullptr)
		m_world->deleteReference();

	if (!m_cursor->referenced())
		delete m_cursor;
	if (!m_world->referenced())
		delete m_world;
}

// ------------------------------------------------------------
void COldScene::setCamera(CCamera * camera)
{
	CSelectComposite::setCamera(camera);
}

// ------------------------------------------------------------
CCamera* COldScene::getCamera()
{
	return (CCamera*) CSelectComposite::getCamera();
}

// ------------------------------------------------------------
void COldScene::setView(CView *view)
{
	CSelectComposite::setCamera(view);
}

// ------------------------------------------------------------
CView* COldScene::getView()
{
	return CSelectComposite::getCamera();
}

// ------------------------------------------------------------
void COldScene::doCreateGeometry()
{
	CSelectComposite::doCreateGeometry();

	if (m_showGrid)
		if (m_world!=nullptr)
			m_world->render();


	if (m_showCursor)
		if (m_cursor!=nullptr)
			m_cursor->render();

	// Draw line from cursor to plane

	if (m_showCursor)
		if (m_cursor!=nullptr)
		{
			double x1, y1, z1;
			double x2, y2, z2;
			m_pointOnPlane.getComponents(x1, y1, z1);
			m_cursor->getPosition(x2, y2, z2);

			glPushAttrib(GL_LIGHTING);
			glPushMatrix();
			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 0.0);
			glVertex3d(x1, y1, z1);
			glColor3f(1.0, 1.0, 0.0);
			glVertex3d(x2, y2, z2);
			glEnd();
			glPopMatrix();
			glPopAttrib();
		}

	if (m_tempShape!=nullptr)
	{
		switch (m_tempRenderStyle) {
		case IVF_TEMP_SOLID:
			m_tempShape->render();
			break;
		case IVF_TEMP_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_tempShape->render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			m_tempShape->render();
			break;
		}
	}

	if (m_transparentShape!=nullptr)
		m_transparentShape->render();
}

void COldScene::doBeginTransform()
{
	glPushMatrix();
	if (m_lightMode == IVF_LIGHT_LOCAL)
		if (m_lighting != nullptr)
			m_lighting->render();
	CSelectComposite::doBeginTransform();
	if (m_lightMode == IVF_LIGHT_WORLD)
		if (m_lighting != nullptr)
			m_lighting->render();
}

void COldScene::doEndTransform()
{
		CSelectComposite::doEndTransform();
	glPopMatrix();
}

void COldScene::setWorldSystem(CCoordinateSystem * world)
{
	if (m_world!=nullptr)
	{
		m_world->deleteReference();

		if (!m_world->referenced())
			delete m_world;
	}

	m_world = world;
	m_world->addReference();
}

CCoordinateSystem* COldScene::getWorldSystem()
{
	return m_world;
}

void COldScene::setCursor(CCursor * cursor)
{
	if (m_cursor!=nullptr)
	{
		m_cursor->deleteReference();
		if (!m_cursor->referenced())
			delete m_cursor;
	}

	m_cursor = cursor;
	m_cursor->addReference();
}

CCursor* COldScene::getCursor()
{
	return m_cursor;
}

//end changes

void COldScene::updateCursor(int x, int y)
{
	double i, j, k;
	double ix, iy, iz;
	double x0, y0, z0;
	double dist;
	int viewWidth, viewHeight;
	double dy;

	CCamera* camera = (CCamera*) getCamera();

	if ((m_cursor!=nullptr)&&
		(camera!=nullptr)&&
		(m_world!=nullptr))
	{



		if (!m_cursorLocked)
		{
			// Calculate pick vector

			camera->pickVector(x, y, i, j, k);

			// Get camera position

			camera->getPosition(x0, y0, z0);

			// Calculate intersection with XZ plane

			m_world->intersect(x0, y0, z0,  i, j, k, ix, iy, iz);

			// Align cursor with coordinate system

			m_world->alignShape(m_cursor);

			// Snap to transformed system

			m_world->transform(ix, iy, iz, ix, iy, iz);

			if (m_snapToGrid)
				m_world->snap(ix, iy, iz);

			// cursor is positioned in the global system

			double planeX, planeY, planeZ;
			planeX = ix;
			planeZ = iz;
			planeY = 0.0;

			m_pointOnPlane.setComponents(planeX, planeY, planeZ);

			m_world->transformWorld(ix, iy, iz, ix, iy, iz);
			m_world->transformWorld(&m_pointOnPlane);
			m_cursor->setPosition(ix, iy, iz);
		}
		else
		{
			// Calculate y movement
			double sign;

			if ( (m_cursorY-m_cursorStartY)>0 )
				sign = -1.0;
			else
				sign = 1.0;

			dist = sign*sqrt(pow((double)m_cursorX-(double)m_cursorStartX,2) + pow((double)m_cursorY - (double)m_cursorStartY,2));
			camera->getViewPort(viewWidth, viewHeight);
			dy = 10*m_objectSize * (dist / sqrt(pow((double)viewHeight,2) + pow((double)viewWidth,2)));

			// move cursor in transformed y direction

			m_cursor->getPosition(ix, iy, iz);
			m_world->transform(ix, iy, iz, ix, iy, iz);
			iy = dy;

			if (m_snapToGrid)
				m_world->snap(ix, iy, iz);

			m_world->transformWorld(ix, iy, iz, ix, iy, iz);
			m_cursor->setPosition(ix, iy, iz);
		}
	}

	m_cursorX = x;
	m_cursorY = y;
}

void COldScene::setTempShape(CShape * shape)
{
	m_tempShape = shape;
}

CShape* COldScene::getTempShape()
{
	return m_tempShape;
}

void COldScene::lockCursor()
{
	m_cursorLocked = true;
	m_cursorStartX = m_cursorX;
	m_cursorStartY = m_cursorY;
}

void COldScene::unlockCursor()
{
	m_cursorLocked = false;
}

int COldScene::isCursorLocked()
{
	return m_cursorLocked;
}

void COldScene::setObjectSize(double size)
{
	m_objectSize = size;
}

void COldScene::setNodeSize(double size)
{
	//g_nodeSize = size;
}

void COldScene::setWorkspace(double size)
{
	m_world->setWorkspace(size);
	double snapUnit = m_world->getSnapUnit();
	m_cursor->setSize(snapUnit*10.0f);
	this->setObjectSize(size/10.0f);
}

void COldScene::setTransparentShape(CShape *shape)
{
	m_transparentShape = shape;
}

void COldScene::setShowGrid(bool flag)
{
	m_showGrid = flag;
}

void COldScene::setShowCursor(bool flag)
{
	m_showCursor = flag;
}

bool COldScene::getShowGrid()
{
	return m_showGrid;
}

bool COldScene::getShowCursor()
{
	return m_showCursor;
}

void COldScene::setTempRenderStyle(int style)
{
	m_tempRenderStyle = style;
}

int COldScene::getTempRenderStyle()
{
	return m_tempRenderStyle;
}

void COldScene::setSnapToGrid(bool flag)
{
	m_snapToGrid = flag;
}

bool COldScene::getSnapToGrid()
{
	return m_snapToGrid;
}

void COldScene::setLightMode(int mode)
{
	m_lightMode = mode;
}

int COldScene::getLightMode()
{
	return m_lightMode;
}


